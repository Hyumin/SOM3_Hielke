#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "HielkMath.h"

struct RenderInterface
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	std::string textureName;
	SDL_RendererFlip renderFlip;
	SDL_Point point;
};
struct TextRenderInterface
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_Texture* texture;
	SDL_RendererFlip renderFlip;
	SDL_Point point;
};
struct Line
{
	Vector2 start;
	Vector2 end;
	SDL_Color colour;
};
struct FilledBox
{
	SDL_Rect box;
	SDL_Color col;
};

class ResourceManager;
class SDLRenderer
{
public:
	SDLRenderer();
	SDLRenderer(std::string _name, unsigned int _width, unsigned int _height);
	~SDLRenderer();

	void AddToRenderqueue(RenderInterface _interface);
	void AddToRenderqueue(TextRenderInterface _interface);
	void AddLine(const Vector2& _a, const Vector2& _b,const Vector2& _worldPos, SDL_Color _color = {0xff,0xff,0xff,0xff});
	
	void Render();
	void Update(float _dt);

	unsigned int GetWindowWidth() { return m_WindowWidth; }
	unsigned int GetWindowHeight() { return m_WindowHeight; }

	void SetWindowWidth(unsigned int _width);
	void SetWindowHeight(unsigned int _height);

	SDL_Renderer* GetRenderer() { return m_Renderer; }
	void SetResourceManager(ResourceManager* _resman);
	void DrawBox(BoxCollider _box, SDL_Color _color = { 0xff,0xff,0xff,0xff }, Vector2 _worldPos = {0.0f,0.0f});
	void DrawBox(int _x, int _y, int _w, int _h, SDL_Color _color = { 0xff,0xff,0xff,0xff }, Vector2 _worldPos = { 0.0f,0.0f });
	void DrawFilledBox(int _x, int _y, int _w, int _h, SDL_Color _color = { 0xff,0xff,0xff,0xff }, Vector2 _worldPos = { 0.0f,0.0f });


private:
	bool Init(std::string _name, unsigned int _width, unsigned int _height);
	void CleanUp();


	unsigned int m_WindowWidth = 1280;
	unsigned int m_WindowHeight = 720;
	std::string m_WindowName = "Seiken 3";

	SDL_Texture* m_FPSTexture;
	std::string m_FPSinstring = "FPS";
	int m_FPSWidth = 0;
	int m_FPSHeight =0;


	TTF_Font* m_DefaultFont = NULL;
	SDL_Renderer* m_Renderer = NULL;
	SDL_Window* m_Window = NULL;
	ResourceManager* m_ResMan = NULL;

	std::vector<RenderInterface> m_RenderQueue;
	std::vector<TextRenderInterface> m_TextRenderQueue;
	std::vector<Line> m_LineQueue;
	std::vector< FilledBox> m_FilledBoxes;


};

