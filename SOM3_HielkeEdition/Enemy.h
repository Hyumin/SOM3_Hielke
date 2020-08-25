#pragma once
#include "SDLRenderer.h"
#include "Object.h"
#include "PlayerCharacter.h"
#include "HielkMath.h"

struct EnemyStats
{
	int health;
	int maxHealth;
	int damage;
	float speed;
};
namespace Hielke
{
	class Map;
};


class Enemy
{
public:
	enum State
	{
		IDLE,MOVING,ATTACKING,DYING
	};
	enum Direction
	{
		NORTH,SOUTH,EAST,WEST
	};


	Enemy();
	Enemy(Object& obj);
	virtual ~Enemy();

	virtual void SetPlayerTarget(Hielke::PlayerCharacter* _Player)=0;
	virtual void Update(float _dt) = 0;
	virtual void Render(SDLRenderer* _renderer,Vector2 _worldPos) = 0;
	virtual void HitReaction(Hielke::PlayerStats _stats) = 0;
	virtual void SetMap(Hielke::Map* _map) =0;

	BoxCollider m_Collider;
	EnemyStats m_Stats;
	Object m_Object;
	State m_State;
	Direction m_Direction;
	Vector2 m_Pos;
	bool m_Dead;

protected:

	Hielke::Map* m_Map;
	Hielke::PlayerCharacter* m_PlayerTarget;
	
	Vector2 m_Velocity;

};

