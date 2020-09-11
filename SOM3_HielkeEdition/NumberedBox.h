#pragma once
#include "TextField.h"
#include "HielkMath.h"
#include "SDLRenderer.h"

class NumberedBox
{

public:
	NumberedBox();
	~NumberedBox();

	void Render(SDLRenderer* _renderer, Vector2 _WorldPos = { 0,0 },bool _drawFilled = false);

	void SetBox(BoxCollider _box);
	void SetPos(Vector2 _pos);
	void SetColour(SDL_Color _col);
	void SetFont(TTF_Font* _font);

private:
	
	void UpdateTextField();
	void Init();

	BoxCollider m_Box;
	SDL_Color m_Colour;
	TextField m_Text;

};

