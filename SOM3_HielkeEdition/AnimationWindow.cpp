#include "AnimationWindow.h"
#include "Texture.h"
#include "AnimationClip.h"
#include "TextFieldBuilder.h"
#include "ButtonBuilder.h"

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
	delete m_IntervalInputField;
	m_IntervalInputField = nullptr;
	delete m_FrameX;
	m_FrameX = nullptr;
	delete m_FrameY;
	m_FrameY = nullptr;
	delete m_FrameW;
	m_FrameW = nullptr;
	delete m_FrameH;
	m_FrameH = nullptr;
	delete m_OffsetX;
	m_OffsetX = nullptr;
	delete m_OffsetY;
	m_OffsetY = nullptr;
}

void AnimationWindow::Update(float _dt)
{
	EditorWindow::Update(_dt);

	if (m_CurrentClip != nullptr)
	{
		int currIndex = m_CurrentClip->m_CurrentIndex;
		m_CurrentClip->Update(_dt * m_PlayBackSpeed);
		m_FrameCounterTextField.SetText("Frame:" + std::to_string(m_CurrentClip->m_CurrentIndex));
		m_CurrentSpeedTextField.SetText("Speed:" + std::to_string(m_PlayBackSpeed));

		//update this the moment a new frame has been loaded else when paused can be used to edit certain things OwO
		if (currIndex != m_PrevFrameIndex)
		{
			const SDL_Rect& rect = m_CurrentClip->GetRect();
			m_FrameX->SetText(std::to_string(rect.x));
			m_FrameY->SetText(std::to_string(rect.y));
			m_FrameW->SetText(std::to_string(rect.w));
			m_FrameH->SetText(std::to_string(rect.h));

			Vector2 offset = m_CurrentClip->GetOffset();
			m_OffsetX->SetText(std::to_string((int)offset.x));
			m_OffsetY->SetText(std::to_string((int)offset.y));
		}

		if (m_FrameX->m_Changed)
		{
			m_CurrentClip->m_SourceRects[currIndex].x = std::stoi(m_FrameX->GetText());
		}
		if (m_FrameY->m_Changed)
		{
			m_CurrentClip->m_SourceRects[currIndex].y = std::stoi(m_FrameY->GetText());
		}
		if (m_FrameW->m_Changed)
		{
			m_CurrentClip->m_SourceRects[currIndex].w = std::stoi(m_FrameW->GetText());
		}
		if (m_FrameH->m_Changed)
		{
			m_CurrentClip->m_SourceRects[currIndex].h = std::stoi(m_FrameH->GetText());
		}
		if (m_OffsetX->m_Changed)
		{
			m_CurrentClip->m_Offsets[currIndex].x = std::stoi(m_OffsetX->GetText());
		}
		if (m_OffsetY->m_Changed)
		{
			m_CurrentClip->m_Offsets[currIndex].y = std::stoi(m_OffsetY->GetText());
		}

		if (m_CurrentClip->m_IsFinished)
		{
			m_Playing = false;
		}
		//Vooodooo magic
		const std::string& interval = m_IntervalInputField->GetText();
		try
		{
			float i =std::stof(interval);
			m_CurrentClip->m_AnimInterval = i;
		}
		catch (std::invalid_argument& e)
		{
			printf(e.what());
		}
		
		m_Obj.m_RenderInterface.srcRect = m_CurrentClip->GetRect();
		m_Obj.m_Size = Vector2{ (float)m_CurrentClip->GetRect().w,(float)m_CurrentClip->GetRect().h };
		m_Obj.m_Size *= 2;

		m_PrevFrameIndex = m_CurrentClip->m_CurrentIndex;
	}

}

void AnimationWindow::MouseDown(unsigned int _key)
{
	EditorWindow::MouseDown(_key);

	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->MouseDown(_key);
	}
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
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->MouseUp(_key);
	}
}

void AnimationWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x, _y);
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x, _y);
	}
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->MouseMove(_x, _y);
	}
	if (m_ScalingSize)
	{
		ReScaleContent();
	}
}

void AnimationWindow::KeyDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->KeyDown(_key);
	}
}

void AnimationWindow::KeyUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->KeyUp(_key);
	}
}


void AnimationWindow::Render(SDLRenderer* _renderer)
{
	EditorWindow::Render(_renderer);

	m_Obj.Render(_renderer, Vector2{ 0, 0 },1);

	for (int i = 0; i < m_TextFields.size(); ++i)
	{
		m_TextFields[i]->Render(_renderer, { 0,0 }, 1);
	}
	for (unsigned int i =0; i <m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}
	m_PlayButton.Render(_renderer);
	m_LoopButton.Render(_renderer);
	m_PauseButton.Render(_renderer);

	_renderer->DrawFilledBox(m_BottomContentBox, m_Color, { 0,0 }, 0);
	_renderer->DrawFilledBox(m_RawPreviewBox, { 0x0, 0x8D, 0xAD, 0xff }, { 0,0 }, 0);
	_renderer->DrawFilledBox(m_InGamePreviewBox, { 0x0, 0x8D, 0xAD, 0xff }, { 0,0 }, 0);
	_renderer->DrawFilledBox(m_TopContentBox, m_Color, { 0,0 }, 0);
	_renderer->DrawFilledBox(m_EditFrameBox, m_Color);
	
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->Render(_renderer);
	}
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
		if (m_CurrentClip->m_UseOffsets)
		{
			_renderer->DrawFilledBox(m_ToggleOffset.GetCollider(), { 0,0,0,255 });
		}

	}
	if (m_Pausing)
	{
		_renderer->DrawBox(m_PauseButton.GetCollider(), m_LightColor, { 0,0 }, 1);
	}
	if (m_Looping)
	{
		_renderer->DrawBox(m_LoopButton.GetCollider(), m_LightColor, { 0,0 },1);
	}
	if (m_Playing)
	{
		_renderer->DrawBox(m_PlayButton.GetCollider(), m_LightColor, { 0,0 }, 1);
	}
}

void AnimationWindow::SetClip(AnimationClip* _clip)
{
	m_CurrentClip = _clip;

	m_FilePathTextField.SetText("FilePath: " + _clip->m_FileName);
	m_Looping = m_CurrentClip->m_Looping;


	m_Obj.m_RenderInterface.textureName = m_CurrentClip->m_SourceTexture->GetName();
	m_IntervalInputField->SetText(std::to_string(m_CurrentClip->m_AnimInterval));
	const SDL_Rect& rect =m_CurrentClip->GetRect();
	m_FrameX->SetText(std::to_string(rect.x));
	m_FrameY->SetText(std::to_string(rect.y));
	m_FrameW->SetText(std::to_string(rect.w));
	m_FrameH->SetText(std::to_string(rect.h));
	Vector2 offset = m_CurrentClip->m_Offsets[m_CurrentClip->m_CurrentIndex];
	m_OffsetX->SetText(std::to_string(offset.x));
	m_OffsetY->SetText(std::to_string(offset.y));

	printf("how many times does this happen exactly? \n");

}

void AnimationWindow::SetFont(TTF_Font* _font)
{
	EditorWindow::SetFont(_font);
	for (int i = 0; i < m_TextFields.size(); ++i)
	{
		m_TextFields[i]->SetFont(_font);
	}
	for (int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->SetFont(_font);
	}
}

void AnimationWindow::Init(Texture* _IconsTexture)
{
	EditorWindow::Init(_IconsTexture);
	
	m_PlayBackSpeed = 1.0f;
	m_Obj = Object();
	//Initialize textfields
	m_FilePathTextField= TextFieldBuilder::BuildTextField({ 0,0,0,255 }, "Filepath:", nullptr, { 0,0 }, { 300,70 });
	m_IsLoopingTextField = TextFieldBuilder::BuildTextField({ 0,0,0,255 }, "Looping:", nullptr, { 0,0 }, { 100,30 });
	m_FrameCounterTextField = TextFieldBuilder::BuildTextField({ 255,255,255,255 }, "Frame:", nullptr, { 0,0 }, { 80,20 });
	m_CurrentSpeedTextField = TextFieldBuilder::BuildTextField({ 255,255,255,255 }, "Speed:", nullptr, { 0,0 }, { 80,20 });
	m_RawText = TextFieldBuilder::BuildTextField({ 255,255,255,255 }, "Raw:", nullptr, { 0,0 }, { 80,20 });
	m_InGameText = TextFieldBuilder::BuildTextField({ 255,255,255,255 }, "In-Game:", nullptr, { 0,0 }, { 80,20 });
	m_EnableOffsetTextField = TextFieldBuilder::BuildTextField({ 0,0,0,255 }, "Enable Offsets:", nullptr, { 0,0 }, { 100,20 });

	m_TextFields.push_back(&m_EnableOffsetTextField);
	m_TextFields.push_back(&m_FilePathTextField);
	m_TextFields.push_back(&m_IsLoopingTextField);
	m_TextFields.push_back(&m_FrameCounterTextField);
	m_TextFields.push_back(&m_CurrentSpeedTextField);
	m_TextFields.push_back(&m_RawText);
	m_TextFields.push_back(&m_InGameText);

	//Define  sizes of content boxes( the green boxes )
	m_BottomContentBox = Box{ 0,m_ContentBox.pos.y+m_ContentScaleObject.m_Size.y- m_Bar.h,m_ContentBox.w,m_ContentBox.h/5 };
	m_TopContentBox = Box{m_ContentBox.pos.x,m_ContentBox.pos.y,m_ContentBox.w,m_ContentBox.h/3};
	m_InGamePreviewBox = Box{};
	m_RawPreviewBox = Box{};
	
	//Initialize buttons,
	m_EnableLooping = Box(0, 0, 20, 20);

	m_PlayButton = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 },1,std::bind(&AnimationWindow::PlayClip,this));
	m_PauseButton = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::PauseClip, this));
	m_LoopButton = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::LoopClip, this));
	m_FastForward = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::SpeedUp, this));
	m_SlowDown = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::SlowDown, this));
	m_NextFrame = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::NextFrame, this));
	m_PrevFrame = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::PrevFrame, this));
	m_ToggleOffset = ButtonBuilder::BuildButton({ 0,0 }, { 20,20 }, 1, std::bind(&AnimationWindow::ToggleOffset, this));

	m_ToggleOffset.SetWireFrameMode({0,0,0,255}, {100,100,100,255}, {255,255,255});

	m_ButtonsOffset = Vector2{ m_TopContentBox.w / 2 -(m_PlayButton.GetSize().x*1.5f),20 };

	if (m_IconTexture != nullptr)
	{
		SDL_Rect norm= { 0,0,16,16 };
		SDL_Rect hovered = { 64,0,16,16 };
		SDL_Rect clicked = {112,0,16,16};
		m_PlayButton.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered);
		
		norm= { 32,16,16,16 };
		hovered = { 96,0,16,16 };
		clicked= { 144,0,16,16 };
		m_LoopButton.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered);

		norm = { 16,0,16,16 };
		hovered = { 80,0,16,16 };
		clicked= { 128,0,16,16 };
		m_PauseButton.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered);

		norm = { 0,32,16,16 };
		hovered = { 16,32,16,16 };
		clicked = { 32,32,16,16 };
		m_NextFrame.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered);
		m_PrevFrame.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered,SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

		norm = { 48,32,16,16 };
		hovered = { 64,32,16,16 };
		clicked = { 80,32,16,16 };
		m_FastForward.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered);
		m_SlowDown.SetTextureDrawModeWithSheet(_IconsTexture->GetName(), norm, clicked, hovered, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

	}
	m_Playing = false;
	m_Looping = false;
	m_Pausing = false;

	m_Buttons.push_back(&m_PlayButton);
	m_Buttons.push_back(&m_PauseButton);
	m_Buttons.push_back(&m_LoopButton);	
	m_Buttons.push_back(&m_FastForward);
	m_Buttons.push_back(&m_SlowDown);
	m_Buttons.push_back(&m_NextFrame);
	m_Buttons.push_back(&m_PrevFrame);
	m_Buttons.push_back(&m_ToggleOffset);

	//Initialize input text field
	m_IntervalInputField = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "Interval", nullptr,
			{ 0,0 }, { 70,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });

	m_IntervalInputField->m_NameOffset = Vector2{ -60,0 };

	//What a fricking mess :eyes:
	m_FrameX = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "X", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_FrameY = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "Y", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_FrameW = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "W", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_FrameH = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "H", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_OffsetX = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "X", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_OffsetY = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "Y", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });


	m_InputTextFields.push_back(m_FrameX);
	m_InputTextFields.push_back(m_FrameY);
	m_InputTextFields.push_back(m_FrameW);
	m_InputTextFields.push_back(m_FrameH);
	m_InputTextFields.push_back(m_OffsetX);
	m_InputTextFields.push_back(m_OffsetY);

	m_InputTextFields.push_back(m_IntervalInputField);

	m_RawPreviewBox.w = m_ContentBox.w / 2;
	m_RawPreviewBox.h = m_ContentBox.h / 4;
	m_InGamePreviewBox.w = m_ContentBox.w / 2;
	m_InGamePreviewBox.h = m_ContentBox.h / 4;

	m_TopContentBox.w = m_ContentBox.w;
	m_TopContentBox.h = m_ContentBox.h / 4;
	m_EditFrameBox.w = m_ContentBox.w;
	m_EditFrameBox.h = m_ContentBox.h / 4;

	m_BottomContentBox.w = m_ContentBox.w;
	m_BottomContentBox.h = m_ContentBox.h - m_TopContentBox.h - m_InGamePreviewBox.h - m_EditFrameBox.h;

	Reposition();
	m_PrevFrameIndex = -1;
	
}

void AnimationWindow::ReScaleContent()
{
	EditorWindow::ReScaleContent();

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

	m_RawPreviewBox.w = m_ContentBox.w / 2;
	m_RawPreviewBox.h = m_ContentBox.h / 4;
	m_InGamePreviewBox.w = m_ContentBox.w / 2;
	m_InGamePreviewBox.h = m_ContentBox.h / 4;

	//Todo fix this so that when we rescale the width will be adjusted yo
	//m_FilePathTextField.m_Size.x= newWidth;

	m_TopContentBox.w = m_ContentBox.w;
	m_TopContentBox.h = m_ContentBox.h / 4;
	m_EditFrameBox.w = m_ContentBox.w;
	m_EditFrameBox.h = m_ContentBox.h / 4;

	m_BottomContentBox.w = m_ContentBox.w;
	m_BottomContentBox.h = m_ContentBox.h - m_TopContentBox.h - m_InGamePreviewBox.h- m_EditFrameBox.h;

	m_ButtonsOffset = Vector2{ m_TopContentBox.w / 2 - (m_PlayButton.GetSize().x * 1.5f),20 };
}

void AnimationWindow::Reposition()
{
	EditorWindow::Reposition();

	//Top part
	m_TopContentBox.pos = m_ContentBox.pos;
	m_FilePathTextField.m_pos = m_TopContentBox.pos;
	m_IsLoopingTextField.m_pos = m_TopContentBox.pos;
	m_IsLoopingTextField.m_pos.y += m_FilePathTextField.m_Size.y;

	m_EnableLooping.pos = m_IsLoopingTextField.m_pos;
	m_EnableLooping.pos.x += m_IsLoopingTextField.m_Size.x;
	m_IntervalInputField->SetPosition(m_IsLoopingTextField.m_pos + Vector2{ m_IntervalInputField->m_NameOffset.x*-1,m_IsLoopingTextField.m_Size.y });

	m_EnableOffsetTextField.m_pos = m_EnableLooping.pos + Vector2{ m_EnableLooping.w ,0};
	m_ToggleOffset.SetPosition(m_EnableOffsetTextField.m_pos + Vector2{ m_EnableOffsetTextField.m_Size.x,0 });

	//The edit framebox position
	m_EditFrameBox.pos = m_TopContentBox.pos + Vector2{0,m_TopContentBox.h};
	//God please forgive me 
	m_FrameX->SetPosition(m_EditFrameBox.pos + m_FrameX->m_NameOffset*-1);
	m_FrameY->SetPosition(m_FrameX->GetPosition() + Vector2{ m_FrameX->GetSize().x * 2,0 });
	
	m_FrameW->SetPosition(m_EditFrameBox.pos + Vector2{ 0,m_FrameX->GetSize().y } + m_FrameX->m_NameOffset * -1);
	m_FrameH->SetPosition(m_FrameW->GetPosition() + Vector2{ m_FrameW->GetSize().x * 2,0 });

	m_OffsetX->SetPosition(m_FrameY->GetPosition() + Vector2{ m_FrameW->GetSize().x*2,0 });
	m_OffsetY->SetPosition(m_FrameY->GetPosition() + Vector2{ m_FrameH->GetSize().x*4,0 });



	//The two preview panels part
	m_InGamePreviewBox.pos = m_EditFrameBox.pos + Vector2{ 0,m_EditFrameBox.h };
	m_RawPreviewBox.pos = m_EditFrameBox.pos + Vector2{ m_InGamePreviewBox.w,m_EditFrameBox.h };

	m_InGameText.m_pos = m_InGamePreviewBox.pos;
	m_RawText.m_pos = m_RawPreviewBox.pos;

	m_Obj.m_Pos = m_RawPreviewBox.pos + Vector2{m_RawPreviewBox.w/2,m_RawPreviewBox.h/2} - m_Obj.m_Size;

	//Bottom part
	m_BottomContentBox.pos.x = m_ContentBox.pos.x;
	m_BottomContentBox.pos.y = m_InGamePreviewBox.pos.y + m_InGamePreviewBox.h;

	m_FrameCounterTextField.m_pos = m_BottomContentBox.pos + Vector2{ m_BottomContentBox.w/4,m_BottomContentBox.h/2 };
	m_CurrentSpeedTextField.m_pos = m_FrameCounterTextField.m_pos + Vector2{m_FrameCounterTextField.m_Size.x,0};

	//Reposition bottom buttons
	m_PauseButton.SetPosition(m_BottomContentBox.pos + m_ButtonsOffset + Vector2{ m_LoopButton.GetSize().x,0 });
	m_PlayButton.SetPosition(m_BottomContentBox.pos + m_ButtonsOffset + Vector2{ m_PauseButton.GetSize().x * 2,0 });
	m_LoopButton.SetPosition(m_BottomContentBox.pos + m_ButtonsOffset);
	m_SlowDown.SetPosition(m_LoopButton.GetPosition() + Vector2{-m_SlowDown.GetSize().x,0});
	m_FastForward.SetPosition(m_PlayButton.GetPosition() + Vector2{ m_FastForward.GetSize().x,0 });
	m_PrevFrame.SetPosition(m_SlowDown.GetPosition() + Vector2{0,m_PrevFrame.GetSize().y});
	m_NextFrame.SetPosition(m_FastForward.GetPosition() + Vector2{ 0,m_NextFrame.GetSize().y });

	

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

void AnimationWindow::NextFrame()
{
	if (m_CurrentClip != nullptr)
	{
		m_CurrentClip->NextFrame();
	}
}

void AnimationWindow::PrevFrame()
{
	if (m_CurrentClip != nullptr)
	{
		m_CurrentClip->PrevFrame();
	}
}

void AnimationWindow::SpeedUp()
{
	m_PlayBackSpeed += 0.1f;
}

void AnimationWindow::SlowDown()
{
	if (m_PlayBackSpeed > 0.1f)
	{
		m_PlayBackSpeed -= 0.1f;
	}
}

void AnimationWindow::ToggleOffset()
{
	if (m_CurrentClip != nullptr)
	{
		m_CurrentClip->m_UseOffsets = m_CurrentClip->m_UseOffsets ? false : true;
	}
}
