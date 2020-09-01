#pragma once
#include <string>
#include "SDLRenderer.h"
#include "HielkMath.h"
#include "TextField.h"
#include "Object.h"

/// <summary>
///  we want an editor window to be a draggable object that contains some kind of data
/// e.g. one that shows the current animation, or one that has a okay or no button or something
/// </summary>

class Texture;
class EditorWindow
{
public:
	EditorWindow(Vector2 _pos, std::string& _name,Texture* _IconsTexture);
	EditorWindow();
	~EditorWindow();


	void Update(float _dt);
	void MouseDown(unsigned int _key);
	void MouseUp(unsigned int _key);
	void MouseMove(unsigned int _x, unsigned int _y);
	bool CanDelete() { return m_ReadyForDelete; }

	void SetFont(TTF_Font* _font);
	void SetName(std::string& _name);
	std::string& GetName() { return m_Name; }
	void SetShowingObject(Object& _obj);
	void Render(SDLRenderer* _renderer);

	bool m_Dragging;

private:

	bool m_ScalingSize = false;

	void Init(Texture* _IconsTexture);
	void ReScaleContent();



	std::string m_Name;
	BoxCollider m_Bar;
	BoxCollider m_CrossCollider;
	BoxCollider m_ContentScaler;


	Object m_CrossObject;
	Object m_ContentScaleObject;


	BoxCollider m_ContentBox;
	Vector2 m_BarRelativePos;
	Vector2 m_CrossRelativePos;
	Vector2 m_ContentRelativePos;
	Object m_Obj;
	TextField m_TextField;

	Texture* m_IconTexture;


	Vector2 m_Pos;
	bool m_ReadyForDelete;

	Vector2 m_MousePos;

	//For scaling
	Vector2 m_ScaleStart;
	Vector2 m_ScaleOnStart;

	//For dragging the window with the mouse
	Vector2 m_StartDragPos;
};

