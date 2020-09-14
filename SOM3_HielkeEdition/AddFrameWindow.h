#pragma once
#include "EditorWindow.h"
class AddFrameWindow :
    public virtual EditorWindow
{
public:

    AddFrameWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture);
    AddFrameWindow();
    virtual ~AddFrameWindow();

    virtual void Update(float _dt);
    virtual void MouseDown(unsigned int _key);
    virtual void MouseUp(unsigned int _key);
    virtual void MouseMove(unsigned int _x, unsigned int _y);
    virtual void SetFont(TTF_Font* _font);

    virtual void Render(SDLRenderer* _renderer);
    virtual void SetClip(AnimationClip* _clip);

protected:


};

