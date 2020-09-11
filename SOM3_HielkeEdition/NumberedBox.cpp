#include "NumberedBox.h"

NumberedBox::NumberedBox()
{
	Init();
}

NumberedBox::~NumberedBox()
{

}


void NumberedBox::Init()
{
	m_Box = BoxCollider();
	m_Colour = { 255,255,255,255 };
	m_Text = TextField();
}

void NumberedBox::UpdateTextField()
{
	//Puts the textfield in the appropiate corner
	//For starters its gonna be in the top left corner
	m_Text.m_pos = m_Box.pos;
}


void NumberedBox::Render(SDLRenderer* _renderer, Vector2 _WorldPos,bool _drawFilled)
{
	if (_drawFilled)
	{
		_renderer->DrawFilledBox(m_Box.pos.x, m_Box.pos.y, m_Box.w, m_Box.h, m_Colour, _WorldPos, 1);
	}
	else
	{
		_renderer->DrawBox(m_Box, m_Colour, _WorldPos, 1);
	}
	
	if (m_Text.GetFont() != nullptr)
	{
		m_Text.Render(_renderer, _WorldPos, 2);
	}

}

void NumberedBox::SetBox(BoxCollider _box)
{
	UpdateTextField();
}

void NumberedBox::SetPos(Vector2 _pos)
{
	UpdateTextField();
}

void NumberedBox::SetColour(SDL_Color _col)
{
	m_Colour = _col;
	m_Text.SetColour(_col);
}

void NumberedBox::SetFont(TTF_Font* _font)
{
	m_Text.SetFont(_font);
}
