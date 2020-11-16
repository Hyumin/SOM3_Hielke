#include "MapEditor.h"
#include "..\Engine\ButtonBuilder.h"

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
}

void MapEditor::Update(float _dt)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Update(_dt);
	}
}

void MapEditor::Render(SDLRenderer* _renderer)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}
}

void MapEditor::KeyDown(unsigned int _key)
{
}

void MapEditor::KeyUp(unsigned int _key)
{
	
}

void MapEditor::MouseUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
}

void MapEditor::MouseDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
}

void MapEditor::MouseMove(int _x, int _y)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x,_y);
	}
}

void MapEditor::ZoomIn()
{
}

void MapEditor::ZoomOut()
{
}

void MapEditor::SaveMap()
{
	printf(" trying to save map but functionality not implented yet :( mapeditor.cpp \n");
}

void MapEditor::LoadMap()
{
	printf(" trying to load map but functionality not implented yet :( mapeditor.cpp \n");
}

void MapEditor::GiveTopBarBox(Box _topBarBox)
{
	//will put the buttons according to the size of this box

}

void MapEditor::Init()
{
	
}
