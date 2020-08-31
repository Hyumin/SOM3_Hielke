#include "AnimationClip.h"

#include "ResourceManager.h"
#include "Texture.h"
#include "HielkMath.h"

AnimationClip::AnimationClip()
{
	m_AnimInterval = 0.5f;
	m_AnimTimer = 0.0f;
	m_CurrentIndex = 0;
	m_IsPlaying = false;
	m_Looping = false;
	m_ClipName = " ";
	m_IsFinished = false;
	m_SourceTexture = NULL;
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::Update(float _dt)
{
	if (m_IsPlaying)
	{
		//Add time to the timer
		m_AnimTimer += _dt;
		if (m_AnimTimer > m_AnimInterval)
		{
			//increment the index and check whether its bigger than the size of the array
			m_AnimTimer -= m_AnimInterval;
			m_CurrentIndex++;
			if (m_CurrentIndex >= m_SourceRects.size())
			{
				m_CurrentIndex = 0;
				//If we're not looping stop playing
				if (!m_Looping)
				{
					m_IsPlaying = false;
					m_IsFinished = true;
				}
			}
		}

	}
}

SDL_Rect& AnimationClip::GetRect()
{
	if (m_SourceRects.size() == 0)
	{
		throw std::exception(("Animation clip with filename"+m_FileName+" has no clips | Animationlip.cpp \n").c_str());
	}

	return m_SourceRects[m_CurrentIndex];
}


Vector2 AnimationClip::GetOffset()
{
	Vector2 defaultReturnValue = { 0,0 };
	if (m_UseOffsets)
	{	
		if (m_Offsets.size() > 0)
		{
			return m_Offsets[m_CurrentIndex];
		}
		else
		{
			return defaultReturnValue;
		}
	}
	return defaultReturnValue;
}

void AnimationClip::LoadClipFromFile(const std::string& _path, ResourceManager* _manager)
{
	m_SourceRects.clear();
	m_SourceTexture = nullptr;

	std::string stuff = " ";
	std::ifstream file(_path);
	m_FileName = _path;

	while (!file.eof())
	{

		file >> stuff;
		if (stuff == "Texture:")
		{
			file >> stuff;
			m_SourceTexture = _manager->GetTexture(stuff);
		}
		if (stuff == "Clip_name:")
		{
			file >> m_ClipName;
		}
		if (stuff == "SDL_rect:")
		{
			int x, y, w, h;
			file >> x;
			file >> y;
			file >> w;
			file >> h;
			SDL_Rect newRect = { x, y, w, h };
			m_SourceRects.push_back(newRect);
		}
		if (stuff == "Using_offset:")
		{
			file >> m_UseOffsets;
		}
		if (stuff == "Offset:")
		{
			Vector2 offset;
			file >> offset.x >> offset.y;
			m_Offsets.push_back(offset);
		}
		if (stuff == "is_looping:")
		{
			file >> m_Looping;
		}
		if (stuff == "anim_interval:")
		{
			file >> m_AnimInterval;
		}

	}

}

void AnimationClip::Play()
{
	m_IsPlaying = true;
	m_IsFinished = false;
}



