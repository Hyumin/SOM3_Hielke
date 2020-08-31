#include "EditorWindow.h"

EditorWindow::EditorWindow(Vector2 _pos, std::string& _name)
{
	m_Bar = BoxCollider{100, 100, 300, 40};
	m_Cross = BoxCollider{350,100,50,40};
	m_ContentBox = BoxCollider{100,140,300,500};
	m_BarRelativePos = Vector2{0,0};
	m_CrossRelativePos = Vector2{250,0};
	m_ContentRelativePos = Vector2{0,40};

	m_Pos = _pos;
	m_Name = _name;
	m_Dragging = false;
	m_ReadyForDelete = false;
	m_MousePos = Vector2{ 0,0 };
	m_TextField = TextField();
	m_Obj = Object();
}

EditorWindow::EditorWindow()
{
	m_Bar = BoxCollider{ 100, 100, 300, 40 };
	m_Cross = BoxCollider{ 350,100,50,40 };
	m_ContentBox = BoxCollider{ 100,140,300,500 };
	m_Pos = Vector2{ 0,0 };
	m_Name = "empty window";
	m_Dragging = false;
	m_ReadyForDelete = false;
	m_MousePos = Vector2{ 0,0 };
	m_TextField = TextField();
	m_Obj = Object();
}

EditorWindow::~EditorWindow()
{
	
}

void EditorWindow::Update(float _dt)
{
	m_Bar.pos = m_Pos + m_BarRelativePos;
	m_TextField.m_pos = m_Pos + m_BarRelativePos;
	m_Cross.pos = m_Pos + m_CrossRelativePos;
	m_ContentBox.pos = m_Pos + m_ContentRelativePos;
	m_Obj.m_Pos = m_ContentBox.pos;

	m_TextField.Update(_dt);
}

void EditorWindow::MouseDown(unsigned int _key)
{
	if (_key ==(SDL_BUTTON_LEFT))
	{
		if (m_Bar.BoxCollision(m_Bar, m_MousePos))
		{
			//Can drag the mouse
			m_Dragging = true;

		}
		if (m_Cross.BoxCollision(m_Cross, m_MousePos))
		{
			//Close the window
			m_ReadyForDelete = true;
		}
	}
}

void EditorWindow::MouseUp( unsigned int _key)
{
	// if mouse button up draggin should be false
	if (_key == (SDL_BUTTON_LEFT))
	{
		m_Dragging = false;
	}
}

void EditorWindow::MouseMove(unsigned int _x, unsigned int _y)
{

	m_MousePos.x = _x;
	m_MousePos.y = _y;
	//IF we're dragging the box set box pos to be the same as mousepos
	if (m_Dragging)
	{
		m_Pos.x = _x;
		m_Pos.y = _y;
	}
}

void EditorWindow::SetFont(TTF_Font* _font)
{
	m_TextField.SetFont(_font);
}

void EditorWindow::SetShowingObject(Object& _obj)
{
	m_Obj = _obj;
}

void EditorWindow::Render(SDLRenderer* _renderer)
{
	_renderer->DrawBox(m_Bar, { 0,100,255,255 });
	_renderer->DrawFilledBox(m_Cross.pos.x, m_Cross.pos.y, m_Cross.w, m_Cross.h, { 255,0,0,255 });
	_renderer->DrawBox(m_ContentBox);
	m_Obj.Render(_renderer, Vector2{ 0, 0 });
	m_TextField.Render(_renderer, Vector2{ 0,0 });
}
