#include "MapEditorWindow.h"

class AddMapColliderWindow : public virtual MapEditorWindow
{
public :
	AddMapColliderWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture);
	AddMapColliderWindow();
	virtual ~AddMapColliderWindow();
	virtual void Update(float _dt);
	virtual void MouseDown(unsigned int _key);
	virtual void MouseUp(unsigned int _key);
	virtual void MouseMove(unsigned int _x, unsigned int _y);
	virtual void KeyDown(unsigned int _key);
	virtual void KeyUp(unsigned int _key);
	virtual void SetFont(TTF_Font* _font);
	virtual void SetMap(Hielke::Map* _map);

	virtual void Render(SDLRenderer* _renderer);
protected:

	virtual void Init(Texture* _IconsTexture);
	virtual void ReScaleContent();
	virtual void Reposition();
};