#include "AddFrameWindow.h"
#include "Texture.h"
#include "AnimationClip.h"

void AddFrameWindow::Init(Texture* _IconsTexture)
{
	EditorWindow::Init(_IconsTexture);

	if (m_IconTexture != nullptr)
	{
		//in case i want to use an extra icon for something in this window
	}
}

void AddFrameWindow::ReScaleContent()
{
	m_Pos = Vector2{ 0,0 };
	m_Name = "empty window";
	Init(nullptr);
}

AddFrameWindow::AddFrameWindow(Vector2 _pos, const std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	Init(_IconsTexture);
}

AddFrameWindow::AddFrameWindow()
{
	m_Pos = Vector2{ 0,0 };
	m_Name = " Unnamed addFramwWindow";
	Init(nullptr);
}

AddFrameWindow::~AddFrameWindow()
{
}

void AddFrameWindow::Update(float _dt)
{
	EditorWindow::Update(_dt);
}

void AddFrameWindow::MouseDown(unsigned int _key)
{
	EditorWindow::MouseDown(_key);
}

void AddFrameWindow::MouseUp(unsigned int _key)
{
	EditorWindow::MouseUp(_key);
}

void AddFrameWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x, _y);
}

void AddFrameWindow::SetFont(TTF_Font* _font)
{
	EditorWindow::SetFont(_font);
}

void AddFrameWindow::Render(SDLRenderer* _renderer)
{
	EditorWindow::Render(_renderer);
}

void AddFrameWindow::SetClip(AnimationClip* _clip)
{
	m_CurrentClip = _clip;
}
