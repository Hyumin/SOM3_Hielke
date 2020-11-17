#include "AddMapColiderWindow.h"

AddMapColliderWindow::AddMapColliderWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	m_Map = nullptr;
	Init(_IconsTexture);
}

AddMapColliderWindow::AddMapColliderWindow()
{
}

AddMapColliderWindow::~AddMapColliderWindow()
{
	MapEditorWindow::~MapEditorWindow();
	m_Map = nullptr;

	
}

void AddMapColliderWindow::Update(float _dt)
{
	MapEditorWindow::Update(_dt);
}

void AddMapColliderWindow::MouseDown(unsigned int _key)
{
	MapEditorWindow::MouseDown(_key);
}

void AddMapColliderWindow::MouseUp(unsigned int _key)
{
	MapEditorWindow::MouseUp(_key);
}

void AddMapColliderWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	MapEditorWindow::MouseMove(_x, _y);
}

void AddMapColliderWindow::KeyDown(unsigned int _key)
{
}

void AddMapColliderWindow::KeyUp(unsigned int _key)
{
}

void AddMapColliderWindow::SetFont(TTF_Font* _font)
{
	MapEditorWindow::SetFont(_font);
}

void AddMapColliderWindow::SetMap(Hielke::Map* _map)
{
	m_Map = _map;
}

void AddMapColliderWindow::Render(SDLRenderer* _renderer)
{
	MapEditorWindow::Render(_renderer);
}

void AddMapColliderWindow::Init(Texture* _IconsTexture)
{
	MapEditorWindow::Init(_IconsTexture);
	ReScaleContent();
	Reposition();
}

void AddMapColliderWindow::ReScaleContent()
{
	MapEditorWindow::ReScaleContent();
}

void AddMapColliderWindow::Reposition()
{
	MapEditorWindow::Reposition();
}
