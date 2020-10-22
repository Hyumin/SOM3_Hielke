#pragma once
#include "EditorWindow.h"
#include "InputTextField.h"

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
      virtual void KeyDown(unsigned int _key);
      virtual void KeyUp(unsigned int _key);

      virtual void Render(SDLRenderer* _renderer);
      virtual void SetClip(AnimationClip* _clip);
      virtual void SetFont(TTF_Font* _font);

protected:
       virtual void Init(Texture* _IconsTexture);
       virtual void ReScaleContent();
         virtual  void Reposition();

       void PlayClip();
       void LoopClip();
       void PauseClip();
       void NextFrame();
       void PrevFrame();
       void SpeedUp();
       void SlowDown();

       Box m_EnableLooping; //Could be a button, 
       Box m_BottomContentBox;
       Box m_TopContentBox;
       Box m_InGamePreviewBox;
       Box m_RawPreviewBox;

       //Buttons
       Button m_PlayButton;
       Button m_PauseButton;
       Button m_LoopButton;
       Button m_NextFrame;
       Button m_PrevFrame;
    
       //Fast forward button
       Button m_FastForward;
       Button m_SlowDown;

       float m_PlayBackSpeed;

       bool m_Playing, m_Looping, m_Pausing;
    

       //Textfields
       TextField m_FilePathTextField;
       TextField m_IsLoopingTextField;
       TextField m_FrameCounterTextField;
       TextField m_CurrentSpeedTextField;
       TextField m_InGameText;
       TextField m_RawText;

       //Input TextFields
       InputTextField m_IntervalInputField;
       //Use this to call generic functions like Render
       std::vector<TextField*> m_TextFields;
       std::vector<Button*> m_Buttons;

       Object m_Obj;
       Object m_InGame;


       Vector2 m_ButtonsOffset;
};

