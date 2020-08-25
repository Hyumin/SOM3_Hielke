#pragma once
#include "ResourceManager.h"
#include "AnimationClip.h"
#include "SDLRenderer.h"
#include "Object.h"
#include "HielkMath.h"

//This class will have the following purpose:
//Creation of new animation clips for any given sprite sheet
//Editing, saving and loading of any existing clip.

class EditorWindow;

class AnimationClipEditor
{
public:
	AnimationClipEditor();
	AnimationClipEditor(ResourceManager* _resMan);
	~AnimationClipEditor();

	void KeyDown(unsigned int _key);
	void KeyUp(unsigned int _key);
	void MouseDown(unsigned int _key);
	void MouseUp(unsigned int _key);
	void MouseMove(int _x, int _y);
	void Update(float _dt);
	void SetResourceManager(ResourceManager* _resMan);
	void Render(SDLRenderer* _renderer);

private:

	void LoadDefaultAssets();

	bool m_Up, m_Left, m_Down, m_Right=false;

	bool m_Dragging;
	std::string m_ClipName;
	std::string m_FileName;

	Object m_SpriteSheet;//The image of the sprite sheet we wish to edit
	AnimationClip m_CurrentClip;
	Object m_CurrentAnimationObject;

	Vector2 m_DragStart;//when you start holding the left mouse button down
	Vector2 m_DragEnd;//When you release the left mouse button
	Vector2 m_MousePos;
	Vector2 m_Position;// "World position"

	ResourceManager* m_ResMan;//Reference to the resource manager do not delete
	BoxCollider m_Box; //Current selection box

	EditorWindow* m_WindowTest;
	TTF_Font* m_DefaultFont;
	
	float m_Zoom = 1.0f;
	float m_Speed = 50.0f;
};

