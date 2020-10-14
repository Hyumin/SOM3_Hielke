#include "AddFrameWindow.h"
#include "Texture.h"
#include "AnimationClip.h"

void AddFrameWindow::Init(Texture* _IconsTexture)
{
	EditorWindow::Init(_IconsTexture);

	m_ContentBox.h = 200;

	//Init addframe button
	m_AddFrameButton = Button{};
	m_AddFrameButton.SetLayer(1);
	m_AddFrameButton.SetText(std::string("Add"));
	m_AddFrameButton.SetSize({50,25});
	m_AddFrameButton.SetCallbackFunction(std::bind(&AddFrameWindow::AddFrame, this));
	m_AddFrameButton.SetWireFrameMode();

	//Init the two arrow buttons 
	m_PrevFrameButton = Button{};
	m_NextFrameButton = Button{};

	m_PrevFrameButton.SetLayer(1);
	m_NextFrameButton.SetLayer(1);

	m_PrevFrameButton.SetSize({ 50,50 });
	m_NextFrameButton.SetSize({50,50});

	m_PrevFrameButton.SetCallbackFunction(std::bind(&AddFrameWindow::PrevFrame, this));
	m_NextFrameButton.SetCallbackFunction(std::bind(&AddFrameWindow::NextFrame, this));

	if (m_IconTexture != nullptr)
	{
		m_PrevFrameButton.SetTextureDrawModeWithSheet(m_IconTexture->GetName(), {32,48,16,16}, {16,48,16,16}, {0,48,16,16});
		m_NextFrameButton.SetTextureDrawModeWithSheet(m_IconTexture->GetName(), { 0,32,16,16 }, { 16,32,16,16 }, { 32,32,16,16 });
	}

}

void AddFrameWindow::ReScaleContent()
{
	EditorWindow::ReScaleContent();
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

void AddFrameWindow::AddFrame()
{
	
}

void AddFrameWindow::PrevFrame()
{
	printf("Prevframe pressed \n");
}

void AddFrameWindow::NextFrame()
{
	printf("Nextframe pressed \n");
}

void AddFrameWindow::Update(float _dt)
{
	EditorWindow::Update(_dt);

	m_AfOffset.x =  m_ContentBox.pos.x + m_AddFrameButton.GetSize().x;
	m_AfOffset.y = m_ContentBox.pos.y + m_AddFrameButton.GetSize().y * 2;

	m_PrevOffset.x = m_ContentBox.pos.x + m_PrevFrameButton.GetSize().x ;
	m_PrevOffset.y = m_ContentBox.pos.y + m_ContentBox.h - m_PrevFrameButton.GetSize().y;

	m_NextOffset.x = m_ContentBox.pos.x + m_ContentBox.w - m_NextFrameButton.GetSize().x*2;
	m_NextOffset.y = m_ContentBox.pos.y + m_ContentBox.h - m_NextFrameButton.GetSize().y;

	m_AddFrameButton.SetPosition(m_AfOffset);
	m_NextFrameButton.SetPosition(m_NextOffset);
	m_PrevFrameButton.SetPosition(m_PrevOffset);


}

void AddFrameWindow::MouseDown(unsigned int _key)
{
	EditorWindow::MouseDown(_key);
	m_AddFrameButton.MouseDown(_key);
	m_NextFrameButton.MouseDown(_key);
	m_PrevFrameButton.MouseDown(_key);
}

void AddFrameWindow::MouseUp(unsigned int _key)
{
	EditorWindow::MouseUp(_key);
	m_AddFrameButton.MouseUp(_key);
	m_NextFrameButton.MouseUp(_key);
	m_PrevFrameButton.MouseUp(_key);
}

void AddFrameWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x, _y);
	m_AddFrameButton.MouseMove(_x, _y);
	m_NextFrameButton.MouseMove(_x, _y);
	m_PrevFrameButton.MouseMove(_x, _y);
}

void AddFrameWindow::SetFont(TTF_Font* _font)
{
	EditorWindow::SetFont(_font);

	if (m_TextField.GetFont() == nullptr)
	{
		printf("Wtf went wrong?");
	}

	m_AddFrameButton.SetFont(_font);
}

void AddFrameWindow::Render(SDLRenderer* _renderer)
{
	EditorWindow::Render(_renderer);

	_renderer->DrawFilledBox(m_ContentBox, m_Color, { 0,0 }, 0);
	m_AddFrameButton.Render(_renderer);
	m_NextFrameButton.Render(_renderer);
	m_PrevFrameButton.Render(_renderer);
}

void AddFrameWindow::SetClip(AnimationClip* _clip)
{
	m_CurrentClip = _clip;
}
