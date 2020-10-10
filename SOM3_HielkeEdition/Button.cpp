#include "Button.h"
#include <iostream>

Button::Button()
{
	Init();

}

Button::~Button()
{
}

void Button::Update(float _dt)
{

}

void Button::MouseDown(unsigned int _key)
{
	if (_key == 1)
	{
		if (m_Hovered) // once its hoeverd we know we're already colliding with 
		{
			m_Clicked = true;
		}
	}
}

void Button::MouseMove(int _x, int _y)
{
	m_MousePos.x = (float)_x;
	m_MousePos.y = (float)_y;

	if (m_Collider.BoxCollision(m_Collider, m_MousePos))
	{
		m_Hovered = true;
	}
	else
	{
		m_Hovered = false;
	}

}

void Button::MouseUp(unsigned int _key)
{
	if (_key == 1)
	{
		if (m_Clicked) 
		{
			m_Clicked = false;
			//Fire event
			//std::cout << " A click has happened datebayo \n";
			if (callback != NULL)
			{
				callback();
			}
		}	
	}
}

void Button::Render(SDLRenderer* _renderer)
{
	
	SDL_Colour col = m_ColNorm;
	if (m_Hovered)
	{
		col = m_ColHov;
	}
	if (m_Clicked)
	{
		col = m_ColClicked;
	}

	switch (m_DrawMode)
	{
	case WIREFRAME:
		_renderer->DrawBox(m_Collider, col, { 0,0 },m_Layer);
		//m_TextField.Render(_renderer, { 0,0 }, m_Layer + 1);
		break;
	case FILLEDRECT:
		_renderer->DrawFilledBox(m_Collider, col, { 0,0 }, m_Layer);

		break;
	case TEXTURE:

		break;
	}
}

void Button::SetTextureDrawMode()
{

}

void Button::SetFilledRectMode(SDL_Colour _normal, SDL_Colour _hovered, SDL_Colour _clicked)
{
	m_ColNorm = _normal;
	m_ColHov = _hovered;
	m_ColClicked = _clicked;
	m_DrawMode = FILLEDRECT;
}

void Button::SetWireFrameMode(SDL_Colour _normal, SDL_Colour _hovered, SDL_Colour _clicked)
{
	m_ColNorm = _normal;
	m_ColHov = _hovered;
	m_ColClicked = _clicked;
	m_DrawMode = WIREFRAME;
}

void Button::SetLayer(int _layer)
{
	m_Layer = _layer;
}

void Button::SetSize(Vector2 _size)
{
	m_Size = _size;
	m_Object.m_Size = _size;
	m_Collider.w = _size.x;
	m_Collider.h = _size.y;
}

void Button::SetPosition(Vector2 _pos)
{
	m_Pos = _pos;
	m_Object.m_Pos = m_Pos;
	m_Collider.pos = m_Pos;

}

void Button::SetCallbackFunction(void(*funcpntr)())
{
	callback = funcpntr;
}

void Button::Init()
{
	m_Hovered = false;
	m_Clicked = false;
	m_Layer = 1;
	m_MousePos = { 0,0 };
	m_TexHovered = " ";
	m_TextureName = " ";
	m_Font = nullptr;
	m_Object = Object{};
	m_Pos = Vector2{ 0,0 };
	m_Collider = BoxCollider(0, 0, 100, 100);
	callback = NULL;
	SetFilledRectMode({ 125,125,125,255 }, { 255,255,255,255 }, { 0,0,0,255 });

}
