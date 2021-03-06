#include "SDLRenderer.h"

#include "ResourceManager.h"
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

void SDLRenderer::AddToRenderqueue(RenderInterface _interface)
{
	m_RenderQueue.push_back(_interface);
}

void SDLRenderer::AddToRenderqueue(TextRenderInterface _interface)
{
	m_TextRenderQueue.push_back(_interface);
}

void SDLRenderer::AddLine(const Vector2& _a, const Vector2& _b, const Vector2& _worldPos, SDL_Color _color)
{
	Line newLine;
	newLine.start = _a;
	newLine.end = _b;
	newLine.start -= _worldPos;
	newLine.end -= _worldPos;


	newLine.colour = _color;

	m_LineQueue.push_back(newLine);
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
	
		//Iterate through the render queue
		for (int i = 0; i < m_RenderQueue.size(); ++i)
		{
			RenderInterface inter = m_RenderQueue[i];
			//Get texture based on string identifier within the _interface
			//Then copy it to the renderer
			Texture* tex = m_ResMan->GetTexture(inter.textureName);
			if (tex != nullptr)
			{
				//SDL_point is only used for SDL_rotate
				SDL_RenderCopyEx(m_Renderer, tex->GetTexture(), &inter.srcRect, &inter.destRect, 0, 0, inter.renderFlip);
			}
		}
		//Iterate through text renderer
		for (int i = 0; i < m_TextRenderQueue.size(); ++i)
		{
			TextRenderInterface inter = m_TextRenderQueue[i];
			//Get texture based on string identifier within the _interface
			//Then copy it to the renderer
			const SDL_RendererFlip flip = inter.renderFlip;
			if (inter.texture != nullptr)
			{
				SDL_RenderCopyEx(m_Renderer, inter.texture, &inter.srcRect, &inter.destRect, 0, 0, flip);
			}
		}
		for (int i = 0; i < m_LineQueue.size(); ++i)
		{
			Line l = m_LineQueue[i];

			SDL_SetRenderDrawColor(m_Renderer, l.colour.r, l.colour.g, l.colour.b, l.colour.a);
			SDL_RenderDrawLine(m_Renderer, (int)l.start.x, (int)l.start.y, (int)l.end.x, (int)l.end.y);
		}
		for (int i = 0; i < m_FilledBoxes.size(); ++i)
		{
			SDL_SetRenderDrawColor(m_Renderer, m_FilledBoxes[i].col.r, m_FilledBoxes[i].col.g, m_FilledBoxes[i].col.b, m_FilledBoxes[i].col.a);
			SDL_RenderFillRect(m_Renderer, &m_FilledBoxes[i].box);
		}

		//Present screen
		SDL_RenderPresent(m_Renderer);
	}
	else
	{
		printf("SDLRenderer: Resourcemanager was not set but the render function requires one to work \n");
	}
	m_RenderQueue.clear();
	m_TextRenderQueue.clear();
	m_LineQueue.clear();
	m_FilledBoxes.clear();

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

void SDLRenderer::DrawBox(BoxCollider _box, SDL_Color _color, Vector2 _worldPos)
{
	//Draw pos x to x+w
	Vector2 screenPos = _box.pos  ;
	Vector2 endPoint= screenPos;
	Vector2 startPoint = screenPos;
	endPoint.x += _box.w;
	AddLine(startPoint, endPoint, _worldPos, _color);

	//Draw x+w to y+h
	startPoint = endPoint;
	endPoint.y += _box.h;

	AddLine(startPoint, endPoint, _worldPos, _color);
	
	//Draw xy+wh to y+h
	startPoint = endPoint;
	endPoint.x = screenPos.x;
	AddLine(startPoint, endPoint, _worldPos, _color);
	//finally draw y+h to start point
	startPoint = endPoint;
	endPoint = screenPos;
	AddLine(startPoint, endPoint, _worldPos,_color);

}

void SDLRenderer::DrawBox(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos)
{
	//Draw pos x to x+w
	Vector2 screenPos = Vector2(_x,_y);
	Vector2 endPoint = screenPos;
	Vector2 startPoint = screenPos;
	endPoint.x += _h;
	AddLine(startPoint, endPoint, _worldPos, _color);

	//Draw x+w to y+h
	startPoint = endPoint;
	endPoint.y += _h;

	AddLine(startPoint, endPoint, _worldPos, _color);

	//Draw xy+wh to y+h
	startPoint = endPoint;
	endPoint.x = screenPos.x;
	AddLine(startPoint, endPoint, _worldPos, _color);
	//finally draw y+h to start point
	startPoint = endPoint;
	endPoint = screenPos;
	AddLine(startPoint, endPoint, _worldPos, _color);
}

void SDLRenderer::DrawFilledBox(int _x, int _y, int _w, int _h, SDL_Color _color, Vector2 _worldPos)
{
	FilledBox b;
	b.box = { _x-(int)_worldPos.x,_y-(int)_worldPos.y,_w,_h };
	b.col = _color;
	m_FilledBoxes.push_back(b);

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
