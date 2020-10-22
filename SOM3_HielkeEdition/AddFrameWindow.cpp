#include "AddFrameWindow.h"
#include "Texture.h"
#include "AnimationClip.h"
#include "TextFieldBuilder.h"


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
	m_AddFrameButton.SetWireFrameMode({ 0,0,0,255 }, { 100,100,100,255 }, {255,255,255,255});
	m_AddFrameButton.SetTextColor({0,0,0,255});

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

	//Init input fields
	m_XInput = new InputTextField{InputTextField::InputTextMode::Numbers,"X:"};
	m_YInput = new InputTextField{ InputTextField::InputTextMode::Numbers,"Y:" };
	m_WInput = new InputTextField{ InputTextField::InputTextMode::Numbers,"W:" };
	m_HInput = new InputTextField{ InputTextField::InputTextMode::Numbers,"H:" };

	m_XInput->m_CharLimit = 5;
	m_YInput->m_CharLimit = 5;
	m_WInput->m_CharLimit = 5;
	m_HInput->m_CharLimit = 5;

	m_XInputOffset = Vector2{ 30,5 };
	m_YInputOffset = Vector2{ 175,5 };
	m_WInputOffset = Vector2{ 30,55 };
	m_HInputOffset = Vector2{ 175,55 };

	m_XInput->SetSize(Vector2{ 50,30 });
	m_YInput->SetSize(Vector2{ 50,30 });
	m_WInput->SetSize(Vector2{ 50,30 });
	m_HInput->SetSize(Vector2{ 50,30 });
	

	m_InputFields.push_back(m_XInput);
	m_InputFields.push_back(m_YInput);
	m_InputFields.push_back(m_WInput);
	m_InputFields.push_back(m_HInput);

	//init current frame field
	m_CurrentFrameField = TextField();
	m_CurrentFrameField.m_Size = Vector2{240, 30};
	m_CurrentFrameField.SetText("Current frame:");
	m_CurrentFrameField.SetColour({0,0,0,255});
	Reposition();

}

void AddFrameWindow::ReScaleContent()
{
	EditorWindow::ReScaleContent();
}

void AddFrameWindow::Reposition()
{
	EditorWindow::Reposition();
	m_AfOffset.y = m_ContentBox.pos.y + m_ContentBox.h - m_PrevFrameButton.GetSize().y;

	m_PrevOffset.x = m_ContentBox.pos.x + m_PrevFrameButton.GetSize().x;
	m_PrevOffset.y = m_ContentBox.pos.y + m_ContentBox.h - m_PrevFrameButton.GetSize().y;

	m_NextOffset.x = m_ContentBox.pos.x + m_ContentBox.w - m_NextFrameButton.GetSize().x * 2;
	m_NextOffset.y = m_ContentBox.pos.y + m_ContentBox.h - m_NextFrameButton.GetSize().y;

	m_AfOffset.x = m_PrevOffset.x + m_AddFrameButton.GetSize().x;
	m_CurrentFrameField.m_pos = m_AfOffset + Vector2{ 0,-30 };

	m_AddFrameButton.SetPosition(m_AfOffset);
	m_NextFrameButton.SetPosition(m_NextOffset);
	m_PrevFrameButton.SetPosition(m_PrevOffset);


	m_XInput->SetPosition(m_ContentBox.pos + m_XInputOffset);
	m_YInput->SetPosition(m_ContentBox.pos + m_YInputOffset);
	m_WInput->SetPosition(m_ContentBox.pos + m_WInputOffset);
	m_HInput->SetPosition(m_ContentBox.pos + m_HInputOffset);
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
	delete m_XInput;
	m_XInput = nullptr;
	delete m_YInput;
	m_YInput = nullptr;
	delete m_WInput;
	m_WInput= nullptr;
	delete m_HInput;
	m_HInput = nullptr;
}

void AddFrameWindow::AddFrame()
{
	printf("AddFrame pressed \n");
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

	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->Update(_dt);
	}


	if (m_CurrentClip != nullptr)
	{
		m_CurrentFrameField.SetText((m_ConstFrameText + std::to_string(m_CurrentClip->m_CurrentIndex).data()));
	}

}

void AddFrameWindow::MouseDown(unsigned int _key)
{
	EditorWindow::MouseDown(_key);
	m_AddFrameButton.MouseDown(_key);
	m_NextFrameButton.MouseDown(_key);
	m_PrevFrameButton.MouseDown(_key);
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->MouseDown(_key);
	}
}

void AddFrameWindow::MouseUp(unsigned int _key)
{
	EditorWindow::MouseUp(_key);
	m_AddFrameButton.MouseUp(_key);
	m_NextFrameButton.MouseUp(_key);
	m_PrevFrameButton.MouseUp(_key);
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->MouseUp(_key);
	}
}

void AddFrameWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x, _y);
	m_AddFrameButton.MouseMove(_x, _y);
	m_NextFrameButton.MouseMove(_x, _y);
	m_PrevFrameButton.MouseMove(_x, _y);
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->MouseMove(_x, _y);
	}
}

void AddFrameWindow::SetFont(TTF_Font* _font)
{
	EditorWindow::SetFont(_font);

	if (m_TextField.GetFont() == nullptr)
	{
		printf("Wtf went wrong?");
	}

	m_AddFrameButton.SetFont(_font);
	m_CurrentFrameField.SetFont(_font);
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->SetFont(_font);
	}
}

void AddFrameWindow::KeyDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->KeyDown(_key);
	}
}

void AddFrameWindow::KeyUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->KeyUp(_key);
	}
}

void AddFrameWindow::Render(SDLRenderer* _renderer)
{
	EditorWindow::Render(_renderer);

	_renderer->DrawFilledBox(m_ContentBox, m_Color, { 0,0 }, 0);
	m_AddFrameButton.Render(_renderer);
	m_NextFrameButton.Render(_renderer);
	m_PrevFrameButton.Render(_renderer);
	m_CurrentFrameField.Render(_renderer, { 0,0 }, 1);
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->Render(_renderer);
	}

}

void AddFrameWindow::SetClip(AnimationClip* _clip)
{
	m_CurrentClip = _clip;
}
