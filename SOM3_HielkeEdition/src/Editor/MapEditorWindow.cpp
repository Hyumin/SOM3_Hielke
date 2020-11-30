#include "MapEditorWindow.h"

MapEditorWindow::MapEditorWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	m_Map = nullptr;
	Init(_IconsTexture);
}

MapEditorWindow::MapEditorWindow()
{
	m_Zoom = 0.0f;
	m_Map = nullptr;
}

MapEditorWindow::~MapEditorWindow()
{
	EditorWindow::~EditorWindow();
	m_Map = nullptr;
}

void MapEditorWindow::Update(float _dt)
{
	EditorWindow::Update(_dt);
}

void MapEditorWindow::MouseDown(unsigned int _key)
{
	EditorWindow::MouseDown(_key);
}

void MapEditorWindow::MouseUp(unsigned int _key)
{
	EditorWindow::MouseUp(_key);
}

void MapEditorWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x, _y);
}

void MapEditorWindow::SetFont(TTF_Font* _font)
{
	EditorWindow::SetFont(_font);
}

void MapEditorWindow::SetMap(Hielke::Map* _map)
{
	m_Map = _map;
}

void MapEditorWindow::Render(SDLRenderer* _renderer)
{
	EditorWindow::Render(_renderer);
}

void MapEditorWindow::PassWorldPosAndZoom(Vector2 _pos, float _zoom)
{
	m_WorldPos = _pos;
	m_Zoom = _zoom;
}

void MapEditorWindow::Init(Texture* _IconsTexture)
{
	EditorWindow::Init(_IconsTexture);
}

void MapEditorWindow::ReScaleContent()
{
	EditorWindow::ReScaleContent();
}

void MapEditorWindow::Reposition()
{
	EditorWindow::Reposition();
}
