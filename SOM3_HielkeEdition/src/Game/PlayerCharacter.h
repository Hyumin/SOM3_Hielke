#pragma once

#include <map>
#include <vector>
#include <string>
#include "..\HielkMath.h"

class AnimationClip;
class Object;
class SDLRenderer;

namespace Hielke
{
	class Map;
	struct PlayerStats {

		float moveSpeed;
		float sprintSpeed;
		int health;
		int maxHealth;
		int damage;
		int mana;
		int maxMana;
		int Level;
		int classStrikes;
		int classStrikesMax;
		float attackCooldown;

	};
	enum PlayerState
	{
		IDLE, MOVING, IDLE_COMBAT, MOVING_COMBAT, ATTACKING, SPRINTING, ENTER_COMBAT, CLASS_STRIKE
	};

	class PlayerCharacter
	{
	public:

	
		enum Direction
		{
			NORTH, SOUTH, EAST, WEST
		};

		PlayerCharacter();
		~PlayerCharacter();

		void SetSize(float _w, float _h);
		void Update(float _dt);
		void UpdateAnimation(float _dt);
		void KeyDown(unsigned int _keycode);
		void KeyUp(unsigned int _keycode);
		void SetObject(Object* _obj);
		void InitializeObject();
		void MovePlayer(const Vector2& _velocity, float _dt); // moves the player with a given velocity and delta time
		void TakeDamage(float _dmg);
		void SetCombatState(bool _newState);
		void Attack(float _dt);
		void ClassStrike(float _dt);


		Box& GetAttackCollider();

		void SetMap(Map* _map);
		void HandleMovementInput(float _dt);
		void AddAttackAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west);
		void SetMoveAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west);
		void SetIdleAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west);
		void SetMoveCombatAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west);
		void SetIdleCombatAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west);
		void SetIdleCombatExhaustedAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west);
		void SetMoveSprintAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west);
		void SetEnterCombatAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west);
		void SetClassStrikeAnim(AnimationClip& _north, AnimationClip& _south, AnimationClip& _east, AnimationClip& _west);

		void BlockDirection(Direction _dir);
		void UnBlockDirection(Direction _dir);
		void UnBlockAllDirections();



		void Render(SDLRenderer* _render, Vector2 _worldPos);

		Vector2 m_Velocity;
		Vector2 m_InitialSize;
		Vector2 m_BaseSrcSize;
		Vector2 m_Aspectratio;
		Vector2 m_Pos; //Position of the player character and where the boxcollider will start

		PlayerState m_State;
		Direction m_Dir;

		Object* m_Object; //Object is the rendered part of the player character
		std::map<Direction, bool> m_BlockedDir;
		Box m_Collider;
		Box m_AttackCollider;
		PlayerStats m_Stats;
		Map* m_CurrentMap;
		AnimationClip* m_CurrentAnimationClip;

	private:
		void ApplyClassStrikeDamage();

		bool m_Up, m_Down, m_Left, m_Right, m_Attack, m_Sprint,m_Special;//Key bools
		bool m_InCombat;
		bool m_DamageThisAttack;
		int m_CurrentAttackIndex = 0;
		float m_AttackCooldDownTimer = 0.0f;
		bool m_IsCoolingdown = false;


		std::vector<std::map<Direction, AnimationClip>> m_AttackAnim;
		std::map<Direction, AnimationClip> m_MoveAnim;
		std::map<Direction, AnimationClip> m_IdleAnim;
		std::map<Direction, AnimationClip> m_IdleCombtaAnim;
		std::map<Direction, AnimationClip> m_IdleCombtExhaustedAnim;
		std::map<Direction, AnimationClip> m_MoveCombtaAnim;
		std::map<Direction, AnimationClip> m_MoveSprintAnim;
		std::map<Direction, AnimationClip> m_EnterCombatAnim;
		std::map<Direction, AnimationClip> m_ClassStrikeAnim;
	};

	static std::string ReturnPlayerStateString(PlayerState _state)
	{
		switch (_state)
		{
		case IDLE:
			return "IDLE";
			break;
		case MOVING:
			return "MOVING";
			break;
		case IDLE_COMBAT:
			return "IDLE_COMBAT";
			break;
		case MOVING_COMBAT:
			return "MOVING_COMBAT";
			break;
		case ATTACKING:
			return "ATTACKING";
			break;
		case SPRINTING:
			return "SPRINTING";
			break;
		case ENTER_COMBAT:
			return "ENTER_COMBAT";
			break;
		case CLASS_STRIKE:
			return "CLASS_STRIKE";
			break;
		default:
			return " ";
			break;
		}
	}
};