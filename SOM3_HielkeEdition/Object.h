#pragma once
#include <SDL.h>
#include "HielkMath.h"
#include <string>
#include "SDLRenderer.h"


class Object
{
public :
	Object();
	Object(Vector2 _pos, Vector2 _size);
	Object(Vector2 _pos, Vector2 _size, RenderInterface _visual);
	Object(Vector2 _pos, Vector2 _size, SDL_Rect _srcRect, SDL_RendererFlip _flip,std::string _texName);
	~Object();

	void Update(float _dt);
	void Render(SDLRenderer* _Renderer,Vector2 _WorldPos);
	void Render(SDLRenderer* _Renderer, Vector2 _WorldPos, Vector2 _ScaleMult);

	Vector2 m_Pos;
	Vector2 m_Size;
	RenderInterface m_RenderInterface;
private:
};

