#pragma once
#include "EditorWindow.h"
#include "InputTextField.h"


class AddFrameWindow :
    public virtual EditorWindow
{
public:

    AddFrameWindow(Vector2 _pos, const std::string& _name, Texture* _IconsTexture);
    AddFrameWindow();
    virtual ~AddFrameWindow();

    virtual void Update(float _dt);
    virtual void MouseDown(unsigned int _key);
    virtual void MouseUp(unsigned int _key);
    virtual void MouseMove(unsigned int _x, unsigned int _y);
    virtual void SetFont(TTF_Font* _font);
    virtual void KeyDown(unsigned int _key);
    virtual void KeyUp(unsigned int _key);


    virtual void Render(SDLRenderer* _renderer);
    virtual void SetClip(AnimationClip* _clip);


protected:
    virtual void Init(Texture* _IconsTexture);
    virtual void ReScaleContent();
    virtual  void Reposition();
    void AddFrame();
    void PrevFrame();
    void NextFrame();


    Button m_AddFrameButton;
    Button m_PrevFrameButton;
    Button m_NextFrameButton;

    Vector2 m_AfOffset,m_PrevOffset,m_NextOffset, m_XInputOffset,m_YInputOffset, m_WInputOffset, m_HInputOffset;
    InputTextField* m_XInput,*m_YInput,*m_WInput,*m_HInput;
    std::vector<InputTextField*> m_InputFields; // Store al the input textfields in the vector but keep the pointer of them seperate somewhere
                                                // so we can acces seperately when needed, the pointers won't be deleted unless we delete the addfraem windwow anyways
    int m_X;
    unsigned int m_CurrentIndex = 0;
    
    TextField m_CurrentFrameField;
    std::string m_ConstFrameText = "Current frame:";
};

