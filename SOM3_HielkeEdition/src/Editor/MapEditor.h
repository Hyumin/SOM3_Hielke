#pragma once
#include "..\Engine\SDLRenderer.h"
#include "..\Engine\ResourceManager.h"
#include "..\Engine\Button.h"


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

	Box m_TopBarBox;
	Button m_AddColliderButton;

	ResourceManager* m_ResourceManager;
	//AddcolliderWindow m_ColliderWindow

	Texture* m_IconsTexture;

	std::vector<Button*> m_Buttons;
};

