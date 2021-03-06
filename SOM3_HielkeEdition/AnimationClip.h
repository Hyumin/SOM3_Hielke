#pragma once

#include <SDL.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
class ResourceManager;
class Texture;
class Vector2;

class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();
	void Update(float _dt);
	SDL_Rect& GetRect();
	Vector2 GetOffset();
	void LoadClipFromFile(const std::string& _path,ResourceManager* _manager);
	void Play();

	std::vector<SDL_Rect> m_SourceRects;
	std::vector<Vector2> m_Offsets;
	Texture* m_SourceTexture;//what texture does this clip belong to
	unsigned int m_CurrentIndex;
	float m_AnimInterval;
	bool m_IsPlaying;
	bool m_Looping;
	bool m_IsFinished;

	std::string m_ClipName;
	std::string m_FileName;

private:
	float m_AnimTimer;
	bool m_UseOffsets = false;
	
};

