#include "Text.h"

Text::Text()
{
	m_Texture = NULL;
	m_Colour.a = 0xff;
	m_Colour.b = 0xaa;
	m_Colour.g = 0xaa;
	m_Colour.r = 0xaa;
	m_Text = "Not set text lol! ";
	m_FontPointer = NULL;

}

Text::~Text()
{
	SDL_DestroyTexture(m_Texture);
	m_Texture = NULL;
}


void Text::UpdateInterface(SDLRenderer* _renderer)
{
	//Delete old texture if any
	if (m_Texture != NULL)
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = NULL;
	}
	//Do stuff to update the 
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_FontPointer, m_Text.data(), m_Colour);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s \n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		m_Texture = SDL_CreateTextureFromSurface(_renderer->GetRenderer(), textSurface);
		if (m_Texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s \n", SDL_GetError());
		}
		else
		{
			m_SrcSize.x = textSurface->w;
			m_SrcSize.y = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);

		m_Interface.texture = m_Texture;
		m_Interface.destRect.x = m_pos.x;
		m_Interface.destRect.y = m_pos.y;
		m_Interface.destRect.w = m_Size.x;
		m_Interface.destRect.h = m_Size.y;

		m_Interface.srcRect.x = 0;
		m_Interface.srcRect.y = 0;
		m_Interface.srcRect.w = m_SrcSize.x;
		m_Interface.srcRect.h = m_SrcSize.y;


		m_Interface.renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
		m_Interface.point = { 0,0 };

	}

}



