#include "MapEditorWindow.h"
#include "..\Engine\Button.h"
#include "..\Engine\InputTextField.h"
#include "..\Game\Enemy.h"
#include "..\Engine\RenderTarget.h"
#include "..\Engine\ScrollBar.h"

enum class MapHierachyWindowMode
{
	WallSelect, ConnectedMapMode,Enemy,NPC
};

class MapHierachyWindow : public virtual MapEditorWindow
{

public :
	MapHierachyWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture);
	MapHierachyWindow();
	virtual ~MapHierachyWindow();
	virtual void Update(float _dt);
	virtual void MouseDown(unsigned int _key);
	virtual void MouseUp(unsigned int _key);
	virtual void MouseMove(unsigned int _x, unsigned int _y);
	virtual void KeyDown(unsigned int _key);
	virtual void KeyUp(unsigned int _key);
	virtual void SetFont(TTF_Font* _font);
	virtual void SetMap(Hielke::Map* _map);
	void MouseWheel(int _x, int _y);
	virtual void Render(SDLRenderer* _renderer);



	void SelectWall(int _index);
protected:

	virtual void Init(Texture* _IconsTexture);
	virtual void ReScaleContent();
	virtual void Reposition();

	void RepositionButtons();
	void RepositionInputTexts();
	void GenerateHierachyWalls();

	ScrollBar *m_ScrollBarObj;
	std::vector<Button*> m_WallButtons;

	Button m_ScrollUp, m_ScrollDown;

	std::vector<Button*> m_Buttons;
	std::vector<InputTextField*> m_InputTexts;

	Enemy* m_CurrentSelectedEnemy;
	Box* m_CurrentSelectedWall;
	MapHierachyWindowMode m_Mode;
	Box m_CurrentSelectedBox;
	Box m_PrevContentBox;
	RenderTarget* m_Target;

	TTF_Font* m_Font;

};