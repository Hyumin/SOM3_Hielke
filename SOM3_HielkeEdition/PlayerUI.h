#pragma once
#include "PlayerCharacter.h"
#include "TextField.h"
#include "HielkMath.h"
#include "Object.h"

class SDLRenderer;
class PlayerUI
{
public :
	PlayerUI();
	~PlayerUI();
	PlayerUI(Hielke::PlayerCharacter* _pc);
	void SetPlayer(Hielke::PlayerCharacter* _pc);
	void SetObject(Object _obj);
	void SetFont(TTF_Font* _font);
	void Update(float _dt);
	void Render(SDLRenderer* _renderer);

	Vector2 m_Pos;
private:

	Hielke::PlayerCharacter* m_Character;
	TextField* m_Health,* m_Level,* m_Mana;
	Object m_PlayerIcon;
	TTF_Font* m_Font;	
};

