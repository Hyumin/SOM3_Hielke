#include "AnimationWindow.h"
#include "Texture.h"
#include "AnimationClip.h"

AnimationWindow::AnimationWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	Init(_IconsTexture);
}

AnimationWindow::AnimationWindow()
{
	m_Pos = Vector2{ 0,0 };
	m_Name = "empty window";
	Init(nullptr);
}

AnimationWindow::~AnimationWindow()
{
}

void AnimationWindow::Update(float _dt)
{
	EditorWindow::Update(_dt);

	m_Obj.m_Pos = m_ContentBox.pos + Vector2{ m_ContentBox.w / 2 - m_Obj.m_Size.x / 2,m_ContentBox.h * 0.75f - m_Obj.m_Size.y / 2 };
	
	m_FilePathTextField.m_pos = m_ContentBox.pos;
	m_IsLoopingTextField.m_pos = m_ContentBox.pos;
	m_IsLoopingTextField.m_pos.y += m_FilePathTextField.m_Size.y;


	m_EnableLooping.pos = m_IsLoopingTextField.m_pos;
	m_EnableLooping.pos.x += m_IsLoopingTextField.m_Size.x;

	m_PauseButton.pos = m_Obj.m_Pos + Vector2{ 0,m_PauseButton.h };
	m_PlayButton.pos = m_Obj.m_Pos + Vector2{ m_PlayButton.w + 10,m_PlayButton.h };
	m_LoopButton.pos = m_Obj.m_Pos + Vector2{ -(m_LoopButton.w + 10),m_LoopButton.h };

	m_LoopObject.m_Pos = m_LoopButton.pos;
	m_PlayObject.m_Pos = m_PlayButton.pos;
	m_PauseObject.m_Pos = m_PauseButton.pos;

	m_LoopObject.m_Size = { m_LoopButton.w,m_LoopButton.h };
	m_PlayObject.m_Size = { m_PlayButton.w,m_PlayButton.h };
	m_PauseObject.m_Size = { m_PauseButton.w,m_PauseButton.h };

	m_FilePathTextField.Update(_dt);


	if (m_CurrentClip != nullptr)
	{
		if (m_CurrentClip->m_IsFinished)
		{
			m_Playing = false;
		}
		m_Obj.m_RenderInterface.srcRect = m_CurrentClip->GetRect();
	}

}

void AnimationWindow::MouseDown(unsigned int _key)
{
	EditorWindow::MouseDown(_key);
	if (_key == (SDL_BUTTON_LEFT))
	{
		if (m_EnableLooping.BoxCollision(m_PauseButton, m_MousePos))
		{
			if (m_CurrentClip != nullptr)
			{
				m_Pausing = m_Pausing ? false : true;
				m_CurrentClip->m_IsPlaying = false;
				m_Playing = false;
			}
		}
		if (m_EnableLooping.BoxCollision(m_PlayButton, m_MousePos))
		{
			if (m_CurrentClip != nullptr)
			{
				m_Playing = m_Playing ? false : true;
				if (m_Playing)
				{
					m_CurrentClip->Play();
					m_Pausing = false;
				}
			}
		}
		if (m_EnableLooping.BoxCollision(m_LoopButton, m_MousePos))
		{
			if (m_CurrentClip != nullptr)
			{
				m_Looping = m_Looping ? false : true;
				m_CurrentClip->m_Looping = m_Looping;
			}

		}
		if (m_EnableLooping.BoxCollision(m_EnableLooping, m_MousePos))
		{
			if (m_CurrentClip != nullptr)
			{
				m_CurrentClip->m_Looping = m_CurrentClip->m_Looping ? false : true;
				m_Looping = m_CurrentClip->m_Looping;
			}
		}
	}
}

void AnimationWindow::MouseUp(unsigned int _key)
{
	EditorWindow::MouseUp(_key);
}

void AnimationWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x, _y);
	if (m_ScalingSize)
	{
		ReScaleContent();
	}
}


void AnimationWindow::Render(SDLRenderer* _renderer)
{
	EditorWindow::Render(_renderer);

	m_Obj.Render(_renderer, Vector2{ 0, 0 });
	m_FilePathTextField.Render(_renderer, Vector2{ 0,0 }, 1);
	m_IsLoopingTextField.Render(_renderer, Vector2{ 0,0 }, 1);

	m_PlayObject.Render(_renderer, {});
	m_LoopObject.Render(_renderer, {});
	m_PauseObject.Render(_renderer, {});


	if (m_CurrentClip != nullptr)
	{
		if (m_CurrentClip->m_Looping)
		{
			_renderer->DrawFilledBox(m_EnableLooping.pos.x, m_EnableLooping.pos.y, m_EnableLooping.w, m_EnableLooping.h, { 0,0,0,255 });
		}
		else
		{
			_renderer->DrawBox(m_EnableLooping, { 0,0,0,255 }, { 0,0 }, 1);
		}
	}

	if (m_Pausing)
	{
		_renderer->DrawBox(m_PauseButton, { 0,255,0 });
	}
	if (m_Looping)
	{
		_renderer->DrawBox(m_LoopButton, { 0,255,0 });
	}
	if (m_Playing)
	{
		_renderer->DrawBox(m_PlayButton, { 0,255,0 });
	}
}

void AnimationWindow::SetClip(AnimationClip* _clip)
{
	m_CurrentClip = _clip;

	m_FilePathTextField.SetText("FilePath: " + _clip->m_FileName);
	m_Looping = m_CurrentClip->m_Looping;


	m_Obj.m_RenderInterface.textureName = m_CurrentClip->m_SourceTexture->GetName();

}

void AnimationWindow::SetFont(TTF_Font* _font)
{
	m_TextField.SetFont(_font);
	m_FilePathTextField.SetFont(_font);
	m_IsLoopingTextField.SetFont(_font);
}

void AnimationWindow::Init(Texture* _IconsTexture)
{
	EditorWindow::Init(_IconsTexture);
	//Buttons
	m_PauseButton = BoxCollider{ 0,0,50,50 };
	m_PlayButton = BoxCollider{ 0,0,50,50 };
	m_LoopButton = BoxCollider{ 0,0,50,50 };

	m_PlayObject = Object();
	m_LoopObject = Object();
	m_PauseObject = Object();
	m_PlayObject.m_RenderInterface.srcRect = { 0,0,16,16 };
	m_LoopObject.m_RenderInterface.srcRect = { 32,16,16,16 };
	m_PauseObject.m_RenderInterface.srcRect = { 16,0,16,16 };


	m_FilePathTextField = TextField();
	m_IsLoopingTextField = TextField();
	m_Obj = Object();

	m_FilePathTextField.m_Size = Vector2{ 300,100 };
	m_FilePathTextField.SetColour(0, 0, 0, 255);
	m_FilePathTextField.SetText("Filepath:");

	m_IsLoopingTextField.SetText("Looping:");
	m_IsLoopingTextField.m_Size = Vector2(100, 30);
	m_IsLoopingTextField.SetColour(0, 0, 0, 255);

	m_EnableLooping = BoxCollider(0, 0, 20, 20);

	if (m_IconTexture != nullptr)
	{
		m_PlayObject.m_RenderInterface.textureName = _IconsTexture->GetName();
		m_LoopObject.m_RenderInterface.textureName = _IconsTexture->GetName();
		m_PauseObject.m_RenderInterface.textureName = _IconsTexture->GetName();
	}
	m_Playing = false;
	m_Looping = false;
	m_Pausing = false;
}

void AnimationWindow::ReScaleContent()
{
	float newWidth = m_ScaleOnStart.x - (m_ScaleStart.x - m_MousePos.x);
	float newHeight = m_ScaleOnStart.y - (m_ScaleStart.y - m_MousePos.y);

	if (newWidth < 100)
	{
		newWidth = 100;
	}
	if (newHeight < 100)
	{
		newHeight = 100;
	}

	m_ContentBox.w = newWidth;
	m_ContentBox.h = newHeight;

	m_Bar.w = m_ContentBox.w;
	m_CrossRelativePos.x = m_Bar.w - m_ExitButton.GetSize().x;
}
