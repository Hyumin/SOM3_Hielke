#pragma once
#include "HielkMath.h"
#include "SDLRenderer.h"
#include "Object.h"
#include "TextField.h"
#include <functional>



class Button
{
	public:
		Button();
		~Button();
		void Update(float _dt);
		void MouseDown(unsigned int _key);
		void MouseMove(int _x,int _y);
		void MouseUp(unsigned int _key);
		void Render(SDLRenderer* _renderer);

		bool Clicked() 
		{
			return m_Clicked; 
		}

		enum  DrawMode
		{
			WIREFRAME = 0,
			FILLEDRECT =1,
			TEXTURE =2
		};

		void SetTextureDrawMode(RenderInterface _norm, RenderInterface _clicked, RenderInterface _hovered);
		void SetFilledRectMode(SDL_Colour _normal,SDL_Colour _hovered, SDL_Colour _clicked);//
		void SetWireFrameMode(SDL_Colour _normal, SDL_Colour _hovered, SDL_Colour _clicked);
		void SetLayer(int _layer);
		int GetLayer() { return m_Layer; }

		void SetSize(Vector2 _size);
		Vector2 GetSize() { return m_Size; }
		void SetPosition(Vector2 _pos);
		Vector2 GetPosition() { return m_Pos; }
		void SetCallbackFunction(std::function<void()>fnc);// to pass in a function call back use std::bind
														   // Example: std::bind(&Class::Function,this); (for a function with no arguments second parameter must be the object of the class
														   // https://stackoverflow.com/questions/22422147/why-is-stdbind-not-working-without-placeholders-in-this-example-member-functi

	private:
		void Init();

		bool m_Hovered,m_Clicked;
		Vector2 m_Size;
		Vector2 m_MousePos;
		Vector2 m_Pos;
		RenderInterface m_TexHovered;
		RenderInterface m_TexClicked;
		RenderInterface m_TextureNorm;
		Object m_Object;
		DrawMode m_DrawMode;
		BoxCollider m_Collider;
		TextField m_TextField;
		TTF_Font* m_Font; // this will be an external reference, do not delete in this class
		
		int m_Layer;

		std::function<void()> m_Callback;

		//Colours for wireframe and filled rect modes, might make a filled wire frame mode in the future
		//would require me to seperate them
		SDL_Colour m_ColNorm, m_ColHov, m_ColClicked;


};

