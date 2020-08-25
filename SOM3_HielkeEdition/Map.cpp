#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Enemy.h"
#include "Rabite.h"
#include "FloatingText.h"
#include "ResourceManager.h"
#include "AnimationClip.h"

using namespace std;
using namespace Hielke;

//global to not constantly create a new map struct
ConnectedMap emptMapReturn;

Hielke::Map::Map()
{
	m_MapName = "";
	m_VisalName = "";
	m_ColliderMapName = "";
	m_Walls.clear();
	m_ConnectedMaps.clear();
	m_BackGround = new Object();

	emptMapReturn.mapName = "None";
}

Hielke::Map::~Map()
{
	delete m_BackGround;
}

void Hielke::Map::Update(float _dt)
{
	auto it = m_Enemies.begin();
	while (it != m_Enemies.end())
	{
		it[0]->Update(_dt);
		if (it[0]->m_Dead)
		{
			it = m_Enemies.erase(it);
		}
		else
		{
			++it;
		}
	}



	for (uint32_t i = 0; i < m_FloatingTexts.size(); ++i)
	{
		m_FloatingTexts[i]->Update(_dt);
		if (m_FloatingTexts[i]->m_MarkedForRemoval)
		{
			delete m_FloatingTexts[i];
			m_FloatingTexts[i] = nullptr;
			m_FloatingTexts.erase(m_FloatingTexts.begin() + i);
		}
	}
	//Aka no enemies
	if (m_Enemies.size() == 0)
	{
		for (int i = 0; i < m_Players.size(); ++i)
		{
			m_Players[i]->SetCombatState(false);
		}
	}
}

void Hielke::Map::Render(SDLRenderer* _renderer,Vector2 _worldPos)
{
	m_BackGround->Render(_renderer,_worldPos);
	SDL_Color col = { 0xff,0x00,0x00,0xff };
	for (uint32_t i = 0; i < m_ConnectedMaps.size(); ++i)
	{
		_renderer->DrawBox(m_ConnectedMaps[i].collider, col, _worldPos);
	}
	for (uint32_t i = 0; i < m_FloatingTexts.size(); ++i)
	{
		m_FloatingTexts[i]->Render(_renderer, _worldPos);
	}
	for (auto it = m_Enemies.begin(); it != m_Enemies.end(); ++it)
	{
		it[0]->Render(_renderer,_worldPos);
	}
}

void Hielke::Map::SaveMap(const std::string& _filepath)
{

}

void Hielke::Map::LoadMap(const std::string& _filePath, ResourceManager* _res)
{
	ifstream file(_filePath);

	if (file.bad())
	{
		throw exception("Filepath bad");
	}

	//While not in end of file
	while (!file.eof())
	{
		string word;
		file >> word;
		//The name given to the map
		if (word == "Map")
		{
			printf("Found the map word \n");
			char name[256];
			file.getline(name,256);
			m_MapName = string(name);
			if (m_MapName[0] == ' ');
			{
				m_MapName.erase(m_MapName.begin());
			}
			cout << m_MapName << endl;
		}
		//Visual file name
		if (word == "VisualFile")
		{
			printf("Found the VisualFile word \n");
			char name[256];
			file.getline(name, 256);
			m_VisalName = string(name);
			if (m_VisalName[0] == ' ');
			{
				m_VisalName.erase(m_VisalName.begin());
			}
			cout << m_VisalName << endl;
		}
		//Collidermask file
		if (word == "ColliderFile")
		{
			printf("Found the VisualFile word \n");
			char name[256];
			file.getline(name, 256);
			m_ColliderMapName = string(name);
			if (m_ColliderMapName[0] == ' ');
			{
				m_ColliderMapName.erase(m_ColliderMapName.begin());
			}
			cout << m_ColliderMapName << endl;
		}
		if (word == "ConnectedMap")
		{
			printf("Found the VisualFile word \n");
			char name[256];
			file.getline(name, 256);
			string newString;
			newString = string(name);
			if (newString[0] == ' ');
			{
				newString.erase(newString.begin());
			}
			cout << newString << endl;

			ConnectedMap newMap;
			newMap.mapName = newString;
			file >> word;
			file >> word >> newMap.fileName;;

			BoxCollider b;
			file >> word;
			file>>b.pos.x;
			file >> word;
			file >> b.pos.y;
			file >> word;
			file >> b.w;
			file >> word;
			file >> b.h;
			newMap.collider = b;
			file >> word >>word>> newMap.startPos.x >>word>> newMap.startPos.y;

			m_ConnectedMaps.push_back(newMap);
		}
		if (word == "MapObject")
		{
			SDL_Rect srcRect;
			SDL_Rect destRect;

			file >> word;
			file >> word;
			file >> word>>destRect.x>>word>>destRect.y;
			file >> word >> destRect.w>> word >> destRect.h;
			file >> word;
			file >> word >> srcRect.x >> word >> srcRect.y;
			file >> word >> srcRect.w >> word >> srcRect.h;

			m_BackGround->m_RenderInterface.srcRect = srcRect;
			m_BackGround->m_RenderInterface.destRect = destRect;
			//assumes destrect is an accurate representation of our position etc.
			m_BackGround->m_Pos = Vector2(destRect.x, destRect.y);
			m_BackGround->m_Size = Vector2(destRect.w, destRect.h);
			m_BackGround->m_RenderInterface.point = SDL_Point{ 0,0 };

		}
		if (word == "Enemy")
		{
			std::string typeString;
			file >> typeString >> typeString >> typeString;

			std::string fileName;
			file >> fileName >> fileName;

			float x, y;
			int level;
			file >> word>>x>>word>>y>>word>>level;
			
			//Get a copy of enemy named typestring
			Rabite* enem = dynamic_cast<Rabite*>(_res->GetEnemy(fileName));
			Rabite* newEnem = new Rabite(enem->m_Object);
			newEnem->m_Stats = enem->m_Stats;
			newEnem->m_Pos.x = x;
			newEnem->m_Pos.y = y;
			std::map<Enemy::Direction, AnimationClip> idles = enem->GetIdleAnimMap();
			std::map<Enemy::Direction, AnimationClip> move = enem->GetMoveAnimMap();
			std::map<Enemy::Direction, AnimationClip> attack = enem->GetAttacAnimMap();
			newEnem->SetAttackAnimations(attack[Enemy::NORTH], attack[Enemy::EAST], attack[Enemy::SOUTH]);
			newEnem->SetIdleAnimations(idles[Enemy::NORTH], idles[Enemy::EAST], idles[Enemy::SOUTH]);
			newEnem->SetMoveAnimations(move[Enemy::NORTH], move[Enemy::EAST], move[Enemy::SOUTH]);

			newEnem->SetMap(this);
			m_Enemies.push_back(newEnem);

		}
	}
	file.close();

	//did we find an image construct an object out of it.
	if (m_VisalName.size() > 0)
	{
		m_BackGround->m_RenderInterface.textureName = m_VisalName;
	}

	//Did we find a string thats bigger than size 0? do stuff with it
	if (m_ColliderMapName.size() > 0)
	{
		LoadCollidersFromFile(m_ColliderMapName,m_BackGround);
	}


}

void Hielke::Map::LoadCollidersFromFile(const std::string& _filePath, const Object* _obj)
{
	//Instead of the usual shit we'll load a sdl_surface

	SDL_Surface* loadedSurface = IMG_Load(_filePath.c_str());

	SDL_Rect srcRect = m_BackGround->m_RenderInterface.srcRect;

	//Calculate the aspect ratio
	Vector2 aspectRatio = _obj->m_Size;
	aspectRatio.x /= srcRect.w;
	aspectRatio.y /= srcRect.h;

	//Figure out the following what format every pixel is and iterate throuhg the whole
	//Texture and check whether the pixel is equal to white if so create a bounding box

	SDL_PixelFormat* fmt = loadedSurface->format;

	std::cout << "\n Bits per pixel:" << (int)fmt->BitsPerPixel << std::endl;

	uint32_t bpp = loadedSurface->format->BytesPerPixel;
	SDL_LockSurface(loadedSurface);

	uint32_t* pixels = (uint32_t*)loadedSurface->pixels;

	int xStart = -1;
	int yStart = -1;
	Uint32 pitch = loadedSurface->pitch / 4;
	for (uint32_t y = srcRect.y; y < srcRect.y + srcRect.h; ++y)
	{
		uint32_t pixel, temp;

		for (uint32_t x = srcRect.x; x < srcRect.w + srcRect.x; ++x)
		{
			//this is wrong try to fix it 
			pixel = pixels[(y * pitch) + x];

			Uint8 r;
			/* Get Red component */
			temp = pixel & fmt->Rmask;  /* Isolate red component */
			temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
			temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
			r = (Uint8)temp;

			//We assume the texture is greyscale value
			//And we make the assumption that, white which is 255 in all 3 channels
			// is a collider so we use that to get start position of the box
			if (r == UINT8_MAX || r == 55)
			{
				if (xStart == -1)
				{
					xStart = x;
				}
				if (yStart == -1)
				{
					yStart = y;
				}

			}
			//once we encounter a black pixel we'll stop processing the current x and y position and place a collider
			if (r == 0)
			{
				if (xStart >= 0)
				{
					//Create a box collider

					int w = std::abs((xStart - (int)x)) + 1;
					int h = std::abs((yStart - (int)y)) + 1;
					//We found a black pixel on the new line!
					if (x == srcRect.x)
					{
						//Use our length instead srcRect.w+srcRect.x
						w = std::abs(xStart - (srcRect.w + srcRect.x)) + 1;
					}
					BoxCollider box = BoxCollider{ ((float)xStart - (float)srcRect.x) * aspectRatio.x,((float)yStart - srcRect.y) * aspectRatio.y,aspectRatio.x * (float)w,aspectRatio.y * (float)h };

					m_Walls.push_back(box);
					xStart = -1;
					yStart = -1;
				}
			}

		}
	}
	SDL_UnlockSurface(loadedSurface);

	SDL_FreeSurface(loadedSurface);
}

Object* Hielke::Map::GetBackground()
{
	return m_BackGround;
}



ConnectedMap& Hielke::Map::GetConnectedMap(BoxCollider& _playerCollider)
{
	for (uint32_t i = 0; i < m_ConnectedMaps.size(); ++i)
	{
		if (_playerCollider.BoxCollision(_playerCollider,m_ConnectedMaps[i].collider))
		{
			cout << "UwU returning map: " << m_ConnectedMaps[i].mapName << endl;
			return m_ConnectedMaps[i];
		}

	}

	return emptMapReturn;
}

//Returns the colliders input collider has hit 
HitResult Hielke::Map::CheckMapCollision(BoxCollider& _collider)
{

	HitResult result;
	
	for (uint32_t i = 0; i < m_Enemies.size(); ++i)
	{
		if (_collider.BoxCollision(m_Enemies[i]->m_Collider, _collider))
		{
			result.enemies.push_back(m_Enemies[i]);
		}
	}

	for (uint32_t i = 0; i < m_Players.size(); ++i)
	{
		if (_collider.BoxCollision(m_Players[i]->m_Collider, _collider))
		{
			result.players.push_back(m_Players[i]);
		}
	}



	return result;
}

void Hielke::Map::SetBackGround(Object* _background)
{
	m_BackGround = _background;
}

void Hielke::Map::AssignPlayer(PlayerCharacter* _player)
{
	m_Players.push_back(_player);

	//If enemies are assinged here
	for (uint32_t i = 0; i < m_Enemies.size(); ++i)
	{
		m_Enemies[i]->SetPlayerTarget(_player);
	}


}

void Hielke::Map::AssignEnemy(Enemy* _enem)
{
	m_Enemies.push_back(_enem);
}
void Hielke::Map::AddFloatingText(Vector2 _pos, SDL_Color _col, float _duration,float _speed, const std::string& _text)
{
	FloatingText* ft = new FloatingText(_duration, _speed, _text);
	ft->m_pos = _pos;
	ft->SetColour(_col);
	ft->m_Size = Vector2(30, 30);
	ft->SetFont(m_DefaultFont);

	m_FloatingTexts.push_back(ft);

}

void Hielke::Map::SetDefaultFont(TTF_Font* _defaultFont)
{
	m_DefaultFont = _defaultFont;
}
