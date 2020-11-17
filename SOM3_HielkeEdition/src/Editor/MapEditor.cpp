#include "MapEditor.h"
#include "..\Engine\ButtonBuilder.h"
#include "..\Engine\WindowOpener.h"
using namespace Hielke;

MapEditor::MapEditor()
{
	printf("Default initialization not feasible without resource manager mapeditor.cpp \n");
}

MapEditor::MapEditor(ResourceManager* _resMan)
{
	m_ResourceManager = _resMan;
	Init();
}

MapEditor::~MapEditor()
{
	m_IconsTexture = nullptr;//Texture will be removed in the resource manager
	m_EditorWindows.clear();
}

void MapEditor::Update(float _dt)
{
	Vector2 vel  = { 0,0 };
	if (m_Kup)
	{
		vel.y -= 1;
	}
	if (m_Kright)
	{
		vel.x += 1;
	}
	if (m_Kleft)
	{
		vel.x -= 1;
	}
	if (m_Kdown)
	{
		vel.y += 1;
	}
	if (m_Kshift)
	{
		vel *= m_SprintMultiplier;
	}
	vel *= m_MoveSpeed*m_Zoom;
	m_WorldPos += vel * _dt;



	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Update(_dt);
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->Update(_dt);
		if (m_EditorWindows[i]->m_ReadyForDelete)
		{
			m_EditorWindows.erase(m_EditorWindows.begin() + i);
			continue;
		}
	}

}

void MapEditor::Render(SDLRenderer* _renderer)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->Render(_renderer);
	}
	if (m_CurrentMap != nullptr)
	{
		m_CurrentMap->RenderZoomed(_renderer, m_WorldPos,m_Zoom);
	}
}

void MapEditor::KeyDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->KeyDown(_key);
	}
	switch (_key)
	{
	case SDLK_LEFT:
		m_Kleft = true;
		break;
	case SDLK_DOWN:
		m_Kdown = true;
		break;
	case SDLK_UP:
		m_Kup = true;
		break;
	case SDLK_RIGHT:
		m_Kright = true;
		break;
	case SDLK_LSHIFT:
		m_Kshift = true;
		break;
	}
}

void MapEditor::KeyUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->KeyUp(_key);
	}

	switch (_key)
	{
	case SDLK_LEFT:
		m_Kleft = false;
		break;
	case SDLK_DOWN:
		m_Kdown = false;
		break;
	case SDLK_UP:
		m_Kup = false;
		break;
	case SDLK_RIGHT:
		m_Kright = false;
		break;
	case SDLK_LSHIFT:
		m_Kshift = false;
		break;
	}
}

void MapEditor::MouseUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseUp(_key);
	}
}

void MapEditor::MouseDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseDown(_key);
	}
}

void MapEditor::MouseMove(int _x, int _y)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x,_y);
	}
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		m_EditorWindows[i]->MouseMove(_x, _y);
	}
	m_MousePos.x = _x;
	m_MousePos.y = _y;
}

void MapEditor::ZoomIn()
{
	m_Zoom += 0.1f;
}

void MapEditor::ZoomOut()
{
	if (m_Zoom > 0.1f)
	{
		m_Zoom -= 0.1f;
	}
}

void MapEditor::SaveMap()
{
	printf(" trying to save map but functionality not implented yet :( mapeditor.cpp \n");
	//Exports map
}

void MapEditor::LoadMap()
{
	//Sets map

	std::string filepath = WindowOpener::GetFilepath("hmap");

	m_CurrentMap = new Map();
	m_CurrentMap =m_ResourceManager->LoadMap(filepath);
	if (m_CurrentMap != nullptr)
	{
		//Reset world pos and zoom after loading 
		m_WorldPos = Vector2{ 0,0 };
		m_Zoom = 1.0f;
		for (int i = 0; i < m_EditorWindows.size(); ++i)
		{
			m_EditorWindows[i]->SetMap(m_CurrentMap);
		}
	}
	
}

void MapEditor::GiveTopBarBox(Box _topBarBox)
{
	Vector2 currentPos = _topBarBox.pos;
	//will put the buttons according to the size of this box
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		//Adjust height
		m_Buttons[i]->SetPosition(currentPos);
		m_Buttons[i]->SetSize({ m_Buttons[i]->GetSize().x, _topBarBox.h });
		currentPos.x += m_Buttons[i]->GetSize().x;
	}
}

void MapEditor::Init()
{
	try
	{
		GetDefaultAssets();
	}
	catch (std::exception& e)
	{
		printf(e.what());
		return;
	}
	m_WorldPos = { 0,0 };
	m_MoveSpeed = 200.0f;//movement speed is in pixels
	m_SprintMultiplier = 3.0f;

	m_Kdown = false;
	m_Kleft = false;
	m_Kup = false;
	m_Kright = false;
	m_Kshift = false;

	
	std::string windowName = "Add Collider";
	m_ColliderWindow = new AddMapColliderWindow(Vector2{ 600,300 }, windowName, m_IconsTexture);

	m_AddColliderButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 150,30 }, 1, std::bind(&MapEditor::CollidorWindowCallback, this),
		"AddCollider", Button::DrawMode::FILLEDRECT, { 0,122,0,255 }, { 10,160,10,255 }, { 0,80,0,255 }, { 0,0,0,255 });

	if (m_Font != nullptr)
	{
		m_AddColliderButton.SetFont(m_Font);
		m_ColliderWindow->SetFont(m_Font);
	}

	m_Buttons.push_back(&m_AddColliderButton);
}
void MapEditor::CollidorWindowCallback()
{
	bool found = false;

	//If the add frame window is not within the 
	for (unsigned int i = 0; i < m_EditorWindows.size(); ++i)
	{
		if (m_EditorWindows[i] == m_ColliderWindow)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		//set can delete to false;
		m_ColliderWindow->m_ReadyForDelete = false;
		m_ColliderWindow->MouseMove((unsigned int)m_MousePos.x, (unsigned int)m_MousePos.y);
		m_EditorWindows.push_back(m_ColliderWindow);
	}
}
void MapEditor::GetDefaultAssets()
{
	if (m_ResourceManager == nullptr)
	{
		throw std::exception("Can't get default assets if there is no resource manager defined \n");
	}
	m_IconsTexture = m_ResourceManager->LoadTexture("Assets//editor//sprite editor icons.png");
	m_Font = m_ResourceManager->LoadFont("Assets//Fonts//LucidaBrightRegular.ttf", 16);
}
