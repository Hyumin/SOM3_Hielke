#include "FloatingText.h"

FloatingText::FloatingText()
{
	m_Texture = NULL;
	m_Colour.a = 0xff;
	m_Colour.b = 0xaa;
	m_Colour.g = 0xaa;
	m_Colour.r = 0xaa;
	m_Text = "Not set text lol! ";
	m_FontPointer = NULL;
	m_timer = 0.0f;
	m_Duration = 5.0f;
	m_Speed = 0.2f;
	m_MarkedForRemoval = false;
}

FloatingText::FloatingText(float _dur,float _speed, std::string _text)
{
	m_Texture = NULL;
	m_Colour.a = 0xff;
	m_Colour.b = 0xaa;
	m_Colour.g = 0xaa;
	m_Colour.r = 0xaa;
	m_Text = _text;
	m_FontPointer = NULL;
	m_Duration =_dur;
	m_Speed = _speed;
	m_MarkedForRemoval = false;

}

FloatingText::~FloatingText()
{
}

void FloatingText::Update(float _dt)
{
	m_pos.y -= m_Speed * _dt;

	m_timer += _dt;
	if (m_timer > m_Duration)
	{
		//Trigger death 
		m_MarkedForRemoval = true;
	}

}

void FloatingText::Render(SDLRenderer* _renderer, Vector2 _worldpos)
{
	UpdateInterface(_renderer);
	m_Interface.destRect.x = m_pos.x - _worldpos.x;
	m_Interface.destRect.y = m_pos.y - _worldpos.y;
	_renderer->AddToRenderqueue(m_Interface);
}

void FloatingText::SetText(const std::string& _string)
{
	m_Text = _string;
}

void FloatingText::SetText(const char* _text)
{
	m_Text = std::string(_text);
}

void FloatingText::SetFont(TTF_Font* _font)
{
	m_FontPointer = _font;
}

void FloatingText::SetColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a)
{
	m_Colour.r = _r;
	m_Colour.g = _g;
	m_Colour.b = _b;
	m_Colour.a = _a;
}

void FloatingText::SetColour(const SDL_Color& _color)
{
	m_Colour = _color;
}
