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
	m_ClipName = "newClip";
	m_FileName = "newClip.hanimclip";
	std::string windowName = "Animation Window";

	AnimationWindow* animWindow = new AnimationWindow(Vector2{ 700,100 }, windowName, m_EditorIconsTexture);
	AddFrameWindow* frameWin = new AddFrameWindow(Vector2{ 300,300 }, std::string("AddFrame window"), m_EditorIconsTexture);
	m_AddFrameWindow = frameWin;
	
	m_NewFileButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 50,0 }, { 100,25 }, 1, std::bind(&AnimationClipEditor::CreateNewFile, this),
		"New clip", Button::DrawMode::FILLEDRECT, { 0,140,15,255 }, { 0,160,15, 255 }, { 0,235,15,255 }, { 0,0,0,255 });

	m_OpenAddFrameWindow = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 150,0 }, { 100,25 }, 1, std::bind(&AnimationClipEditor::OpenAddFrameWindow, this),
		"AddFrame", Button::DrawMode::FILLEDRECT, { 0,140,15,255 }, { 0,160,15, 255 }, { 0,235,15,255 }, {0,0,0,255});

	m_OpenAnimationWindow = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 250,0 }, { 100,25 }, 1, std::bind(&AnimationClipEditor::OpenAnimationWindow, this),
		"Animation", Button::DrawMode::FILLEDRECT, { 0,140,15,255 }, { 0,160,15, 255 }, { 0,235,15,255 }, { 0,0,0,255 });

	m_LoadButton = ButtonBuilder::BuildButton({ 0,0 }, { 25,25 }, 1, std::bind(&AnimationClipEditor::LoadClip, this));
	m_SaveButton = ButtonBuilder::BuildButton({ 25,0 }, { 25,25 }, 1, std::bind(&AnimationClipEditor::SaveClip, this));
	//TODO make sure we get the window width and height to use for this instead
	m_ZoomInButton = ButtonBuilder::BuildButton({1280-50,0}, { 25,25 }, 1, std::bind(&AnimationClipEditor::ZoomIn, this));
	m_ZoomOutButton = ButtonBuilder::BuildButton({ 1280-25,0 }, { 25,25 }, 1, std::bind(&AnimationClipEditor::ZoomOut, this));

	SDL_Rect norm = {96,16,16,16};
	SDL_Rect clicked = {128,16,16,16};
	SDL_Rect hover = {112,16,16,16};

	if (m_EditorIconsTexture != nullptr)
	{
		m_SaveButton.SetTextureDrawModeWithSheet(m_EditorIconsTexture->GetName(), norm, clicked, hover);
		norm = { 96,32,16,16 };
		clicked = { 128,32,16,16 };
		hover = { 112,32,16,16 };
		m_LoadButton.SetTextureDrawModeWithSheet(m_EditorIconsTexture->GetName(), norm, clicked, hover);
		
		norm = { 48,48,16,16 };
		clicked = { 80,48,16,16 };
		hover = { 64,48,16,16 };
		m_ZoomInButton.SetTextureDrawModeWithSheet(m_EditorIconsTexture->GetName(), norm, clicked, hover);
		norm = { 96,48,16,16 };
		clicked = { 128,48,16,16 };
		hover = { 112,48,16,16 };
		m_ZoomOutButton.SetTextureDrawModeWithSheet(m_EditorIconsTexture->GetName(), norm, clicked, hover);



		m_Buttons.push_back(&m_ZoomInButton);
		m_Buttons.push_back(&m_ZoomOutButton);
		m_Buttons.push_back(&m_LoadButton);
		m_Buttons.push_back(&m_SaveButton);
	}

	Object obj = Object{};


	m_AnimationWindow = animWindow;
	m_EditorWindows.push_back(m_AddFrameWindow);
	m_EditorWindows.push_back(m_AnimationWindow);


	if (m_DefaultFont != nullptr)
	{
		m_OpenAddFrameWindow.SetFont(m_DefaultFont);
		m_OpenAnimationWindow.SetFont(m_DefaultFont);
		m_NewFileButton.SetFont(m_DefaultFont);
		m_AnimationWindow->SetFont(m_DefaultFont);
		m_AddFrameWindow->SetFont(m_DefaultFont);
	}


	m_Buttons.push_back(&m_OpenAddFrameWindow);
	m_Buttons.push_back(&m_OpenAnimationWindow);
	m_Buttons.push_back(&m_NewFileButton);

	m_HighlightedBoxColour = {255,0,0,255};
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

}

void AnimationClipEditor::MouseMove(int _x, int _y)
{
	m_MousePos = Vector2(_x, _y);
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
		if (m_EditorWindows[i]->m_ReadyForDelete)
		{
			m_EditorWindows.erase(m_EditorWindows.begin() + i);
			continue;
		}
		if (m_EditorWindows[i]->m_ChangeToAnimationClip)
		{
			GenerateNumberedBoxes();
			m_EditorWindows[i]->m_ChangeToAnimationClip = false;
		}
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
	m_AddFrameWindow->m_ViewPos = m_Position;
	m_AddFrameWindow->m_ZoomVector = {m_Zoom,m_Zoom};

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
	_renderer->DrawFilledBox(0, 0, 1280, 25, SDL_Color{ 0,122,0,255 });


	for (unsigned int i = 0; i < m_NumbrdBoxes.size(); ++i)
	{
		if(i == m_CurrentClip.m_CurrentIndex)
		{
			m_NumbrdBoxes[i].Render(_renderer, m_Position, m_HighlightedBoxColour, zoomVector, false);
		}
		else
		{
			m_NumbrdBoxes[i].Render(_renderer, m_Position,zoomVector,false);
		}
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

void AnimationClipEditor::LoadClip()
{
	WindowOpener opener = WindowOpener();
	std::string path = opener.PrintAndOpenStuff();
	if (path != "CANCELED")
	{
		m_CurrentClip = AnimationClip();// "clear the previous animation clip then load
		m_CurrentClip.LoadClipFromFile(path, m_ResMan);

		//m_CurrentClip.m_Looping = true;
		m_SpriteSheet.m_RenderInterface.textureName = m_CurrentClip.m_SourceTexture->GetName();
		m_SpriteSheet.m_Size = Vector2{ (float)m_CurrentClip.m_SourceTexture->GetWidth(),(float)m_CurrentClip.m_SourceTexture->GetHeight() };
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

void AnimationClipEditor::SaveClip()
{
	WindowOpener opener = WindowOpener();
	std::string path = opener.SaveFileStuff();
	if (path != "thisisnotasavelocation")
	{
		m_CurrentClip.SaveClipToFilePath(path);
	}
}

void AnimationClipEditor::ZoomIn()
{
	m_Zoom += 0.1f;
}

void AnimationClipEditor::ZoomOut()
{
	m_Zoom -= 0.1f;
}

//Create new animation clip file, allows you to choose for a texture and then creates a 
// animation clip based on this filepath
void AnimationClipEditor::CreateNewFile()
{
	WindowOpener opener = WindowOpener();
	std::string path = opener.PrintAndOpenPng();
	if (path != "CANCELED")
	{
		//replace 


		m_CurrentClip = AnimationClip();// 
		m_CurrentClip.m_SourceTexture = m_ResMan->LoadTexture(path);

		m_SpriteSheet.m_RenderInterface.textureName = m_CurrentClip.m_SourceTexture->GetName();
		m_SpriteSheet.m_Size = Vector2{ (float)m_CurrentClip.m_SourceTexture->GetWidth(),(float)m_CurrentClip.m_SourceTexture->GetHeight() };
		m_SpriteSheet.m_RenderInterface.srcRect.w = m_SpriteSheet.m_Size.x;
		m_SpriteSheet.m_RenderInterface.srcRect.h = m_SpriteSheet.m_Size.y;

		GenerateNumberedBoxes();
		if (m_AnimationWindow != nullptr)
		{
			m_AnimationWindow->SetClip(&m_CurrentClip);
			m_AddFrameWindow->SetClip(&m_CurrentClip);
		}
	}
}
