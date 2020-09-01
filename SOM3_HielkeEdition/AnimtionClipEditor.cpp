#include "AnimtionClipEditor.h"

#include "EditorWindow.h"


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
	delete m_WindowTest;
	m_WindowTest = nullptr;
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
	std::string windowName = "Testwindow";


	m_LoadObject.m_Pos = Vector2{0, 0};
	m_LoadObject.m_Size = Vector2{ 25,25 };
	m_LoadButtonCollider.pos = m_LoadObject.m_Pos;
	m_LoadButtonCollider.w = m_LoadObject.m_Size.x;
	m_LoadButtonCollider.h = m_LoadObject.m_Size.y;

	
	m_WindowTest = new EditorWindow(Vector2{ 100,100 }, windowName, m_EditorIconsTexture);

	if (m_DefaultFont != nullptr)
	{
		m_WindowTest->SetFont(m_DefaultFont);
	}

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
	}
}

void AnimationClipEditor::MouseDown(unsigned int _key)
{
	if (m_WindowTest != nullptr)
	{
		m_WindowTest->MouseDown(_key);
	}
	if (_key == SDL_BUTTON_LEFT)
	{
		m_Dragging = true;
		m_DragStart = m_MousePos+ m_Position;
	}
}

void AnimationClipEditor::MouseUp(unsigned int _key)
{
	if (m_WindowTest != nullptr)
	{
		m_WindowTest->MouseUp(_key);
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
	if (m_WindowTest != nullptr)
	{
		m_WindowTest->MouseMove(_x, _y);
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
		m_CurrentClip.Play();

		if (m_WindowTest != nullptr)
		{
			m_WindowTest->SetName(m_CurrentClip.m_ClipName);
			m_WindowTest->SetClip(&m_CurrentClip);
		}
	}
}

void AnimationClipEditor::Update(float _dt)
{
	if (m_WindowTest != nullptr)
	{
		m_CurrentClip.Update(_dt);
	
		try
		{
		
			m_CurrentAnimationObject.m_RenderInterface.srcRect = m_CurrentClip.GetRect();
			m_CurrentAnimationObject.m_RenderInterface.textureName = m_SpriteSheet.m_RenderInterface.textureName;

			m_WindowTest->SetShowingObject(m_CurrentAnimationObject);
		}
		catch (std::exception& e)
		{
			//std::cout << e.what() << "\n";
		}
		m_WindowTest->Update(_dt);
		if (m_WindowTest->m_Dragging)
		{
			m_Dragging = false;
		}
		if (m_WindowTest->CanDelete())
		{
			delete m_WindowTest;
			m_WindowTest = nullptr;
			printf("yeetus deletus");
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

	m_Position += direction * m_Speed * _dt;

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
	//m_CurrentAnimationObject.Render(_renderer, Vector2(0, 0));
	m_LoadObject.Render(_renderer, Vector2{ 0,0 },1);
	_renderer->DrawBox(m_SelectionBox, { 255,255,255,255 },m_Position);
	_renderer->DrawFilledBox(0, 0, 1280, 25, SDL_Color{ 0,122,0,255 });
	
	if (m_HoverLoadButton)
	{
		_renderer->DrawBox(m_LoadButtonCollider, { 255,255,255,255 }, Vector2(0,0),1);
	}

	if (m_WindowTest != nullptr)
	{
		m_WindowTest->Render(_renderer);
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
