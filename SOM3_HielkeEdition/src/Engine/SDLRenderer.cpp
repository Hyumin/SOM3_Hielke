#include "SDLRenderer.h"

#include "ResourceManager.h"
#include "Layer.h"
#include <iostream>

using namespace std;

SDLRenderer::SDLRenderer()
{
	if (!Init(m_WindowName,m_WindowWidth,m_WindowHeight))
	{
		printf("Initialization has failed \n");
	}
}

SDLRenderer::SDLRenderer(std::string _name, unsigned int _width, unsigned int _height)
{
	if (!Init(_name, _width, _height))
	{
		printf("Initialization has failed \n");
	}
}

SDLRenderer::~SDLRenderer()
{
	CleanUp();
}

void SDLRenderer::AddToRenderqueue(RenderInterface _interface, unsigned int _layer )
{
	m_Layers[_layer].AddInterface(_interface);
}

void SDLRenderer::AddToRenderqueue(TextRenderInterface _interface, unsigned int _layer )
{
	m_Layers[_layer].AddText(_interface);
}

void SDLRenderer::AddLine(const Vector2& _a, const Vector2& _b, const Vector2& _worldPos, SDL_Color _color, unsigned int _layer )
{
	Line newLine;
	newLine.start = _a;
	newLine.end = _b;
	newLine.start -= _worldPos;
	newLine.end -= _worldPos;


	newLine.colour = _color;

	m_Layers[_layer].AddLine(newLine);
}

void SDLRenderer::AddLine(const Line& _line, const Vector2& _worldPos, unsigned int _layer)
{
	Line newLine= _line;
	newLine.start -= _worldPos;
	newLine.end -= _worldPos;


	m_Layers[_layer].AddLine(newLine);
}

void SDLRenderer::AddLineZoomed(const Vector2& _a, const Vector2& _b, const Vector2& _worldPos, SDL_Color _color, float _zoom, unsigned int _layer)
{
	Line newLine;
	newLine.start = _a*_zoom;
	newLine.end = _b*_zoom;
	newLine.start -= _worldPos*_zoom;
	newLine.end -= _worldPos*_zoom;


	newLine.colour = _color;

	m_Layers[_layer].AddLine(newLine);
}

void SDLRenderer::AddLineZoomed(const Line& _line, const Vector2& _worldPos, float _zoom, unsigned int _layer)
{
	Line newLine = _line;
	newLine.start *=  _zoom;
	newLine.end *=  _zoom;
	newLine.start -= _worldPos * _zoom;
	newLine.end -= _worldPos * _zoom;



	m_Layers[_layer].AddLine(newLine);
}

void SDLRenderer::Render()
{
	if (m_ResMan != nullptr)
	{

		SDL_Rect txtsrc= { 0,0,m_FPSWidth,m_FPSHeight };
		SDL_Rect txtdest = { 0,0,m_FPSWidth,m_FPSHeight };
		
		//Clear screen
		SDL_SetRenderDrawColor(m_Renderer, 0x0, 0x8D, 0xAD, 0xff);
		SDL_RenderClear(m_Renderer);
		for (unsigned int l = 0; l < m_Layers.size(); ++l)
		{
			std::vector<RenderInterface> renderInterfaces = m_Layers[l].GetRenderQueue();
			//Iterate through the render queue
			for (int i = 0; i < renderInterfaces.size(); ++i)
			{
				RenderInterface inter = renderInterfaces[i];
				//Get texture based on string identifier within the _interface
				//Then copy it to the renderer
				Texture* tex = m_ResMan->GetTexture(inter.textureName);
				if (tex != nullptr)
				{
					//SDL_point is only used for SDL_rotate
					SDL_RenderCopyEx(m_Renderer, tex->GetTexture(), &inter.srcRect, &inter.destRect, 0, 0, inter.renderFlip);
				}
			}
		
			std::vector<Line> lines = m_Layers[l].GetLineQueue();

			for (int i = 0; i < lines.size(); ++i)
			{
				Line l = lines[i];

				SDL_SetRenderDrawColor(m_Renderer, l.colour.r, l.colour.g, l.colour.b, l.colour.a);
				SDL_RenderDrawLine(m_Renderer, (int)l.start.x, (int)l.start.y, (int)l.end.x, (int)l.end.y);
			}
			std::vector<FilledBox> boxes = m_Layers[l].GetFilledBoxQueue();

			for (int i = 0; i < boxes.size(); ++i)
			{
				SDL_SetRenderDrawColor(m_Renderer, boxes[i].col.r, boxes[i].col.g, boxes[i].col.b, boxes[i].col.a);
				SDL_RenderFillRect(m_Renderer, &boxes[i].box);
			}
			std::vector<TextRenderInterface> textInterfaces = m_Layers[l].GetTextRenderQueue();

			//Iterate through text renderer
			for (int i = 0; i < textInterfaces.size(); ++i)
			{
				TextRenderInterface inter = textInterfaces[i];
				//Get texture based on string identifier within the _interface
				//Then copy it to the renderer
				const SDL_RendererFlip flip = inter.renderFlip;
				if (inter.texture != nullptr)
				{
					SDL_RenderCopyEx(m_Renderer, inter.texture, &inter.srcRect, &inter.destRect, 0, 0, flip);
				}
			}

			m_Layers[l].ClearQueues();

		
		}
		//Present screen
		SDL_RenderPresent(m_Renderer);
	}
	else
	{
		printf("SDLRenderer: Resourcemanager was not set but the render function requires one to work \n");
	}

}

void SDLRenderer::Update(float _dt)
{


}

void SDLRenderer::SetWindowWidth(unsigned int _width)
{
	m_WindowWidth = _width;
	//Resize window
}

void SDLRenderer::SetWindowHeight(unsigned int _height)
{
	m_WindowHeight = _height;
	//Resize window
	
}

void SDLRenderer::SetResourceManager(ResourceManager* _resman)
{
	m_ResMan = _resman;
}

void SDLRenderer::DrawBox(Box _box, SDL_Color _color, Vector2 _worldPos, unsigned int _layer )
{
	//Draw pos x to x+w
	Vector2 screenPos = _box.pos  ;
	Vector2 endPoint= screenPos;
	Vector2 startPoint = screenPos;
	endPoint.x += _box.w;
	AddLine(startPoint, endPoint, _worldPos, _color, _layer);

	//Draw x+w to y+h
	startPoint = endPoint;
	endPoint.y += _box.h;

	AddLine(startPoint, endPoint, _worldPos, _color, _layer);
	
	//Draw xy+wh to y+h
	startPoint = endPoint;
	endPoint.x = screenPos.x;
	AddLine(startPoint, endPoint, _worldPos, _color,_layer);
	//finally draw y+h to start point
	startPoint = endPoint;
	endPoint = screenPos;
	AddLine(startPoint, endPoint, _worldPos,_color, _layer);

}

void SDLRenderer::DrawBox(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos, unsigned int _layer)
{
	//Draw pos x to x+w
	Vector2 screenPos = Vector2((float)_x, (float)_y);
	Vector2 endPoint = screenPos;
	Vector2 startPoint = screenPos;
	endPoint.x += _h;
	AddLine(startPoint, endPoint, _worldPos, _color, _layer);

	//Draw x+w to y+h
	startPoint = endPoint;
	endPoint.y += _h;

	AddLine(startPoint, endPoint, _worldPos, _color, _layer);

	//Draw xy+wh to y+h
	startPoint = endPoint;
	endPoint.x = screenPos.x;
	AddLine(startPoint, endPoint, _worldPos, _color, _layer);
	//finally draw y+h to start point
	startPoint = endPoint;
	endPoint = screenPos;
	AddLine(startPoint, endPoint, _worldPos, _color, _layer);
}

void SDLRenderer::DrawFilledBox(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos, unsigned int _layer )
{
	FilledBox b;
	b.box = { _x-(int)_worldPos.x,_y-(int)_worldPos.y,_w,_h };
	b.col = _color;
	m_Layers[_layer].AddFilledBox(b);
}

void SDLRenderer::DrawFilledBox(Box _box, SDL_Color _color, Vector2 _worldPos, unsigned int _layer)
{
	FilledBox b;
	b.box = {(int) _box.pos.x - (int)_worldPos.x,(int)_box.pos.y - (int)_worldPos.y,(int)_box.w,(int)_box.h };
	b.col = _color;
	m_Layers[_layer].AddFilledBox(b);
}

void SDLRenderer::DrawBoxZoomed(Box _box, SDL_Color _color, Vector2 _worldPos, float _zoom, unsigned int _layer)
{
	//Draw pos x to x+w
	Vector2 screenPos = _box.pos*_zoom;
	Vector2 endPoint = screenPos;
	Vector2 startPoint = screenPos;
	_box.w *= _zoom;
	_box.h *= _zoom;
	endPoint.x += _box.w;
	AddLine(startPoint, endPoint, _worldPos*_zoom, _color, _layer);

	//Draw x+w to y+h
	startPoint = endPoint;
	endPoint.y += _box.h;

	AddLine(startPoint, endPoint, _worldPos * _zoom, _color, _layer);

	//Draw xy+wh to y+h
	startPoint = endPoint;
	endPoint.x = screenPos.x;
	AddLine(startPoint, endPoint, _worldPos * _zoom, _color, _layer);
	//finally draw y+h to start point
	startPoint = endPoint;
	endPoint = screenPos;
	AddLine(startPoint, endPoint, _worldPos * _zoom, _color, _layer);
}

void SDLRenderer::DrawBoxZoomed(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos, float _zoom, unsigned int _layer)
{
	//Draw pos x to x+w
	Vector2 screenPos = Vector2(_x, (float)_y)*_zoom;
	Vector2 endPoint = screenPos;
	Vector2 startPoint = screenPos;
	endPoint.x += _h*_zoom;
	AddLine(startPoint, endPoint, _worldPos * _zoom, _color, _layer);

	//Draw x+w to y+h
	startPoint = endPoint;
	endPoint.y += _h * _zoom;

	AddLine(startPoint, endPoint, _worldPos * _zoom, _color, _layer);

	//Draw xy+wh to y+h
	startPoint = endPoint;
	endPoint.x = screenPos.x;
	AddLine(startPoint, endPoint, _worldPos * _zoom, _color, _layer);
	//finally draw y+h to start point
	startPoint = endPoint;
	endPoint = screenPos;
	AddLine(startPoint, endPoint, _worldPos * _zoom, _color, _layer);
}

void SDLRenderer::DrawFilledBoxZoomed(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos, float _zoom, unsigned int _layer)
{
	
	FilledBox b;
	b.box = { _x - (int)(_worldPos.x * _zoom),_y - (int)(_worldPos.y * _zoom),(int)(_w*_zoom),(int)(_h*_zoom) };
	b.box.x = (int)(_zoom *b.box.x);
	b.box.y = (int)(_zoom* b.box.y);
	b.col = _color;
	m_Layers[_layer].AddFilledBox(b);
}

void SDLRenderer::DrawFilledBoxZoomed(Box _box, SDL_Color _color, Vector2 _worldPos, float _zoom, unsigned int _layer)
{
	FilledBox b;
	b.box = { (int)_box.pos.x - (int)(_worldPos.x * _zoom),(int)_box.pos.y - (int)(_worldPos.y * _zoom),(int)(_box.w*_zoom),(int)(_box.h*_zoom) };
	b.box.x = (int)(_zoom * b.box.x);
	b.box.y = (int)(_zoom * b.box.y);
	b.col = _color;
	m_Layers[_layer].AddFilledBox(b);
}

bool SDLRenderer::Init(std::string _name, unsigned int _width, unsigned int _height)
{
	bool succes = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initialized SDL_ERROR: %s \n", SDL_GetError());
		succes = false;
	}
	else //SDL initialization succesfull
	{
		//Create a window
		m_Window = SDL_CreateWindow(_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_SHOWN);
		if (m_Window == NULL)
		{
			printf("Window couold not be created! SDL_ERROR: %s \n", SDL_GetError());
			succes = false;
		}
		else
		{
			//Assign height,width and name to their variables
			m_WindowName = _name;
			m_WindowWidth = _width;
			m_WindowHeight = _height;

			//Create renderer for window allowing textures to be used so we can utilise the gpu instead of the cpu for graphics
			m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
			if (m_Renderer == NULL)
			{
				printf("Window renderer  could not be created! SDL_ERROR: %s\n", SDL_GetError());
				succes = false;
			}
			else
			{
				SDL_SetRenderDrawColor(m_Renderer, 0x0, 0x8D, 0xAD, 0xff);

				//Initialize SDL_Image
				int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

				//IMG_init returns  the flags it initialized or 0 on failure, and operation with the flags will allow us to check whether 
				//the correct fileformat has been loaded
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					succes = false;
				}
				else
				{
					//succes

					
				}
				if (TTF_Init() == -1)
				{
					printf("Failed to initailize! SDL_ttf Error: %s \n", TTF_GetError());
				}
				else
				{
					//Load in default font
					m_DefaultFont = TTF_OpenFont("Assets//Fonts//Jupiter.ttf", 28);
					if (m_DefaultFont == NULL)
					{
						printf("Could not load defaultfont \n");
					}
				}
			}

			m_Layers.push_back(Layer("Default"));
			m_Layers.push_back(Layer("Background"));
			m_Layers.push_back(Layer("UI"));
		}
	}

	return succes;
}

void SDLRenderer::CleanUp()
{
	m_ResMan = NULL;
	SDL_DestroyRenderer(m_Renderer);
	m_Renderer = NULL;
	SDL_DestroyWindow(m_Window);
	m_Window = NULL;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
