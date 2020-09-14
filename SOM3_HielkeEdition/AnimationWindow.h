#pragma once
#include "EditorWindow.h"
class AnimationWindow :
    public virtual  EditorWindow
{
public:
      
      AnimationWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture);
      AnimationWindow();
      virtual ~AnimationWindow();
      virtual void Update(float _dt);
      virtual void MouseDown(unsigned int _key);
      virtual void MouseUp(unsigned int _key);
      virtual void MouseMove(unsigned int _x, unsigned int _y);


      virtual void Render(SDLRenderer* _renderer);
      virtual void SetClip(AnimationClip* _clip);
      virtual void SetFont(TTF_Font* _font);

protected:
       virtual void Init(Texture* _IconsTexture);
       virtual void ReScaleContent();

       //Buttons 
       BoxCollider m_PauseButton;
       BoxCollider m_PlayButton;
       BoxCollider m_LoopButton;

       BoxCollider m_EnableLooping;

       Object m_PlayObject, m_LoopObject, m_PauseObject;


       bool m_Playing, m_Looping, m_Pausing;
   
       TextField m_FilePathTextField;
       TextField m_IsLoopingTextField;
       Object m_Obj;

};

