#include "EditorWindow.h"
#include "Texture.h"
#include "AnimationClip.h"
#include <iostream>

EditorWindow::EditorWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	Init(_IconsTexture);

}

EditorWindow::EditorWindow()
{
	m_Pos = Vector2{ 0,0 };
	m_Name = "empty window";
	Init(nullptr);
}

EditorWindow::~EditorWindow()
{
	
}

void EditorWindow::Init(Texture* _IconsTexture)
{
	m_Bar = BoxCollider{ 100, 100, 300, 40 };
	m_CrossCollider = BoxCollider{ 350,100,50,40 };
	m_ContentBox = BoxCollider{ 100,140,300,500 };
	m_ContentScaler = BoxCollider{ 0,0,40,40 };

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

	m_ContentScaler.pos = m_ContentBox.pos + Vector2{ m_ContentBox.w,m_ContentBox.h };


	m_BarRelativePos = Vector2{ 0,0 };
	m_CrossRelativePos = Vector2{ 250,0 };
	m_ContentRelativePos = Vector2{ 0,40 };

	m_Dragging = false;
	m_ReadyForDelete = false;

	m_MousePos = Vector2{ 0,0 };
	m_TextField = TextField();
	m_FilePathTextField = TextField();
	m_IsLoopingTextField = TextField();
	m_Obj = Object();
	m_TextField.SetText(m_Name);
	m_TextField.m_Size = Vector2{ 250,30 };
	m_TextField.SetColour(255, 255, 255, 255);

	m_FilePathTextField.m_Size = Vector2{ 300,100 };
	m_FilePathTextField.SetColour(0, 0, 0, 255);
	m_FilePathTextField.SetText("Filepath:");
	
	m_IsLoopingTextField.SetText("Looping:");
	m_IsLoopingTextField.m_Size = Vector2(100, 30);
	m_IsLoopingTextField.SetColour(0, 0, 0, 255);

	m_EnableLooping = BoxCollider(0, 0, 20, 20);

	m_IconTexture = _IconsTexture;

	m_CrossObject = Object();
	m_CrossObject.m_RenderInterface.srcRect = { 48,16,16,16 };

	m_ContentScaleObject = Object();
	m_ContentScaleObject.m_RenderInterface.srcRect = { 16,16,16,16 };

	if (m_IconTexture != nullptr)
	{
		m_CrossObject.m_RenderInterface.textureName = _IconsTexture->GetName();
		m_ContentScaleObject.m_RenderInterface.textureName = _IconsTexture->GetName();
		m_PlayObject.m_RenderInterface.textureName = _IconsTexture->GetName();
		m_LoopObject.m_RenderInterface.textureName = _IconsTexture->GetName();
		m_PauseObject.m_RenderInterface.textureName = _IconsTexture->GetName();
	}
	m_CurrentClip = nullptr;

	m_Playing = false;
	m_Looping = false;
	m_Pausing = false;

	
}


void EditorWindow::Update(float _dt)
{
	m_Bar.pos = m_Pos + m_BarRelativePos;
	m_TextField.m_pos = m_Pos + m_BarRelativePos;
	m_CrossCollider.pos = m_Pos + m_CrossRelativePos;
	m_ContentBox.pos = m_Pos + m_ContentRelativePos;
	m_Obj.m_Pos = m_ContentBox.pos + Vector2{ m_ContentBox.w/2 - m_Obj.m_Size.x/2,m_ContentBox.h*0.75f - m_Obj.m_Size.y/2};
	m_CrossObject.m_Pos = m_CrossCollider.pos;
	m_CrossObject.m_Size = Vector2{m_CrossCollider.w,m_CrossCollider.h};
	m_ContentScaler.pos = m_ContentBox.pos + Vector2{ m_ContentBox.w-m_ContentScaler.w,m_ContentBox.h -m_ContentScaler.h};
	
	m_ContentScaleObject.m_Pos = m_ContentScaler.pos;
	m_ContentScaleObject.m_Size = Vector2{ m_ContentScaler.w,m_ContentScaler.h };

	m_FilePathTextField.m_pos = m_ContentBox.pos;
	m_IsLoopingTextField.m_pos = m_ContentBox.pos;
	m_IsLoopingTextField.m_pos.y += m_FilePathTextField.m_Size.y;


	m_EnableLooping.pos = m_IsLoopingTextField.m_pos;
	m_EnableLooping.pos.x += m_IsLoopingTextField.m_Size.x;

	m_PauseButton.pos = m_Obj.m_Pos + Vector2{0,m_PauseButton.h};
	m_PlayButton.pos = m_Obj.m_Pos + Vector2{ m_PlayButton.w+10,m_PlayButton.h };
	m_LoopButton.pos = m_Obj.m_Pos + Vector2{ -(m_LoopButton.w+10),m_LoopButton.h };

	m_LoopObject.m_Pos = m_LoopButton.pos;
	m_PlayObject.m_Pos = m_PlayButton.pos;
	m_PauseObject.m_Pos = m_PauseButton.pos;

	m_LoopObject.m_Size = { m_LoopButton.w,m_LoopButton.h };
	m_PlayObject.m_Size = { m_PlayButton.w,m_PlayButton.h };
	m_PauseObject.m_Size = { m_PauseButton.w,m_PauseButton.h};

	m_TextField.Update(_dt);
	m_FilePathTextField.Update(_dt);


	if (m_CurrentClip != nullptr)
	{
		if (m_CurrentClip->m_IsFinished)
		{
			m_Playing = false;
		}
	}

}

void EditorWindow::MouseDown(unsigned int _key)
{
	if (_key ==(SDL_BUTTON_LEFT))
	{
		if (m_Bar.BoxCollision(m_Bar, m_MousePos))
		{
			//Can drag the mouse
			m_Dragging = true;
			m_StartDragPos = m_Bar.pos -m_MousePos;

		}
		if (m_CrossCollider.BoxCollision(m_CrossCollider, m_MousePos))
		{
			//Close the window
			m_ReadyForDelete = true;
		}
		if (m_ContentScaler.BoxCollision(m_ContentScaler, m_MousePos))
		{
			m_ScalingSize = true;
			m_ScaleStart = m_MousePos;
			m_ScaleOnStart = Vector2{m_ContentBox.w, m_ContentBox.h};
		}

		if (m_CrossCollider.BoxCollision(m_PauseButton, m_MousePos))
		{
			if (m_CurrentClip != nullptr)
			{
				m_Pausing = m_Pausing ? false : true;
				m_CurrentClip->m_IsPlaying = false;
				m_Playing = false;
			}
		}
		if (m_CrossCollider.BoxCollision(m_PlayButton, m_MousePos))
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
		if (m_CrossCollider.BoxCollision(m_LoopButton, m_MousePos))
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

void EditorWindow::MouseUp( unsigned int _key)
{
	// if mouse button up draggin should be false
	if (_key == (SDL_BUTTON_LEFT))
	{
		m_Dragging = false;
		m_ScalingSize = false;
	}
}

void EditorWindow::MouseMove(unsigned int _x, unsigned int _y)
{

	m_MousePos.x = _x;
	m_MousePos.y = _y;
	//IF we're dragging the box set box pos to be the same as mousepos
	if (m_Dragging)
	{
		float x =m_StartDragPos.x;
		float y =m_StartDragPos.y;

		m_Pos.x = _x+x;
		m_Pos.y = _y+y;
	}

	if (m_ScalingSize)
	{
		ReScaleContent();
	
	}
}

void EditorWindow::ReScaleContent()
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
	m_CrossRelativePos.x = m_Bar.w - m_CrossCollider.w;

}

void EditorWindow::SetFont(TTF_Font* _font)
{
	m_TextField.SetFont(_font);
	m_FilePathTextField.SetFont(_font);
	m_IsLoopingTextField.SetFont(_font);
}

void EditorWindow::SetName(std::string& _name)
{
	m_Name = _name;
	m_TextField.SetText(m_Name);
}

void EditorWindow::SetShowingObject(Object& _obj)
{
	m_Obj = _obj;
}

void EditorWindow::Render(SDLRenderer* _renderer)
{
	_renderer->DrawFilledBox(m_Bar.pos.x,m_Bar.pos.y,m_Bar.w, m_Bar.h, { 0,153,15,255 });
	_renderer->DrawFilledBox(m_ContentBox.pos.x, m_ContentBox.pos.y, m_ContentBox.w, m_ContentBox.h / 3, { 0,160,15,255 });

	_renderer->DrawBox(m_CrossCollider, { 255,0,0,255 });
	_renderer->DrawBox(m_ContentBox);
	m_ContentScaleObject.Render(_renderer, Vector2{ 0, 0 });
	m_Obj.Render(_renderer, Vector2{ 0, 0 });
	m_TextField.Render(_renderer, Vector2{ 0,0 },1);
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
			_renderer->DrawBox(m_EnableLooping, { 0,0,0,255 }, { 0,0 },1);
		}
	}

	if (m_Pausing)
	{
		_renderer->DrawBox(m_PauseButton, {0,255,0});
	}
	if (m_Looping)
	{
		_renderer->DrawBox(m_LoopButton, { 0,255,0 });
	}
	if (m_Playing)
	{
		_renderer->DrawBox(m_PlayButton, { 0,255,0 });
	}
	//Render m_IconTexture dependant objects
	if (m_IconTexture != nullptr)
	{
		m_CrossObject.Render(_renderer, Vector2{ 0,0 }, 1);
	}

}

void EditorWindow::SetClip(AnimationClip* _clip)
{
	m_CurrentClip = _clip;

	m_FilePathTextField.SetText("FilePath: "+_clip->m_FileName);
	m_Looping = m_CurrentClip->m_Looping;
}
