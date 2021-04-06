#pragma once
#include "..\Engine\SDLRenderer.h"
#include "..\Engine\ResourceManager.h"
#include "..\Engine\Object.h"
#include "..\Engine\AnimationClip.h"
#include "PlayerCharacter.h"
#include "..\Engine\TextField.h"
#include "..\Engine\FloatingText.h"
#include "..\Engine\Map.h"
#include "Enemy.h"
#include "Rabite.h"
#include "PlayerUI.h"
#include "..\Debug\PlayerDebugInfo.h"

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

	void ToggleDebugMode();

	ResourceManager* m_ResMan;
	RenderInterface m_Duran;
	Hielke::PlayerDebugInfo* m_DebugInfo;

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

	bool m_DebugMode = false;// set this to true if you want see things like colliders etc.

	SDL_Keycode m_debug_mode_key = SDLK_1;


};

