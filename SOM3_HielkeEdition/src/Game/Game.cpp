#include "Game.h"
#include <math.h>

using namespace Hielke;

Game::Game()
{
	m_ResMan = NULL;
	m_PlayerCharacter = NULL;
	Init();
}
Game::Game(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
	m_PlayerCharacter = NULL;
	Init();
}

Game::~Game()
{
	m_ResMan = NULL;
	delete m_PlayerCharacter;
	m_PlayerCharacter = NULL;
	delete m_DuranUI;
	m_DuranUI = NULL;

}

void Game::Init()
{
	//init the player render interface
	m_Duran.srcRect = { 203,155,35,35 };
	m_Duran.renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
	
	m_WindowSize = { 0,0 };
	m_WorldPos = { 0,0 };

	//TODO convert all the animations into a single character file
	if (m_ResMan != nullptr)
	{
		m_Duran.texture = m_ResMan->LoadTexture("Assets\\SpriteSheets\\Duran\\seikendensetsu3_duran_sheet.png");

		AnimationClip nAnim, hAnim, sAnim;

		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\idle_combat_horizontal_duran.hanimclip", m_ResMan);
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\idle_combat_up_duran.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\idle_combat_down_duran.hanimclip",m_ResMan);

	
		m_PlayerCharacter = new Hielke::PlayerCharacter();

		Object* duran = new Object(Vector2(500,700), Vector2(75, 75), m_Duran);

		m_PlayerCharacter->SetObject(duran);

		m_PlayerCharacter->SetIdleCombatAnim(nAnim, sAnim, hAnim, hAnim);

		hAnim = AnimationClip();
		nAnim = AnimationClip();
		sAnim = AnimationClip();
		//Move
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\move_duran_up.hanimclip",m_ResMan);
		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\move_duran_horizontal.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\move_duran_down.hanimclip", m_ResMan);

		m_PlayerCharacter->SetMoveAnim(nAnim, sAnim, hAnim, hAnim);

		hAnim = AnimationClip();
		nAnim = AnimationClip();
		sAnim = AnimationClip();
		//MoveCombat
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\move_combat_up_duran.hanimclip", m_ResMan);
		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\move_combat_horizontal_duran.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\move_combat_down_duran.hanimclip", m_ResMan);
		
		m_PlayerCharacter->SetMoveCombatAnim(nAnim, sAnim, hAnim, hAnim);

		hAnim = AnimationClip();
		nAnim = AnimationClip();
		sAnim = AnimationClip();
		//Idle
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\ilde_duran_up.hanimclip", m_ResMan);
		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\ilde_duran_horizontal.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\ilde_duran_down.hanimclip", m_ResMan);

		m_PlayerCharacter->SetIdleAnim(nAnim, sAnim, hAnim, hAnim);

		hAnim = AnimationClip();
		nAnim = AnimationClip();
		sAnim = AnimationClip();
		//Sprinting
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\move_duran_up_sprint.hanimclip", m_ResMan);
		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\move_duran_horizontal_sprint.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\move_duran_down_sprint.hanimclip", m_ResMan);

		m_PlayerCharacter->SetMoveSprintAnim(nAnim, sAnim, hAnim, hAnim);
		
		hAnim = AnimationClip();
		nAnim = AnimationClip();
		sAnim = AnimationClip();

		//Attack
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\attack_up_duran.hanimclip", m_ResMan);
		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\attack_horizontal_duran.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\attack_down_duran.hanimclip", m_ResMan);

		m_PlayerCharacter->AddAttackAnim(nAnim, sAnim, hAnim, hAnim);

		hAnim = AnimationClip();
		nAnim = AnimationClip();
		sAnim = AnimationClip();

		//EnterCombat
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\enterCombat_duran_up.hanimclip", m_ResMan);
		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\enterCombat_duran_horizontal.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\enterCombat_duran_down.hanimclip", m_ResMan);
		
		m_PlayerCharacter->SetEnterCombatAnim(nAnim, sAnim, hAnim, hAnim);
		
		hAnim = AnimationClip();
		nAnim = AnimationClip();
		sAnim = AnimationClip();
		//Attack2
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\attack_up2_duran.hanimclip", m_ResMan);
		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\attack_horizontal2_duran.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\attack_down2_duran.hanimclip", m_ResMan);

		m_PlayerCharacter->AddAttackAnim(nAnim, sAnim, hAnim, hAnim);
		
		hAnim = AnimationClip();
		nAnim = AnimationClip();
		sAnim = AnimationClip();
		//idle exhausted
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\idle_exhausted_combat_up_duran.hanimclip", m_ResMan);
		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\idle_exhausted_combat_horizontal_duran.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\idle_exhausted_combat_down_duran.hanimclip", m_ResMan);

		m_PlayerCharacter->SetIdleCombatExhaustedAnim(nAnim, sAnim, hAnim, hAnim);

		hAnim = AnimationClip();
		nAnim = AnimationClip();
		sAnim = AnimationClip();
		//special
		nAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\classstrike_up_duran.hanimclip", m_ResMan);
		hAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\classstrike_horizontal_duran.hanimclip", m_ResMan);
		sAnim.LoadClipFromFile("Assets\\AnimationClips\\Duran\\classstrike_down_duran.hanimclip", m_ResMan);

		m_PlayerCharacter->SetClassStrikeAnim(nAnim, sAnim, hAnim, hAnim);
		//Pretty hardcoded way of getting the playericon for the ui
		Object playerIcon = Object(Vector2(0, 0), Vector2(40, 40));
		playerIcon.m_RenderInterface.srcRect = { 9,307,16,16 };
		playerIcon.m_RenderInterface.texture = m_ResMan->LoadTexture("Assets\\SpriteSheets\\Duran\\seikendensetsu3_duran_sheet.png");
		
		m_DuranUI = new PlayerUI(m_PlayerCharacter);
		m_DuranUI->SetFont(m_ResMan->GetFont(("Assets//Fonts//LucidaBrightRegular.ttf")));

		m_DuranUI->SetObject(playerIcon);
		m_DuranUI->m_Pos = Vector2(500, 600);
	}
	m_CurrentMap = m_ResMan->LoadMap("Assets\\Maps\\Free_City_of_maia.hmap");
	m_CurrentMap->SetDefaultFont(m_ResMan->GetFont(("Assets//Fonts//arial.ttf")));
	m_PlayerCharacter->SetMap(m_CurrentMap);


	

}



void Game::SetResourceManager(ResourceManager* _resMan)
{
	m_ResMan = _resMan;
}

void Game::Update(float _dt)
{
	m_DuranUI->Update(_dt);
	for (uint32_t i = 0; i < m_FloatingTexts.size(); ++i)
	{
		m_FloatingTexts[i].Update(_dt);
		if (m_FloatingTexts[i].m_MarkedForRemoval)
		{
			m_FloatingTexts.erase(m_FloatingTexts.begin() + i);
			--i;
		}
	}
	m_PlayerCharacter->Update(_dt);
	if (m_CurrentMap != nullptr)
	{
		m_CurrentMap->Update(_dt);
	}

	//Get worldpos by taking character pos minus windowsize
	m_WorldPos = m_PlayerCharacter->m_Pos- Vector2(m_WindowSize.x/2, m_WindowSize.y/2);

	//Limit world pos to be within window borders 
	if (m_WorldPos.x < 0)
	{
		m_WorldPos.x = 0;
	}
	else if (m_WorldPos.x > m_CurrentMap->GetBackground()->m_Size.x- m_WindowSize.x)
	{
		m_WorldPos.x = m_CurrentMap->GetBackground()->m_Size.x- m_WindowSize.x;
	}
	if (m_WorldPos.y < 0)
	{
		m_WorldPos.y = 0;
	}
	else if (m_WorldPos.y > m_CurrentMap->GetBackground()->m_Size.y- m_WindowSize.y)
	{
		m_WorldPos.y = m_CurrentMap->GetBackground()->m_Size.y- m_WindowSize.y;
	}

	b = m_PlayerCharacter->m_Collider;
	ConnectedMap& conMap = m_CurrentMap->CheckPlayerCollisionWithConnectedMap(b);
	//If its not our expected nothing happened string
	if (conMap.mapName != "None")
	{
		//load a new map
		m_CurrentMap = m_ResMan->LoadMap(conMap.fileName);
		m_CurrentMap->SetDefaultFont(m_ResMan->GetFont(("Assets//Fonts//arial.ttf")));
		m_PlayerCharacter->m_Pos = conMap.startPos;
		m_PlayerCharacter->SetMap(m_CurrentMap);
		return;
	}
}

void Game::KeyDown(unsigned int _key)
{
	m_PlayerCharacter->KeyDown(_key);
}

void Game::KeyUp(unsigned int _key)
{
	m_PlayerCharacter->KeyUp(_key);
}

void Game::Render(SDLRenderer* _renderer)
{
	m_CurrentMap->Render(_renderer, m_WorldPos);
	m_PlayerCharacter->Render(_renderer, m_WorldPos);

	for (uint32_t i = 0; i < m_FloatingTexts.size(); ++i)
	{
		m_FloatingTexts[i].Render(_renderer, m_WorldPos,0);
	}

	std::vector<Box>& mapColliders = m_CurrentMap->GetColliders();
	SDL_Color col = { 0x00,0x00,0xff,0xff };
	for (uint32_t i = 0; i < mapColliders.size(); ++i)
	{
		SDL_Color col2 = { 0xff,0x00,0xff,0xff };
		if (b.BoxCollision(mapColliders[i], b))
		{

			col2.r = 0xff;
			col2.g = 0xff;
			col2.b = 0xff;
		}
		_renderer->DrawBox(mapColliders[i], col2, m_WorldPos);
	}
	m_DuranUI->Render(_renderer);
	_renderer->DrawBox(b, { 0x00,0xff,0x00,0xff },m_WorldPos);
	m_WindowSize.x = (float)_renderer->GetWindowWidth();
	m_WindowSize.y = (float)_renderer->GetWindowHeight();
}
