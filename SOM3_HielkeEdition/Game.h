#pragma once
#include "SDLRenderer.h"
#include "ResourceManager.h"
#include "Object.h"
#include "AnimationClip.h"
#include "PlayerCharacter.h"
#include "TextField.h"
#include "FloatingText.h"
#include "Map.h"
#include "Enemy.h"
#include "Rabite.h"
#include "PlayerUI.h"

struct Vec2
{
	float x, y;
};

class Game
{

public:
	Game();
	Game(ResourceManager* _resMan);
	~Game();


	void Init();

	void SetResourceManager(ResourceManager* _resMan);
	void Update(float _dt);
	void KeyDown(unsigned int _key);
	void KeyUp(unsigned int _key);
	void Render(SDLRenderer* _renderer);
	
private:
	ResourceManager* m_ResMan;
	RenderInterface m_Duran;

	Hielke::PlayerCharacter* m_PlayerCharacter;

	Hielke::Map* m_CurrentMap;
	std::vector<FloatingText> m_FloatingTexts;
	PlayerUI* m_DuranUI; //UI of the player

	bool m_kup, m_kdwn, m_klft, m_krght;
	Vector2 m_WorldPos;
	Vector2 m_WindowSize;
	float m_Speed = 100.0f;
	float m_AnimTimer = 0.0f;
	float m_AnimTimeLimit = 0.3f;

	Box b;

};

