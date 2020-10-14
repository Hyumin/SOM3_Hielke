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

	//Todo clean this messy shit lmao
	m_PauseButton.SetPosition(m_Obj.m_Pos + Vector2{0,m_PauseButton.GetSize().y});
	m_PlayButton.SetPosition(m_Obj.m_Pos + Vector2{ m_PlayButton.GetSize().x + 10,m_PlayButton.GetSize().y });
	m_LoopButton.SetPosition(m_Obj.m_Pos + Vector2{ -(m_LoopButton.GetSize().x + 10),m_LoopButton.GetSize().y });


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

	m_PlayButton.MouseDown(_key);
	m_LoopButton.MouseDown(_key);
	m_PauseButton.MouseDown(_key);

	if (_key == (SDL_BUTTON_LEFT))
	{
		if (m_EnableLooping.BoxCollision(m_EnableLooping, m_MousePos))
		{
			LoopClip();
		}
	}
}

void AnimationWindow::MouseUp(unsigned int _key)
{
	EditorWindow::MouseUp(_key);
	m_PlayButton.MouseUp(_key);
	m_LoopButton.MouseUp(_key);
	m_PauseButton.MouseUp(_key);
}

void AnimationWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x, _y);
	m_PlayButton.MouseMove(_x,_y);
	m_LoopButton.MouseMove(_x, _y);
	m_PauseButton.MouseMove(_x, _y);
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

	m_PlayButton.Render(_renderer);
	m_LoopButton.Render(_renderer);
	m_PauseButton.Render(_renderer);

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
		_renderer->DrawBox(m_PauseButton.GetCollider(), { 0,255,0 });
	}
	if (m_Looping)
	{
		_renderer->DrawBox(m_LoopButton.GetCollider(), { 0,255,0 });
	}
	if (m_Playing)
	{
		_renderer->DrawBox(m_PlayButton.GetCollider(), { 0,255,0 });
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

	m_PlayButton = Button{};
	m_PauseButton = Button{};
	m_LoopButton = Button{};

	m_PlayButton.SetSize(Vector2{ 50,50 });
	m_LoopButton.SetSize(Vector2{ 50,50 });
	m_PauseButton.SetSize(Vector2{ 50,50 });

	m_PlayButton.SetLayer(1);
	m_LoopButton.SetLayer(1);
	m_PauseButton.SetLayer(1);


	m_PlayButton.SetCallbackFunction(std::bind(&AnimationWindow::PlayClip, this));
	m_LoopButton.SetCallbackFunction(std::bind(&AnimationWindow::LoopClip, this));
	m_PauseButton.SetCallbackFunction(std::bind(&AnimationWindow::PauseClip, this));


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
		//std::string& tex = _IconsTexture->GetName();
		SDL_Rect norm, hovered, clicked;

		norm= { 0,0,16,16 };
		hovered = { 64,0,16,16 };
		clicked = {112,0,16,16};

		m_PlayButton.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered);
		norm= { 32,16,16,16 };
		hovered = { 96,0,16,16 };
		clicked= { 144,0,16,16 };
		m_LoopButton.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered);

		norm = { 16,0,16,16 };
		hovered = { 80,0,16,16 };
		clicked= { 128,0,16,16 };
		m_PauseButton.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered);

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

void AnimationWindow::PlayClip()
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

void AnimationWindow::LoopClip()
{
	if (m_CurrentClip != nullptr)
	{
		m_Looping = m_Looping ? false : true;
		m_CurrentClip->m_Looping = m_Looping;
	}
}

void AnimationWindow::PauseClip()
{
	if (m_CurrentClip != nullptr)
	{
		m_Pausing = m_Pausing ? false : true;
		m_CurrentClip->m_IsPlaying = false;
		m_Playing = false;
	}
}
