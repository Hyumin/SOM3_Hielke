#pragma once
#include "Enemy.h"
#include <map>
#include "PlayerCharacter.h"

class AnimationClip;
namespace Hielke
{
    class Map;
};
class Rabite :
     public virtual Enemy
{
public:
    Rabite();
    Rabite(Object& _obj);
    virtual ~Rabite();

    virtual void SetPlayerTarget(Hielke::PlayerCharacter* _Player);
    virtual void Update(float _dt);
    virtual void Render(SDLRenderer* _renderer, Vector2 _worldPos);
    virtual void RenderZoomed(SDLRenderer* _renderer, Vector2 _worldPos, float _zoom);
    virtual void HitReaction(Hielke::PlayerStats _stats);
    void AnimationStateMachine();
    virtual void SetMap(Hielke::Map* _map);
    void Attack();

    void SetIdleAnimations(AnimationClip _north, AnimationClip _horizontal, AnimationClip _south);
    void SetMoveAnimations(AnimationClip _north, AnimationClip _horizontal, AnimationClip _south);
    void SetAttackAnimations(AnimationClip _north, AnimationClip _horizontal, AnimationClip _south);
    std::map<Direction, AnimationClip> GetIdleAnimMap();
    std::map<Direction, AnimationClip> GetMoveAnimMap();
    std::map<Direction, AnimationClip> GetAttacAnimMap();


protected:

    bool attackFrame = false;
    AnimationClip* m_CurrentAnimationClip;
    std::map<Direction, AnimationClip> m_IdleAnimation;
    std::map<Direction, AnimationClip> m_MoveAnimation;
    std::map<Direction, AnimationClip> m_AttackAnimation;
    Box m_AttackCollider;

};

