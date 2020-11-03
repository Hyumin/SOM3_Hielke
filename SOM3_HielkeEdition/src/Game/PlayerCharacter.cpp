#include "PlayerCharacter.h"
#include "..\Engine\AnimationClip.h"
#include "..\Engine\Object.h"
#include "..\Engine\SDLRenderer.h"
#include "..\Engine\Map.h"
#include "Enemy.h"

using namespace Hielke;

Hielke::PlayerCharacter::PlayerCharacter()
{
	m_Velocity = { 0,0 };
	m_Pos = { 0,0 };
	m_State = IDLE;
	m_Dir = EAST;
	m_Object =  NULL;
	m_InCombat = false;
	m_Up = false;
	m_Down = false;
	m_Left = false;
	m_Right = false;
	m_Attack = false;
	UnBlockAllDirections();
	m_AttackCollider.pos = Vector2{ 0,0 };
	m_AttackCollider.w = 80;
	m_AttackCollider.h = 80;

	m_Stats.damage = 10;
	m_Stats.moveSpeed = 100.0f;
	m_Stats.maxHealth = 100;
	m_Stats.health = 100;
	m_Stats.sprintSpeed = 300.0f;
	m_Stats.mana = 9;
	m_Stats.maxMana = 9;
	m_Stats.Level = 1;
	m_Stats.classStrikes = 1;
	m_Stats.classStrikesMax = 4;
	m_Stats.attackCooldown = 0.9f;

	m_InitialSize.x = 30;
	m_InitialSize.y = 30;

	m_CurrentAnimationClip = nullptr;
	m_Collider = Box(m_Pos, m_InitialSize.x, m_InitialSize.y);
	m_CurrentMap = nullptr;
	m_DamageThisAttack = false;

}

Hielke::PlayerCharacter::~PlayerCharacter()
{
	delete m_Object;
	m_Object = NULL;
}

void Hielke::PlayerCharacter::SetSize(float _w, float _h)
{
	m_Object->m_Size.x = _w;
	m_Object->m_Size.y = _h;

	m_InitialSize.x = _w;
	m_InitialSize.y = _h;

	//Change it whenever an object has been initialized
	m_BaseSrcSize = { 30,30 };

	//change collider size
	m_Collider = Box(m_Pos, m_InitialSize.x,m_InitialSize.y);

}

void Hielke::PlayerCharacter::Update(float _dt)
{
	//Reset velocity
	//m_Velocity = { 0,0 };

	UpdateAnimation(_dt);
	if (m_CurrentMap != nullptr)
	{
		//Check collision with map coliders
		std::vector<Box>& mapColliders = m_CurrentMap->GetColliders();
		for (uint32_t i = 0; i < mapColliders.size(); ++i)
		{
			Box& a = mapColliders[i];
			if (a.BoxCollision(a, m_Collider))
			{
				//Figure out which velocities to block
				Vector2 aCenter = a.pos;
				Vector2 bCenter = m_Collider.pos;

				aCenter.x += a.w / 2;
				aCenter.y += a.h / 2;

				//Apply displacement with inversed velocity vector
				Vector2 inverseVel = m_Velocity;
				inverseVel *= -2;
				MovePlayer(inverseVel, _dt);
				m_Velocity = { 0,0 };
			}
			else
			{
				UnBlockAllDirections();
			}
		}
	}

	if (m_State != ENTER_COMBAT)
	{
		if (m_Special)
		{
			ClassStrike(_dt);
		}
		if (m_Attack&& !m_IsCoolingdown &&m_State!=CLASS_STRIKE)
		{
			m_State = ATTACKING;
			Attack(_dt);
		}
		

		HandleMovementInput(_dt);
	}
	else
	{
		//if state is enter combat wait till its finished 
		if (m_CurrentAnimationClip->m_IsFinished)
		{
			m_State = IDLE_COMBAT;
		}
	}
	if (m_IsCoolingdown&& m_State!=CLASS_STRIKE)
	{
		m_AttackCooldDownTimer += _dt;
		if (m_AttackCooldDownTimer >= m_Stats.attackCooldown)
		{
			m_AttackCooldDownTimer -= m_Stats.attackCooldown;
			m_IsCoolingdown = false;
			m_State = ENTER_COMBAT;
		}
	}
	m_Collider.pos.x = m_Pos.x - m_InitialSize.x / 2;
	m_Collider.pos.y = m_Pos.y - m_InitialSize.y / 2;

	
}

void Hielke::PlayerCharacter::UpdateAnimation(float _dt)
{
	switch (m_State)
	{

	case CLASS_STRIKE:
		
		if (m_CurrentAnimationClip != &m_ClassStrikeAnim[m_Dir])
		{
			m_CurrentAnimationClip = &m_ClassStrikeAnim[m_Dir];
			m_CurrentAnimationClip->Play();

			m_AttackCollider.pos = m_Pos - (m_InitialSize / 2);
			switch (m_Dir)
			{
			case NORTH:
				m_AttackCollider.pos.y -= m_AttackCollider.h;
				break;
			case WEST:
				m_AttackCollider.pos.x += m_InitialSize.x;
				break;
			case EAST:
				m_AttackCollider.pos.x -= m_AttackCollider.w;
				break;
			case SOUTH:
				m_AttackCollider.pos.y += m_InitialSize.y;
				break;
			}
		}
		else
		{
			if (m_CurrentAnimationClip->m_CurrentIndex == 10)
			{
				ApplyClassStrikeDamage();
			}
			if (m_CurrentAnimationClip->m_IsFinished)
			{
				m_State = IDLE;
			}
		}
		break;

	case ATTACKING:
		//if its not the same clipname, we can assume the animation should start now 
		if (m_CurrentAnimationClip != &m_AttackAnim[m_CurrentAttackIndex][m_Dir])
		{
			m_CurrentAnimationClip = &m_AttackAnim[m_CurrentAttackIndex][m_Dir];
			m_CurrentAnimationClip->Play();

			m_AttackCollider.pos = m_Pos - (m_InitialSize/2);

			switch (m_Dir)
			{
			case NORTH:
				m_AttackCollider.pos.y -= m_AttackCollider.h;
				break;
			case WEST:
				m_AttackCollider.pos.x += m_InitialSize.x;
				break;
			case EAST:
				m_AttackCollider.pos.x -= m_AttackCollider.w;
				break;
			case SOUTH:
				m_AttackCollider.pos.y += m_InitialSize.y;
				break;
			}

		}
		else if (m_CurrentAnimationClip!= nullptr)
		{
			if (m_CurrentAnimationClip->m_IsFinished)
			{
				m_State = IDLE_COMBAT;
				m_DamageThisAttack = false;
				m_CurrentAttackIndex++;
				if (m_CurrentAttackIndex >= m_AttackAnim.size())
				{
					m_CurrentAttackIndex = 0;
				}
				//initiate cooldown
				m_IsCoolingdown = true;
			}
		}
		break;
	case IDLE:
		//if its not the same clipname, we can assume the animation should start now 
		if (m_CurrentAnimationClip != &m_IdleAnim[m_Dir])
		{
			m_CurrentAnimationClip = &m_IdleAnim[m_Dir];
			m_CurrentAnimationClip->Play();
		}
		break;
	case IDLE_COMBAT:
		//if its not the same clipname, we can assume the animation should start now 
		if (m_IsCoolingdown)
		{
			if (m_CurrentAnimationClip != &m_IdleCombtExhaustedAnim[m_Dir])
			{
				m_CurrentAnimationClip = &m_IdleCombtExhaustedAnim[m_Dir];
				m_CurrentAnimationClip->Play();
			}
		}
		else
		{
			if (m_CurrentAnimationClip != &m_IdleCombtaAnim[m_Dir])
			{
				m_CurrentAnimationClip = &m_IdleCombtaAnim[m_Dir];
				m_CurrentAnimationClip->Play();
			}
		}
		break;
	case MOVING_COMBAT:
		//if its not the same clipname, we can assume the animation should start now 
		if (m_CurrentAnimationClip!= &m_MoveCombtaAnim[m_Dir])
		{
			m_CurrentAnimationClip = &m_MoveCombtaAnim[m_Dir];
			m_CurrentAnimationClip->Play();
		}
		break;
	case MOVING:
		//if its not the same clipname, we can assume the animation should start now 
		if (m_CurrentAnimationClip != &m_MoveAnim[m_Dir])
		{
			m_CurrentAnimationClip = &m_MoveAnim[m_Dir];
			m_CurrentAnimationClip->Play();
		}
		break;
	case SPRINTING:
		if (m_CurrentAnimationClip != &m_MoveSprintAnim[m_Dir])
		{
			m_CurrentAnimationClip = &m_MoveSprintAnim[m_Dir];
			m_CurrentAnimationClip->Play();
		}
		break;
	case ENTER_COMBAT:
		if (m_CurrentAnimationClip != &m_EnterCombatAnim[m_Dir])
		{
			m_CurrentAnimationClip = &m_EnterCombatAnim[m_Dir];
			m_CurrentAnimationClip->Play();
		}
		break;

	default:
		break;
	}

	if (m_CurrentAnimationClip != nullptr)
	{
		m_CurrentAnimationClip->Update(_dt);
		m_Object->m_RenderInterface.srcRect = m_CurrentAnimationClip->GetRect();
	}
}



void Hielke::PlayerCharacter::KeyDown(unsigned int _keycode)
{
	switch (_keycode)
	{
	case SDLK_w:
		m_Up = true;
		break;
	case SDLK_a:
		m_Left = true;
		break;
	case SDLK_d:
		m_Right = true;
		break;
	case SDLK_s:
		m_Down = true;
		break;
	case SDLK_k:
		m_Attack = true;
		break;
	case SDLK_LSHIFT:
		m_Sprint = true;
		break;
	case SDLK_l:
		m_Special = true;
		break;
	default:
		break;
	}
}

void Hielke::PlayerCharacter::KeyUp(unsigned int _keycode)
{
	switch (_keycode)
	{
	case SDLK_w:
		m_Up = false;
		break;
	case SDLK_a:
		m_Left = false;
		break;
	case SDLK_d:
		m_Right = false;
		break;
	case SDLK_s:
		m_Down = false;
		break;
	case SDLK_k:
		m_Attack = false;
		break;
	case SDLK_LSHIFT:
		m_Sprint = false;
		break;
	case SDLK_l:
		m_Special = false;
		break;
	default:
		break;
	}
}

void Hielke::PlayerCharacter::SetObject(Object* _obj)
{
	m_Object = _obj;

	m_InitialSize.x = m_Object->m_Size.x;
	m_InitialSize.y = m_Object->m_Size.y;

	m_Collider = Box(m_Pos, m_InitialSize.x, m_InitialSize.y);
	m_Pos = _obj->m_Pos;
	m_BaseSrcSize = { (float)_obj->m_RenderInterface.srcRect.w,(float)_obj->m_RenderInterface.srcRect.h };

}

void Hielke::PlayerCharacter::InitializeObject()
{
}

//Moves the player with a given velocity and delta time
void Hielke::PlayerCharacter::MovePlayer(const Vector2& _velocity, float _dt)
{
	//If moving increment position by velocity
	if (m_State == MOVING || m_State == MOVING_COMBAT)
	{
		m_Pos += _velocity * _dt * m_Stats.moveSpeed;
	}
	//If sprinting move based on sprintspeed
	if (m_State == SPRINTING)
	{
		m_Pos += _velocity * _dt * m_Stats.sprintSpeed;
	}
}

void Hielke::PlayerCharacter::TakeDamage(float _dmg)
{
	if (!m_InCombat)
	{
		m_State = ENTER_COMBAT;
	}
	m_Stats.health -= _dmg;
	m_InCombat = true;
	
}

void Hielke::PlayerCharacter::SetCombatState(bool _newState)
{
	m_InCombat = _newState;
	if (_newState)
	{
		m_State = ENTER_COMBAT;
	}
	else
	{
		m_Stats.classStrikes = 0;
	}
}

void Hielke::PlayerCharacter::Attack(float _dt)
{
	if (m_CurrentMap != nullptr)
	{
		if (!m_DamageThisAttack)
		{
			HitResult result = m_CurrentMap->CheckMapCollision(m_AttackCollider);
			for (uint32_t i = 0; i < result.enemies.size(); ++i)
			{
				result.enemies[i]->HitReaction(m_Stats);
				m_CurrentMap->AddFloatingText(result.enemies[i]->m_Pos, { 255,255,255,255 }, 3, 100, std::to_string(m_Stats.damage));
				m_DamageThisAttack = true;
				if (m_Stats.classStrikesMax > m_Stats.classStrikes)
				{
					m_Stats.classStrikes++;
				}
			}
		}

	}
}

void Hielke::PlayerCharacter::ClassStrike(float _dt)
{

	if (m_Stats.classStrikes >= 4)
	{
		m_State = CLASS_STRIKE;
		m_Stats.classStrikes = 0;
		m_DamageThisAttack = false;
		UpdateAnimation(_dt);
	}
	else
	{
		return;
	}
}

Box& Hielke::PlayerCharacter::GetAttackCollider()
{
	// TODO: insert return statement here
	if (m_Attack)
	{
		return m_AttackCollider;
	}
	//Return empty boxcollider if we're not attackning
	Box b = Box{};

	return b;

}

void Hielke::PlayerCharacter::SetMap(Map* _map)
{
	m_CurrentMap = _map;
	m_CurrentMap->AssignPlayer(this);
}

void Hielke::PlayerCharacter::HandleMovementInput(float _dt)
{
	if (m_Up)
	{
		if (!m_BlockedDir[NORTH])
		{
			if (m_Velocity.y > -1)
			{
				m_Velocity.y -= 1;
			}
		}
	}

	if (m_Down)
	{
		if (!m_BlockedDir[SOUTH])
		{
			if (m_Velocity.y < 1)
			{
				m_Velocity.y += 1;
			}
		}
	}
	if (m_Left)
	{
		if (!m_BlockedDir[WEST])
		{
			if (m_Velocity.x > -1)
			{
				m_Velocity.x -= 1;
			}
		}
	}

	if (m_Right)
	{
		if (!m_BlockedDir[EAST])
		{
			if (m_Velocity.x < 1)
			{
				m_Velocity.x += 1;
			}
		}
	}
	if (!m_Up && !m_Down)
	{
		m_Velocity.y = 0;
	}
	if (!m_Right && !m_Left)
	{
		m_Velocity.x = 0;
	}
	//If we're not in attacking state we can be idle or move
	if (m_State != ATTACKING&&m_State!= CLASS_STRIKE)
	{
		if (m_Velocity.Magnitude() > 0)
		{
			if (m_InCombat)
			{
				m_State = MOVING_COMBAT;
			}
			else
			{
				if (m_Sprint)
				{
					m_State = SPRINTING;
				}
				else
				{
					m_State = MOVING;
				}

			}
		}
		else
		{
			if (m_InCombat)
			{
				m_State = IDLE_COMBAT;
			}
			else
			{
				m_State = IDLE;
			}
		}
	}
	//Decide upon animation direction
	if (abs(m_Velocity.y) > 0)
	{
		//Up
		if (m_Velocity.y < 0)
		{
			m_Dir = NORTH;
		}
		//Down
		if (m_Velocity.y > 0)
		{
			m_Dir = SOUTH;
		}
	}
	if (abs(m_Velocity.x) > 0)
	{
		//Right
		if (m_Velocity.x > 0)
		{
			m_Dir = WEST;
		}
		//Left
		if (m_Velocity.x < 0)
		{
			m_Dir = EAST;
		}
	}

	MovePlayer(m_Velocity, _dt);
}

void Hielke::PlayerCharacter::AddAttackAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west)
{

	std::map<Direction, AnimationClip> newMap;
	newMap[NORTH] = _north;
	newMap[SOUTH] = _south;
	newMap[EAST] = _east;
	newMap[WEST] = _west;
	m_AttackAnim.push_back(newMap);
}

void Hielke::PlayerCharacter::SetMoveAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west)
{
	m_MoveAnim[NORTH] = _north;
	m_MoveAnim[SOUTH] = _south;
	m_MoveAnim[EAST] = _east;
	m_MoveAnim[WEST] = _west;
}

void Hielke::PlayerCharacter::SetIdleAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west)
{
	m_IdleAnim[NORTH] = _north;
	m_IdleAnim[SOUTH] = _south;
	m_IdleAnim[EAST] = _east;
	m_IdleAnim[WEST] = _west;
}

void Hielke::PlayerCharacter::SetMoveCombatAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west)
{
	m_MoveCombtaAnim[NORTH] = _north;
	m_MoveCombtaAnim[SOUTH] = _south;
	m_MoveCombtaAnim[EAST] = _east;
	m_MoveCombtaAnim[WEST] = _west;
}

void Hielke::PlayerCharacter::SetIdleCombatAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west)
{
	m_IdleCombtaAnim[NORTH] = _north;
	m_IdleCombtaAnim[SOUTH] = _south;
	m_IdleCombtaAnim[EAST] = _east;
	m_IdleCombtaAnim[WEST] = _west;
}

void Hielke::PlayerCharacter::SetIdleCombatExhaustedAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west)
{
	m_IdleCombtExhaustedAnim[NORTH] = _north;
	m_IdleCombtExhaustedAnim[SOUTH] = _south;
	m_IdleCombtExhaustedAnim[EAST] = _east;
	m_IdleCombtExhaustedAnim[WEST] = _west;
}

void Hielke::PlayerCharacter::SetMoveSprintAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west)
{
	m_MoveSprintAnim[NORTH] = _north;
	m_MoveSprintAnim[SOUTH] = _south;
	m_MoveSprintAnim[EAST] = _east;
	m_MoveSprintAnim[WEST] = _west;
}

void Hielke::PlayerCharacter::SetEnterCombatAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west)
{
	m_EnterCombatAnim[NORTH] = _north;
	m_EnterCombatAnim[SOUTH] = _south;
	m_EnterCombatAnim[EAST] = _east;
	m_EnterCombatAnim[WEST] = _west;
}

void Hielke::PlayerCharacter::SetClassStrikeAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west)
{
	m_ClassStrikeAnim[NORTH] = _north;
	m_ClassStrikeAnim[SOUTH] = _south;
	m_ClassStrikeAnim[EAST] = _east;
	m_ClassStrikeAnim[WEST] = _west;
}

void Hielke::PlayerCharacter::BlockDirection(Direction _dir)
{
	m_BlockedDir[_dir] = true;
}

void Hielke::PlayerCharacter::UnBlockDirection(Direction _dir)
{
	m_BlockedDir[_dir] = false;
}

void Hielke::PlayerCharacter::UnBlockAllDirections()
{
	m_BlockedDir[NORTH] = false;
	m_BlockedDir[SOUTH] = false;
	m_BlockedDir[EAST] = false;
	m_BlockedDir[WEST] = false;
}

void Hielke::PlayerCharacter::Render(SDLRenderer* _render, Vector2 _worldPos)
{
	switch (m_Dir)
	{

	case WEST:
		m_Object->m_RenderInterface.renderFlip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		break;

	default:
		m_Object->m_RenderInterface.renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
		break;
	}


	//Calculate aspect ratio
	m_Aspectratio.x = (float)m_Object->m_RenderInterface.srcRect.w/m_BaseSrcSize.x;
	m_Aspectratio.y = (float)m_Object->m_RenderInterface.srcRect.h/ m_BaseSrcSize.y;

	m_Object->m_Size = { m_InitialSize.x*m_Aspectratio.x ,m_InitialSize.y*m_Aspectratio.y };
	
	Vector2 offset = { 0,0 };
	try
	{
		offset += m_CurrentAnimationClip->GetOffset();
	}
	catch (std::exception& e)
	{

	}

	if (m_Object->m_RenderInterface.renderFlip == SDL_RendererFlip::SDL_FLIP_HORIZONTAL)
	{
		offset.x *= -1;
	}
	offset.x -= m_InitialSize.x/2;
	offset.y -= m_InitialSize.y/2;

	m_Object->m_Pos = m_Pos+offset;
	if (m_Object != NULL)
	{
		m_Object->Render(_render, _worldPos);
		_render->DrawBox(m_Collider, { 0,255,0,255 }, _worldPos);
		_render->AddLine(m_Pos, m_Pos + m_Velocity*100,_worldPos);
		_render->DrawBox(m_AttackCollider, { 0xff,0xff,0xff,0xff }, _worldPos);
	}
}

void Hielke::PlayerCharacter::ApplyClassStrikeDamage()
{
	if (m_CurrentMap != nullptr)
	{
		if (!m_DamageThisAttack)
		{
			HitResult result = m_CurrentMap->CheckMapCollision(m_AttackCollider);
			PlayerStats superStats = m_Stats;

			superStats.damage *= 4;
			for (uint32_t i = 0; i < result.enemies.size(); ++i)
			{
				result.enemies[i]->HitReaction(superStats);
				m_CurrentMap->AddFloatingText(result.enemies[i]->m_Pos, { 255,255,255,255 }, 3, 100, std::to_string(superStats.damage));
				m_DamageThisAttack = true;
			}
		}

	}
}
