#include "Rabite.h"
#include "AnimationClip.h"
#include "Map.h"

Rabite::Rabite():Enemy()
{
	m_AttackCollider.w = 44;
	m_AttackCollider.h = 44;
	m_AttackCollider.pos = Vector2(0,0);
}

Rabite::Rabite(Object& _obj):Enemy(_obj)
{
	m_AttackCollider.w = 44;
	m_AttackCollider.h = 44;
	m_AttackCollider.pos = Vector2(0, 0);
	
}

Rabite::~Rabite()
{
	m_PlayerTarget = NULL;
	std::cout << "Rabite is being deleted \n";
}

void Rabite::SetPlayerTarget(Hielke::PlayerCharacter* _Player)
{
	m_PlayerTarget = _Player;
}

void Rabite::Update(float _dt)
{
	if (m_State != DYING)
	{
		if (m_PlayerTarget != nullptr)
		{
			float distance = Vector2::Distance(m_Pos, m_PlayerTarget->m_Pos);
			Vector2 stuff = m_PlayerTarget->m_Pos - m_Pos;
			stuff.Normalize();

			if (distance < 65)
			{
				m_Velocity = Vector2(0, 0);
				m_State = ATTACKING;
			}
			else
			{
				m_Velocity.x = stuff.x * m_Stats.speed;
				m_Velocity.y = stuff.y * m_Stats.speed;
			}
		}
		else
		{
			m_State = State::IDLE;
		}

		m_Pos += m_Velocity * _dt;
		m_Collider.pos = m_Object.m_Pos;
		if (m_Velocity.Magnitude() > 1)
		{
			m_State = State::MOVING;
			if (abs(m_Velocity.x) > abs(m_Velocity.y))
			{
				//We're going horizontal
				if (m_Velocity.x > 0)
				{
					m_Direction = EAST;
				}
				else
				{
					m_Direction = WEST;
				}
			}
			else if (m_Velocity.y > 0)
			{
				m_Direction = SOUTH;
			}
			else
			{
				m_Direction = NORTH;
			}
		}
		if (m_State == Enemy::State::ATTACKING)
		{
			Attack();
		}
	}
	else
	{
		if (m_CurrentAnimationClip->m_IsFinished)
		{
			m_Dead = true;
		}
	}
	m_CurrentAnimationClip->Update(_dt);
	AnimationStateMachine();
	
}

void Rabite::Render(SDLRenderer* _renderer, Vector2 _worldPos)
{
	try
	{
		m_Object.m_RenderInterface.srcRect = m_CurrentAnimationClip->GetRect();
	}
	catch (std::exception& e)
	{

	}
	switch (m_Direction)
	{

	case EAST:
		m_Object.m_RenderInterface.renderFlip = (SDL_RendererFlip)1;
		break;
	default:
		m_Object.m_RenderInterface.renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
		break;
	}

	Vector2 offset = { 0,0 };
	try
	{
		offset += m_CurrentAnimationClip->GetOffset();
	}
	catch (std::exception& e)
	{

	}

	if (m_Object.m_RenderInterface.renderFlip == SDL_RendererFlip::SDL_FLIP_HORIZONTAL)
	{
		offset.x *= -1;
	}
	offset.x -= m_Object.m_Size.x / 2;
	offset.y -= m_Object.m_Size.y / 2;

	m_Object.m_Pos = m_Pos + offset;

	m_Object.Render(_renderer, _worldPos);
	_renderer->DrawBox(m_Collider, SDL_Colour{ 255,0,0,255 }, _worldPos);
	_renderer->DrawBox(m_AttackCollider, SDL_Colour{ 255,0,255,255 }, _worldPos);
	_renderer->AddLine(m_Pos, m_Pos + m_Velocity, _worldPos);

}

void Rabite::HitReaction(Hielke::PlayerStats _stats)
{
	m_Stats.health -= _stats.damage;
	if (m_Stats.health < 0)
	{
		m_State = DYING;
		//This should play the death animation
	}
}

void Rabite::AnimationStateMachine()
{
	switch (m_State)
	{
	case IDLE:

		if (m_CurrentAnimationClip->m_ClipName != m_IdleAnimation[m_Direction].m_ClipName)
		{
			m_CurrentAnimationClip = &m_IdleAnimation[m_Direction];
			m_CurrentAnimationClip->Play();
		}
		break;
	case MOVING:
		if (m_CurrentAnimationClip->m_ClipName != m_MoveAnimation[m_Direction].m_ClipName)
		{
			m_CurrentAnimationClip = &m_MoveAnimation[m_Direction];
			m_CurrentAnimationClip->Play();

		}
		break;
	case ATTACKING:
		if (m_CurrentAnimationClip->m_ClipName != m_AttackAnimation[m_Direction].m_ClipName)
		{
			m_CurrentAnimationClip = &m_AttackAnimation[m_Direction];
			m_CurrentAnimationClip->Play();

		}
		break;
	case DYING:
		//Play idle animation for this enemy's death
		if (m_CurrentAnimationClip->m_ClipName != m_IdleAnimation[m_Direction].m_ClipName)
		{
			m_CurrentAnimationClip = &m_IdleAnimation[m_Direction];
			m_CurrentAnimationClip->Play();
		}
	default:
		break;
	}


}

void Rabite::SetMap(Hielke::Map* _map)
{
	m_Map = _map;
	//m_Map->AssignEnemy(this);
}

void Rabite::Attack()
{
	if (m_Map != nullptr)
	{
		switch (m_Direction)
		{
		case NORTH:
			m_AttackCollider.pos = m_Object.m_Pos - Vector2(0,m_AttackCollider.h);
			break;
		case SOUTH:
			m_AttackCollider.pos = m_Object.m_Pos + Vector2(0,m_Object.m_Size.y);
			break;
		case EAST:
			m_AttackCollider.pos = m_Object.m_Pos + Vector2(m_Object.m_Size.x, 0);
			break;
		case WEST:
			m_AttackCollider.pos= m_Object.m_Pos- Vector2(m_AttackCollider.w,0);

			break;
		default:
			break;
		}

		if (m_CurrentAnimationClip->m_IsFinished)
		{
			attackFrame = false;
			m_CurrentAnimationClip->Play();
		}

		if (!attackFrame)
		{
			Hielke::HitResult result = m_Map->CheckMapCollision(m_AttackCollider);

			SDL_Color col = { 255,0,0,255 };

			for (unsigned int i = 0; i < result.players.size(); ++i)
			{
				result.players[i]->TakeDamage(m_Stats.damage);
				m_Map->AddFloatingText(result.players[i]->m_Pos, col, 3.0f, 100.0f, std::to_string(m_Stats.damage));
			}
			attackFrame = true;
		}

	}
}

void Rabite::SetIdleAnimations(AnimationClip _north, AnimationClip _horizontal, AnimationClip _south)
{
	m_IdleAnimation[NORTH] = _north;
	m_IdleAnimation[EAST] = _horizontal;
	m_IdleAnimation[WEST] = _horizontal;
	m_IdleAnimation[SOUTH] = _south;
	m_CurrentAnimationClip = &m_IdleAnimation[EAST];
}

void Rabite::SetMoveAnimations(AnimationClip _north, AnimationClip _horizontal, AnimationClip _south)
{
	m_MoveAnimation[NORTH] = _north;
	m_MoveAnimation[EAST] = _horizontal;
	m_MoveAnimation[WEST] = _horizontal;
	m_MoveAnimation[SOUTH] = _south;
}

void Rabite::SetAttackAnimations(AnimationClip _north, AnimationClip _horizontal, AnimationClip _south)
{
	m_AttackAnimation[NORTH] = _north;
	m_AttackAnimation[EAST] = _horizontal;
	m_AttackAnimation[WEST] = _horizontal;
	m_AttackAnimation[SOUTH] = _south;
}

std::map<Enemy::Direction, AnimationClip> Rabite::GetIdleAnimMap()
{
	return m_IdleAnimation;
}

std::map<Enemy::Direction, AnimationClip> Rabite::GetMoveAnimMap()
{
	return m_MoveAnimation;
}

std::map<Enemy::Direction, AnimationClip> Rabite::GetAttacAnimMap()
{
	return m_AttackAnimation;
}
