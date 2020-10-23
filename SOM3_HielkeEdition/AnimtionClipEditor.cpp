#include "AnimtionClipEditor.h"

#include "EditorWindow.h"
#include "TextField.h"
#include "AnimationWindow.h"
#include "AddFrameWindow.h"
#include "ButtonBuilder.h"

AnimationClipEditor::AnimationClipEditor()
{
	m_ResMan = NULL;
	Init();
}

AnimationClipEditor::AnimationClipEditor(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
	Init();
}

AnimationClipEditor::~AnimationClipEditor()
{
	delete m_AnimationWindow;
	m_AnimationWindow = nullptr;
	delete m_AddFrameWindow;
	m_AddFrameWindow = nullptr;
}

void AnimationClipEditor::Init()
{
	//Load default resources
	LoadDefaultAssets();

	m_MousePos = Vector2(0, 0);
	m_Position = Vector2(0, 0);
	m_DragStart = Vector2(0, 0);
	m_DragEnd = Vector2(0, 0);
	m_ClipName = "newClip";
	m_FileName = "newClip.hanimclip";
	m_Dragging = false;
	std::string windowName = "Animation Window";


	m_LoadObject.m_Pos = Vector2{ 0, 0 };
	m_LoadObject.m_Size = Vector2{ 25,25 };
	m_LoadButtonCollider.pos = m_LoadObject.m_Pos;
	m_LoadButtonCollider.w = m_LoadObject.m_Size.x;
	m_LoadButtonCollider.h = m_LoadObject.m_Size.y;

	AnimationWindow* animWindow = new AnimationWindow(Vector2{ 700,100 }, windowName, m_EditorIconsTexture);
	AddFrameWindow* frameWin = new AddFrameWindow(Vector2{ 300,300 }, std::string("AddFrame window"), m_EditorIconsTexture);
	m_AddFrameWindow = frameWin;

	m_OpenAddFrameWindow = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 25,0 }, { 100,25 }, 1, std::bind(&AnimationClipEditor::OpenAddFrameWindow, this),
		"AddFrame", Button::DrawMode::FILLEDRECT, { 0,140,15,255 }, { 0,160,15, 255 }, { 0,235,15,255 }, {0,0,0,255});

	m_OpenAnimationWindow = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 125,0 }, { 100,25 }, 1, std::bind(&AnimationClipEditor::OpenAddFrameWindow, this),
		"Animation", Button::DrawMode::FILLEDRECT, { 0,140,15,255 }, { 0,160,15, 255 }, { 0,235,15,255 }, { 0,0,0,255 });

	Object obj = Object{};


	m_AnimationWindow = animWindow;

	m_EditorWindows.push_back(m_AddFrameWindow);
	m_EditorWindows.push_back(m_AnimationWindow);

	if (m_DefaultFont != nullptr)
	{
		m_OpenAddFrameWindow.SetFont(m_DefaultFont);
		m_OpenAnimationWindow.SetFont(m_DefaultFont);
		m_AnimationWindow->SetFont(m_DefaultFont);
		m_AddFrameWindow->SetFont(m_DefaultFont);
	}

	m_Buttons.push_back(&m_OpenAddFrameWindow);
	m_Buttons.push_back(&m_OpenAnimationWindow);
}

void AnimationClipEditor::KeyDown(unsigned int _key)
{

	switch (_key)
	{
	case SDLK_UP:
		m_Up = true;
		break;
	case SDLK_DOWN:
		m_Down = true;
		break;
	case SDLK_RIGHT:
		m_Right = true;
		break;
	case SDLK_LEFT:
		m_Left = true;
		break;
	case SDLK_LSHIFT:
		m_Sprinting = true;
		break;
	}
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->KeyDown(_key);
	}
}

void AnimationClipEditor::KeyUp(unsigned int _key)
{
	switch (_key)
	{
	case SDLK_UP:
		m_Up = false;
		break;
	case SDLK_DOWN:
		m_Down = false;
		break;
	case SDLK_RIGHT:
		m_Right = false;
		break;
	case SDLK_LEFT:
		m_Left = false;
		break;
	case SDLK_EQUALS:
		m_Zoom += 0.1f;
		break;
	case SDLK_MINUS:
		m_Zoom -= 0.1f;
		break;
	case SDLK_LSHIFT:
		m_Sprinting = false;
		break;
	}

	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->KeyUp(_key);
	}
}

void AnimationClipEditor::MouseDown(unsigned int _key)
{
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseDown(_key);
	}
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
	if (_key == SDL_BUTTON_LEFT)
	{
		m_Dragging = true;
		m_DragStart = m_MousePos+ m_Position;
	}
}

void AnimationClipEditor::MouseUp(unsigned int _key)
{
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseUp(_key);
	}
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
	if (_key == SDL_BUTTON_LEFT)
	{
		m_DragEnd = m_MousePos + m_Position;
		m_Dragging = false;

		if (m_LoadButtonCollider.BoxCollision(m_LoadButtonCollider, m_MousePos))
		{
			m_LoadWindowActive = true;
			LoadWindowThingy();
		}
	}
}

void AnimationClipEditor::MouseMove(int _x, int _y)
{
	m_MousePos = Vector2(_x, _y);
	if (m_LoadButtonCollider.BoxCollision(m_LoadButtonCollider, m_MousePos))
	{
		m_HoverLoadButton = true;
	}
	else
	{
		m_HoverLoadButton = false;
	}
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseMove(_x,_y);
	}
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x,_y);
	}

}

void AnimationClipEditor::GenerateNumberedBoxes()
{
	m_NumbrdBoxes.clear();
	if (m_CurrentClip.m_ClipName != " ")
	{
		//Generate numbered boxes based on the current clips rects
		for (unsigned int i = 0; i < m_CurrentClip.m_SourceRects.size(); ++i)
		{
			NumberedBox nb = NumberedBox(i);
			Box box = Box{};

			const SDL_Rect& rect = m_CurrentClip.m_SourceRects[i];
			box.pos = Vector2{ (float)rect.x,(float)rect.y };
			box.w = rect.w;
			box.h = rect.h;

			nb.SetBox(box);
			nb.SetColour({200,200,200,255});
			nb.SetFont(m_DefaultFont);
			m_NumbrdBoxes.push_back(nb);

		}

	}
}

void AnimationClipEditor::LoadWindowThingy()
{
	WindowOpener opener = WindowOpener();
	std::string path = opener.PrintAndOpenStuff();
	if (path != "CANCELED")
	{
		m_CurrentClip = AnimationClip();// "clear the previous animation clip then load
		m_CurrentClip.LoadClipFromFile(path, m_ResMan);

		//m_CurrentClip.m_Looping = true;
		m_SpriteSheet.m_RenderInterface.textureName = m_CurrentClip.m_SourceTexture->GetName();
		m_SpriteSheet.m_Size = Vector2{(float)m_CurrentClip.m_SourceTexture->GetWidth(),(float) m_CurrentClip.m_SourceTexture->GetHeight()};
		m_SpriteSheet.m_RenderInterface.srcRect.w = m_SpriteSheet.m_Size.x;
		m_SpriteSheet.m_RenderInterface.srcRect.h = m_SpriteSheet.m_Size.y;
		//m_CurrentClip.Play();
		GenerateNumberedBoxes();
		if (m_AnimationWindow != nullptr)
		{
			m_AnimationWindow->SetClip(&m_CurrentClip);
			m_AddFrameWindow->SetClip(&m_CurrentClip);
		}
	}
}

void AnimationClipEditor::Update(float _dt)
{
	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->Update(_dt);
		if (m_EditorWindows[i]->m_Dragging)
		{
			m_Dragging = false;
		}
		if (m_EditorWindows[i]->m_ReadyForDelete)
		{
			m_EditorWindows.erase(m_EditorWindows.begin() + i);
			continue;
		}

	}
	
	if (m_Dragging)
	{
		m_DragEnd = m_MousePos + m_Position;
		m_SelectionBox.pos = m_DragStart;
		m_SelectionBox.w = m_DragEnd.x - m_DragStart.x;
		m_SelectionBox.h = m_DragEnd.y - m_DragStart.y;
	}

	//Figure out velocity
	Vector2 direction = Vector2{ 0,0 };
	float sprintMod = m_Sprinting ? m_SprintMultiplier : 1;
	if (m_Up)
	{
		direction.y -= 1;
	}
	if (m_Left)
	{
		direction.x -= 1;
	}
	if (m_Right)
	{
		direction.x += 1;
	}
	if (m_Down)
	{
		direction.y += 1;
	}

	m_Position += direction * m_Speed * _dt*m_Zoom*sprintMod;

}

void AnimationClipEditor::SetResourceManager(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
	LoadDefaultAssets();
}


void AnimationClipEditor::Render(SDLRenderer* _renderer)
{
	Vector2 zoomVector = {m_Zoom,m_Zoom};
	m_SpriteSheet.Render(_renderer, m_Position,zoomVector,0);
	m_LoadObject.Render(_renderer, Vector2{ 0,0 },1);
	_renderer->DrawBox(m_SelectionBox, { 255,255,255,255 },m_Position);
	_renderer->DrawFilledBox(0, 0, 1280, 25, SDL_Color{ 0,122,0,255 });


	for (unsigned int i = 0; i < m_NumbrdBoxes.size(); ++i)
	{
		m_NumbrdBoxes[i].Render(_renderer, m_Position,zoomVector,false);
	}

	if (m_HoverLoadButton)
	{
		_renderer->DrawBox(m_LoadButtonCollider, { 255,255,255,255 }, Vector2(0,0),1);
	}

	for (int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->Render(_renderer);
	}
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}


}

void AnimationClipEditor::LoadDefaultAssets()
{
	if (m_ResMan != nullptr)
	{
		m_ResMan->LoadDefaultMedia();
		m_CurrentTexture =m_ResMan->LoadTexture("Assets//SpriteSheets//Duran//seikendensetsu3_duran_sheet.png");

		m_EditorIconsTexture = m_ResMan->LoadTexture("Assets//editor//sprite editor icons.png");
		
		m_SpriteSheet.m_RenderInterface.textureName = "Assets//SpriteSheets//Duran//seikendensetsu3_duran_sheet.png";
		m_SpriteSheet.m_Size = Vector2{ (float)m_CurrentTexture->GetWidth(),(float)m_CurrentTexture->GetHeight() };
		m_SpriteSheet.m_RenderInterface.srcRect.w = m_CurrentTexture->GetWidth();
		m_SpriteSheet.m_RenderInterface.srcRect.h = m_CurrentTexture->GetHeight();
		
		m_DefaultFont = m_ResMan->LoadFont("Assets//Fonts//LucidaBrightRegular.ttf",16);
		//m_DefaultFont =m_ResMan->GetFont("Assets//Fonts//arial.ttf");

		

		//Icons loaded configure src rect
		if (m_EditorIconsTexture != nullptr)
		{
			m_LoadObject.m_RenderInterface.textureName = m_EditorIconsTexture->GetName();
			m_LoadObject.m_RenderInterface.srcRect = {48,0,16,16};
		}
		
	}
}

void AnimationClipEditor::OpenAddFrameWindow()
{
	bool found = false;

	//If the add frame window is not within the 
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		if (m_EditorWindows[i] == m_AddFrameWindow)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		//set can delete to false;
		m_AddFrameWindow->m_ReadyForDelete = false;
		m_AddFrameWindow->MouseMove((unsigned int)m_MousePos.x, (unsigned int)m_MousePos.y);
		m_EditorWindows.push_back(m_AddFrameWindow);
	}
}

void AnimationClipEditor::OpenAnimationWindow()
{
	bool found = false;
	//If the add frame window is not within the 
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		if (m_EditorWindows[i] == m_AnimationWindow)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		//set can delete to false;
		m_AnimationWindow->m_ReadyForDelete = false;
		m_AnimationWindow->MouseMove((unsigned int)m_MousePos.x, (unsigned int)m_MousePos.y);
		m_EditorWindows.push_back(m_AnimationWindow);
	}
}
