#include "EditorWindow.h"
#include "Texture.h"
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

	m_ContentScaler.pos = m_ContentBox.pos + Vector2{ m_ContentBox.w,m_ContentBox.h };


	m_BarRelativePos = Vector2{ 0,0 };
	m_CrossRelativePos = Vector2{ 250,0 };
	m_ContentRelativePos = Vector2{ 0,40 };

	m_Dragging = false;
	m_ReadyForDelete = false;

	m_MousePos = Vector2{ 0,0 };
	m_TextField = TextField();
	m_Obj = Object();
	m_TextField.SetText(m_Name);
	m_TextField.m_Size = Vector2{ 180,30 };
	m_TextField.SetColour(255, 255, 255, 255);
	m_IconTexture = _IconsTexture;

	m_CrossObject = Object();
	m_CrossObject.m_RenderInterface.srcRect = { 48,16,16,16 };

	m_ContentScaleObject = Object();
	m_ContentScaleObject.m_RenderInterface.srcRect = { 16,16,16,16 };

	if (m_IconTexture != nullptr)
	{
		m_CrossObject.m_RenderInterface.textureName = _IconsTexture->GetName();
		m_ContentScaleObject.m_RenderInterface.textureName = _IconsTexture->GetName();
	}
}


void EditorWindow::Update(float _dt)
{
	m_Bar.pos = m_Pos + m_BarRelativePos;
	m_TextField.m_pos = m_Pos + m_BarRelativePos;
	m_CrossCollider.pos = m_Pos + m_CrossRelativePos;
	m_ContentBox.pos = m_Pos + m_ContentRelativePos;
	m_Obj.m_Pos = m_ContentBox.pos;
	m_CrossObject.m_Pos = m_CrossCollider.pos;
	m_CrossObject.m_Size = Vector2{m_CrossCollider.w,m_CrossCollider.h};
	m_ContentScaler.pos = m_ContentBox.pos + Vector2{ m_ContentBox.w-m_ContentScaler.w,m_ContentBox.h -m_ContentScaler.h};
	
	m_ContentScaleObject.m_Pos = m_ContentScaler.pos;
	m_ContentScaleObject.m_Size = Vector2{ m_ContentScaler.w,m_ContentScaler.h };

	m_TextField.Update(_dt);
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
	_renderer->DrawBox(m_CrossCollider, { 255,0,0,255 });
	_renderer->DrawBox(m_ContentBox);
	m_ContentScaleObject.Render(_renderer, Vector2{ 0, 0 });
	m_Obj.Render(_renderer, Vector2{ 0, 0 });
	m_TextField.Render(_renderer, Vector2{ 0,0 },1);

	//Render m_IconTexture dependant objects
	if (m_IconTexture != nullptr)
	{
		m_CrossObject.Render(_renderer, Vector2{ 0,0 }, 1);
	}

}
