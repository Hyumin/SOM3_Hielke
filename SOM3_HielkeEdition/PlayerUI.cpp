#include "PlayerUI.h"
#include "SDLRenderer.h"

PlayerUI::PlayerUI()
{
	m_Character = nullptr;
	m_Font = nullptr;
	m_PlayerIcon = Object();
	m_PlayerIcon.m_Size = Vector2{ 50,50 };
}

PlayerUI::~PlayerUI()
{
	delete m_Health;
	m_Health = nullptr;
	delete m_Level;
	m_Level = nullptr;
	delete m_Mana;
	m_Mana = nullptr;
}

PlayerUI::PlayerUI(Hielke::PlayerCharacter* _pc)
{
	m_Character = _pc;
	m_Font = nullptr;
	m_PlayerIcon = Object();
	m_PlayerIcon.m_Size = Vector2{50,50};
}

void PlayerUI::SetPlayer(Hielke::PlayerCharacter* _pc)
{
	m_Character = _pc;

	m_PlayerIcon.m_RenderInterface =_pc->m_Object->m_RenderInterface;
	m_PlayerIcon.m_RenderInterface.textureName = "Assets\\SpriteSheets\\Duran\\seikendensetsu3_duran_sheet.png";
	m_PlayerIcon.m_RenderInterface.srcRect = {9,307,16,16};
}

void PlayerUI::SetObject(Object _obj)
{
	m_PlayerIcon = _obj;
}

void PlayerUI::SetFont(TTF_Font* _font)
{
	m_Font = _font;

	//Remove the already existing textfields
	delete m_Health;
	m_Health = nullptr;
	delete m_Level;
	m_Level = nullptr;
	delete m_Mana;
	m_Mana = nullptr;


	m_Health = new TextField();
	m_Level = new TextField();
	m_Mana = new TextField();

	m_Health->SetFont(m_Font);
	m_Level->SetFont(m_Font);
	m_Mana->SetFont(m_Font);

	m_Health->m_Size = Vector2{150,30 };
	m_Level->m_Size = Vector2{ 75,25 };
	m_Mana->m_Size = Vector2{ 75,25 };

	m_Health->SetColour({ 255,255,255,255 });
	m_Level->SetColour({ 255,255,255,255 });
	m_Mana->SetColour({ 255,255,255,255 });

}

void PlayerUI::Update(float _dt)
{
	//Update textfields position and current text.
	if (m_Character != nullptr)
	{
		m_Health->SetText(std::to_string(m_Character->m_Stats.health) + "/" + std::to_string(m_Character->m_Stats.maxHealth));
		m_Level->m_pos = m_Pos + Vector2{ 0,75 };
		m_Health->m_pos = m_Pos + Vector2{ 30,0 };
		m_Mana->m_pos = m_Pos + Vector2{150,75};
		m_Level->SetText("LV:"+ std::to_string(m_Character->m_Stats.Level));
		m_Mana->SetText("MP:" + std::to_string(m_Character->m_Stats.mana));
		m_PlayerIcon.m_Pos = m_Pos + Vector2{80,35};
	}

}
void PlayerUI::Render(SDLRenderer* _renderer)
{
	if (m_Font != nullptr&&m_Character!=nullptr)
	{
		m_Health->Render(_renderer,Vector2());
		m_Level->Render(_renderer, Vector2());
		m_Mana->Render(_renderer, Vector2());
		m_PlayerIcon.Render(_renderer, Vector2());
		_renderer->DrawBox(m_PlayerIcon.m_Pos.x, m_PlayerIcon.m_Pos.y, m_PlayerIcon.m_Size.x, m_PlayerIcon.m_Size.y);
		
		Vector2 vec = { 125,50 };
		for (int i = 0; i < m_Character->m_Stats.classStrikesMax; ++i)
		{
			Vector2 pos = m_Pos + vec;
			pos.x += i * 10;
			SDL_Color col = { 20,20,20,255 };

			if ( i<m_Character->m_Stats.classStrikes)
			{
				if (m_Character->m_Stats.classStrikes >= 4)
				{
					col.g = 255;
				}
				else
				{
					col.b = 255;
				}
			}

			_renderer->DrawFilledBox(pos.x, pos.y, 5, 15, col);
			pos.x -= i * 10;
			pos.x -= 60;
			pos.x -= i * 10;
			_renderer->DrawFilledBox(pos.x, pos.y, 5, 15, col);
		}
	}
}
