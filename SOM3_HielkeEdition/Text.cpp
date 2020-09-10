#include "Text.h"
#include <cmath>


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
	//Do stuff to update the surface
	SDL_Surface* mainSurface= SDL_CreateRGBSurface(0, (int)m_Size.x, (int)m_Size.y, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);


	if(WrapText(mainSurface,m_Text,0,0))
	{

		//Create texture from surface pixels
		m_Texture = SDL_CreateTextureFromSurface(_renderer->GetRenderer(), mainSurface);
		if (m_Texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s \n", SDL_GetError());
		}
		else
		{
			m_SrcSize.x = mainSurface->w;
			m_SrcSize.y = mainSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(mainSurface);

		m_Interface.texture = m_Texture;
		m_Interface.destRect.x = m_pos.x;
		m_Interface.destRect.y = m_pos.y;
		m_Interface.destRect.w = m_SrcSize.x;
		m_Interface.destRect.h = m_SrcSize.y;
	

		m_Interface.srcRect.x = 0;
		m_Interface.srcRect.y = 0;
		m_Interface.srcRect.w = m_SrcSize.x;
		m_Interface.srcRect.h = m_SrcSize.y;


		m_Interface.renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
		m_Interface.point = { 0,0 };

	}

}

//this will edit the mainsurface and return true
bool  Text::WrapText(SDL_Surface* _Mainsurface, std::string _remainingText, float _sizePerCharacter, int _ypos)
{

	float sizePerChar = _sizePerCharacter;

	
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_FontPointer, _remainingText.data(), m_Colour);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s \n", TTF_GetError());
		return false;
	}
	if (sizePerChar == 0)
	{
		sizePerChar = textSurface->w / _remainingText.size();
	}
;

	int height = textSurface->h;
	SDL_Rect destRect= textSurface->clip_rect;
	destRect.y = height * _ypos;

	//does it fit in the main surface?
	if (textSurface->w > _Mainsurface->w)
	{
		//Get rid of old surface
		SDL_FreeSurface(textSurface);
		int cutoffIndex = _Mainsurface->w / sizePerChar;

		std::string buffer;
		buffer.assign(_remainingText,0,  cutoffIndex);

		std::string remaining;
		//std::copy(_remainingText.begin() + cutoffIndex, _remainingText.end(), &remaining);
		remaining.assign(_remainingText,cutoffIndex-1, _remainingText.size());

		textSurface = TTF_RenderText_Solid(m_FontPointer, buffer.data(), m_Colour);

		destRect = textSurface->clip_rect;
		destRect.y = height * _ypos;
		SDL_BlitSurface(textSurface, &textSurface->clip_rect, _Mainsurface, &destRect);

		//Free the surface after we've applied it to the mainsurface
		SDL_FreeSurface(textSurface);

		//Recursive untill textsurface width will no longer exceed mainsurface width aka everything should fit
		//TODO add check for height as well, either if height exceeds don't add more or something else
		return WrapText(_Mainsurface, remaining,sizePerChar,_ypos+1);
	}
	else
	{
		SDL_BlitSurface(textSurface, &textSurface->clip_rect, _Mainsurface, &destRect);

		SDL_FreeSurface(textSurface);
	}
	return true;
}



