#pragma once
#include "..\Engine\SDLRenderer.h"
#include "..\Engine\ResourceManager.h"
#include "..\Engine\Button.h"
#include "..\Engine\Map.h"
#include "AddMapColiderWindow.h"


class MapEditor
{
public:
	MapEditor(ResourceManager* _resMan);
	~MapEditor();

	void Update(float _dt);
	void Render(SDLRenderer* _renderer);
	void KeyDown(unsigned int _key);
	void KeyUp(unsigned int _key);
	void MouseUp(unsigned int _key);
	void MouseDown(unsigned int _key);
	void MouseMove(int _x, int _y);


	void ZoomIn();
	void ZoomOut();
	void SaveMap();
	void LoadMap();
	void GiveTopBarBox(Box _topBarBox);//This topbar is the area in which we can format our buttons

private:
	MapEditor();
	void Init();
	void CollidorWindowCallback();
	void GetDefaultAssets();//This function will set the font and icons texture

	Box m_TopBarBox;
	Button m_AddColliderButton;

	ResourceManager* m_ResourceManager;
	AddMapColliderWindow* m_ColliderWindow;
	std::vector<MapEditorWindow*> m_EditorWindows;

	Vector2 m_MousePos;
	Vector2 m_WorldPos;

	Texture* m_IconsTexture;
	TTF_Font* m_Font;
	Hielke::Map* m_CurrentMap;

	std::vector<Button*> m_Buttons;
	float m_Zoom;

	//Variables to help navigate through the map
	bool m_Kup, m_Kleft, m_Kright, m_Kdown, m_Kshift;//bools to control keys
	float m_MoveSpeed;
	float m_SprintMultiplier;

};

