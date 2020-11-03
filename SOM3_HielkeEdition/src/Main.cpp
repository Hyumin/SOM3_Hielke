
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "Engine\ResourceManager.h"
#include "Game\Game.h"
#include "Engine\SDLRenderer.h"
#include "Editor\AnimtionClipEditor.h"


//Made using https://lazyfoo.net/tutorials/SDL/index.php

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const char* WINDOWNAME = "Hielke's cool windowname ";


bool init();

void close();


SDLRenderer* g_Renderer= NULL;
ResourceManager* g_Resmanager = NULL;
Game* g_Game = NULL;
AnimationClipEditor* g_ClipEditor = NULL;


bool SPRITEDITORMODE = true;


int main(int arg, char* args[])
{
	bool quit = false;
	SDL_Event e;

	float start = 0.0f;
	float end = 0.0f;
	float timer = 0.0f;

	if (!init())
	{
		printf(" Failed to initialize SDL \n");
		close();
		return 1;
	}
	else
	{
		start = SDL_GetTicks();
		//Main loop
		while (!quit)
		{
			start = end;
			end = (float)SDL_GetTicks();
			float dt = (end - start) / 1000.0f;
			//Poll events
			while (SDL_PollEvent(&e) !=0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					if (SPRITEDITORMODE)
					{
						g_ClipEditor->KeyDown(e.key.keysym.sym);
					}
					else
					{
						g_Game->KeyDown(e.key.keysym.sym);
					}
					switch (e.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						quit = true;
						break;
					}
				}
				else if (e.type == SDL_KEYUP)
				{
					if (SPRITEDITORMODE)
					{
						g_ClipEditor->KeyUp(e.key.keysym.sym);
					}
					else 
					{
						g_Game->KeyUp(e.key.keysym.sym);
					}
				}
				else if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					if (SPRITEDITORMODE)
					{
						g_ClipEditor->MouseDown(e.button.button);
					}
				}
				else if (e.type == SDL_MOUSEBUTTONUP)
				{
					if (SPRITEDITORMODE)
					{
						g_ClipEditor->MouseUp(e.button.button);
					}
				}
				else if (e.type == SDL_MOUSEMOTION)
				{
					//Get mouse position
					int x, y;
					SDL_GetMouseState(&x, &y);
					if (SPRITEDITORMODE)
					{
						g_ClipEditor->MouseMove(x, y);
					}
				}
			}
			// if sprite editor mode launch sprite editor
			if (SPRITEDITORMODE)
			{
				g_ClipEditor->Update(dt);
				g_ClipEditor->Render(g_Renderer);
			}
			else
			{
				g_Game->Update(dt);
				g_Game->Render(g_Renderer);
			}
			g_Renderer->Update(dt);
			g_Renderer->Render();	
		}
		
	}
	close();

	return 0;
}



bool init()
{
	bool succes = true;
	
	g_Renderer = new SDLRenderer(std::string(WINDOWNAME), SCREEN_WIDTH, SCREEN_HEIGHT);
	g_Resmanager = new ResourceManager(g_Renderer);
	g_Renderer->SetResourceManager(g_Resmanager);
	if (SPRITEDITORMODE)
	{
		g_ClipEditor = new AnimationClipEditor(g_Resmanager);
	}
	else
	{
		g_Game = new Game(g_Resmanager);
	}

	return succes;
}


//unloads and closes sdl systeems
void close()
{
	delete g_Game;
	g_Game = NULL;
	
	delete g_ClipEditor;
	g_ClipEditor = NULL;

	delete g_Resmanager;
	g_Resmanager = NULL;

	delete g_Renderer;
	g_Renderer = NULL;
	

}

