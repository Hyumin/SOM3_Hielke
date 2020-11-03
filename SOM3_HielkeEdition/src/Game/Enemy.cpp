#include "Enemy.h"

Enemy::Enemy()
{
	m_Object = Object();
	m_Collider.pos = m_Object.m_Pos;
	m_Collider.w = m_Object.m_Size.x;
	m_Collider.h = m_Object.m_Size.y;
	m_PlayerTarget = nullptr;
	m_Velocity = Vector2(0, 0);

	m_Stats.speed = 150.0f;
	m_Stats.maxHealth = 50;
	m_Stats.health = 50;
	m_Stats.damage = 5;
	m_Pos = Vector2(0, 0);

	m_State = IDLE;
	m_Direction = SOUTH;

	m_Dead = false;
}

Enemy::Enemy(Object& obj)
{
	m_Object = obj;
	m_Collider.pos = m_Object.m_Pos;
	m_Collider.w = m_Object.m_Size.x;
	m_Collider.h = m_Object.m_Size.y;
	m_PlayerTarget = nullptr;
	m_Velocity = Vector2(0, 0);
	m_Pos = obj.m_Pos;

	m_Stats.speed = 50.0f;
	m_Stats.maxHealth = 50;
	m_Stats.health = 50;
	m_Stats.damage = 5;

	m_State = IDLE;
	m_Direction = SOUTH;
	m_Dead = false;
}

Enemy::~Enemy()
{
}

