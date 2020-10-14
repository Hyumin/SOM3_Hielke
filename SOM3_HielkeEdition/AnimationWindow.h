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

       void PlayClip();
       void LoopClip();
       void PauseClip();

       //Could be a button, 
       Box m_EnableLooping;
       Box m_BottomContentBox;
       Box m_TopContentBox;


       //Rename to normal once colliders have been refactored out
       Button m_PlayButton;
       Button m_PauseButton;
       Button m_LoopButton;

       bool m_Playing, m_Looping, m_Pausing;
   
       TextField m_FilePathTextField;
       TextField m_IsLoopingTextField;
       Object m_Obj;


       Vector2 m_ButtonsOffset;
};

