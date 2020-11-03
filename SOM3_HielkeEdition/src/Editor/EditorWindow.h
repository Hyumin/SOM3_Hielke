#pragma once
#include <string>
#include "..\Engine\SDLRenderer.h"
#include "..\HielkMath.h"
#include "..\Engine\TextField.h"
#include "..\Engine\Object.h"
#include "..\Engine\Button.h"

/// <summary>
///  we want an editor window to be a draggable object that contains some kind of data
/// e.g. one that shows the current animation, or one that has a okay or no button or something
/// </summary>

class AnimationClip;
class Texture;
class EditorWindow
{
public:
	EditorWindow(Vector2 _pos, std::string& _name,Texture* _IconsTexture);
	EditorWindow();
	virtual ~EditorWindow();

	virtual void Update(float _dt);
	virtual void MouseDown(unsigned int _key);
	virtual void MouseUp(unsigned int _key);
	virtual void MouseMove(unsigned int _x, unsigned int _y);
	virtual void KeyDown(unsigned int _key)= 0;
	virtual void KeyUp(unsigned int _key) =0;
	bool CanDelete() { return m_ReadyForDelete; }

	virtual void SetFont(TTF_Font* _font);
	void SetName(std::string& _name);
	std::string& GetName() { return m_Name; }
	
	virtual void Render(SDLRenderer* _renderer);
	virtual void SetClip(AnimationClip* _clip);
	
	void ExitPressed();

	bool m_Dragging;
	bool m_ReadyForDelete;
	bool m_ChangeToAnimationClip;// If anything changes that requires the animation clip editor to change something set this to true

protected:

	bool m_ScalingSize = false;

	virtual void Init(Texture* _IconsTexture);
	virtual void ReScaleContent();
	virtual void Reposition();//Makes it so the positions of the objects only change when dragging 

	AnimationClip* m_CurrentClip;

	std::string m_Name;
	Box m_Bar;
	Box m_ContentScaler;
	Box m_ContentInformation;

	Object m_ContentScaleObject;

	//This box defines everything below the Title bar
	Box m_ContentBox;


	Vector2 m_BarRelativePos;
	Vector2 m_CrossRelativePos;
	Vector2 m_ContentRelativePos;

	//The name of the window textfield
	TextField m_TextField;

	Texture* m_IconTexture;

	Vector2 m_Pos;

	Vector2 m_MousePos;

	//For scaling
	Vector2 m_ScaleStart;
	Vector2 m_ScaleOnStart;

	//For dragging the window with the mouse
	Vector2 m_StartDragPos;

	Button m_ExitButton;

	//Colours to be rendered for the window, Re-use this for other stuff for consistency
	SDL_Color m_LightColor;
	SDL_Color m_Color;
	SDL_Color m_DarkColor;
};
