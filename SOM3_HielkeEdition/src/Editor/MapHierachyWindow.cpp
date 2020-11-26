#include "MapHierachyWindow.h"
#include "..\Engine\ButtonBuilder.h"
#include "..\Engine\TextFieldBuilder.h"
#include "..\Engine\Texture.h"
#include <string>

MapHierachyWindow::MapHierachyWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	m_Map = nullptr;
	Init(_IconsTexture);
}

MapHierachyWindow::MapHierachyWindow()
{
	m_Pos = { 0,0 };
	m_Name = "Defaultname";
	m_Map = nullptr;
	Init(nullptr);
}

MapHierachyWindow::~MapHierachyWindow()
{
	MapEditorWindow::~MapEditorWindow();
	m_Map = nullptr;
	m_InputTexts.clear();
	m_Buttons.clear();
	delete m_ScrollBarObj;
	m_WallButtons.clear();
}

void MapHierachyWindow::Update(float _dt)
{
	MapEditorWindow::Update(_dt);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->Update(_dt);
	}
	switch (m_Mode)
	{
	case MapHierachyWindowMode::WallSelect:
	{
		for (unsigned int i = 0; i < m_WallButtons.size(); ++i)
		{
			m_WallButtons[i]->Update(_dt);

			
		}
		if (m_ScrollBarObj->m_OffsetChanged)
		{
			for (unsigned int i = 0; i < m_WallButtons.size(); ++i)
			{
				Vector2 newPos = m_WallButtons[i]->GetPosition();
				newPos.y = i * 35 - m_ScrollBarObj->GetOffset();
				m_WallButtons[i]->SetPosition(newPos);
			}
			m_ScrollBarObj->m_OffsetChanged = false;
		}
	}
	break;
	case MapHierachyWindowMode::Enemy:


		break;
	default:
		break;
	}

}

void MapHierachyWindow::MouseDown(unsigned int _key)
{
	MapEditorWindow::MouseDown(_key);
	m_ScrollBarObj->MouseDown(_key);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->MouseDown(_key);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
	switch (m_Mode)
	{
	case MapHierachyWindowMode::WallSelect:
	{
		for (unsigned int i = 0; i < m_WallButtons.size(); ++i)
		{
			m_WallButtons[i]->MouseDown(_key);
		}
	}
	break;
	case MapHierachyWindowMode::Enemy:


		break;
	default:
		break;
	}
	

}

void MapHierachyWindow::MouseUp(unsigned int _key)
{
	MapEditorWindow::MouseUp(_key);
	m_ScrollBarObj->MouseUp(_key);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->MouseUp(_key);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
	switch (m_Mode)
	{
	case MapHierachyWindowMode::WallSelect:
	{
		for (unsigned int i = 0; i < m_WallButtons.size(); ++i)
		{
			m_WallButtons[i]->MouseUp(_key);
		}
	}
	break;
	case MapHierachyWindowMode::Enemy:

		
		break;
	default:
		break;
	}

	//Selecting thing ins the world
	if (!Box::BoxCollision(m_ContentBox, m_MousePos) && !Box::BoxCollision(m_Bar, m_MousePos))
	{

		switch (m_Mode)
		{
		case MapHierachyWindowMode::WallSelect:

			break;
		case MapHierachyWindowMode::Enemy:


			break;
		default:
			break;
		}
	}
}

void MapHierachyWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	Vector2 prev = m_MousePos;
	MapEditorWindow::MouseMove(_x, _y);
	m_ScrollBarObj->MouseMove(_x, _y);
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->MouseMove(_x, _y);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x, _y);
	}
	switch (m_Mode)
	{
	case MapHierachyWindowMode::WallSelect:
	{
		 int relativeX = ( int )m_Target->m_TargetBox.pos.x;
		 int relativey = ( int)m_Target->m_TargetBox.pos.y;
		for (unsigned int i  =0; i<m_WallButtons.size(); ++i)
		{
			m_WallButtons[i]->MouseMove(_x-relativeX, _y-relativey);
		}
	}
	break;
	case MapHierachyWindowMode::Enemy:


		break;
	default:
		break;
	}
}

void MapHierachyWindow::KeyDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->KeyDown(_key);
	}
}

void MapHierachyWindow::KeyUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->KeyUp(_key);
	}

}

void MapHierachyWindow::SetFont(TTF_Font* _font)
{
	MapEditorWindow::SetFont(_font);
	m_Font = _font;
	for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
	{
		m_InputTexts[i]->SetFont(_font);
	}
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->SetFont(_font);
	}
}

void MapHierachyWindow::SetMap(Hielke::Map* _map)
{
	m_Map = _map;
	GenerateHierachyWalls();
}

void MapHierachyWindow::MouseWheel(int _x, int _y)
{
	m_ScrollBarObj->MouseScroll(_x, _y);
}

void MapHierachyWindow::Render(SDLRenderer* _renderer)
{
		MapEditorWindow::Render(_renderer);

		if (m_Target == nullptr)
		{
			m_Target = _renderer->CreateRenderTarget(m_ContentBox, 2);
		}

		for (unsigned int i = 0; i < m_InputTexts.size(); ++i)
		{
			m_InputTexts[i]->Render(_renderer);
		}
		for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		{
			m_Buttons[i]->Render(_renderer);
		}
		_renderer->DrawFilledBox(m_ContentBox, m_Color);


		m_ScrollBarObj->Render(_renderer);
		//Depending on the selection mode we want to visualize currently selected
		// thing with a white box
		SDL_Color selectedCol = { 0xff,0xff,0xff,0xff };
		if (m_Map != nullptr)
		{
		switch (m_Mode)
		{
		case MapHierachyWindowMode::WallSelect:
		{
			if (m_CurrentSelectedWall != nullptr)
			{
				Box b = *m_CurrentSelectedWall;
				_renderer->DrawBoxZoomed(b, selectedCol, m_WorldPos, m_Zoom, 1);
			}
			if (m_Target != nullptr)
			{
				//Render ze buttons
				for (unsigned int i = 0; i < m_WallButtons.size(); ++i)
				{
					m_WallButtons[i]->RenderToTarget(_renderer, m_Target, { 0,0 });
				}

			}
		}
		break;
		case MapHierachyWindowMode::Enemy:


			break;
		default:
			break;
		}
	}
}



void MapHierachyWindow::Init(Texture* _IconsTexture)
{
	MapEditorWindow::Init(_IconsTexture);

	m_Mode = MapHierachyWindowMode::WallSelect;

	m_CurrentSelectedEnemy = nullptr;
	m_CurrentSelectedWall = nullptr;

	//Define the window's limits in terms of size
	m_MaxWidth = 500;
	m_MaxHeight = 800;
	m_MinWidth = 260;

	m_ContentBox.h = (float)m_MaxHeight / 2;
	m_PrevContentBox = m_ContentBox;

	m_ScrollBarObj = new ScrollBar{ m_IconTexture };
	m_ScrollBarObj->SetPosition(m_ContentBox.pos);
	m_ScrollBarObj->SetSize({m_ContentBox.w, m_ContentBox.h});

	m_Target = nullptr;
	//Reserve a flipping thingy
	Reposition();
}

void MapHierachyWindow::ReScaleContent()
{
	MapEditorWindow::ReScaleContent();
	if (m_Target != nullptr)
	{
		m_Target->CreateTexture(m_ContentBox);//Every tune we rescale we recreate the texture
	}
}

void MapHierachyWindow::Reposition()
{
	m_PrevContentBox = m_ContentBox;
	MapEditorWindow::Reposition();
	RepositionInputTexts();
	RepositionButtons();//Reposition buttons depnd on input texts therefor has to go second
	if (m_Target != nullptr)
	{
		m_Target->m_TargetBox.pos = m_ContentBox.pos;
	}
	float scrollBarWidth = m_ContentBox.w / 10;
	m_ScrollBarObj->SetPosition(m_ContentBox.pos + Vector2{ m_ContentBox.w- scrollBarWidth,0 });
	m_ScrollBarObj->SetSize(Vector2{ scrollBarWidth, m_ContentBox.h -m_ContentScaleObject.m_Size.y});
}


void MapHierachyWindow::RepositionButtons()
{
	

}


void MapHierachyWindow::RepositionInputTexts()
{

}

void MapHierachyWindow::SelectWall(int _index)
{
	printf(" Wow i pressed + %i \n", _index);
	if (m_Map != nullptr)
	{
		m_CurrentSelectedWall = &m_Map->GetColliders()[_index];
	}
}

void MapHierachyWindow::GenerateHierachyWalls()
{
	//Here's where we ask ourselves are we going to regenerate a bunch of 
	// textfields every time the array size of the textfield, and the walls
	// is not equal, or are we gonna do it just once and then every time
	// we add a wall or remove it we want to change it directly in this class
	//but how ?
	m_WallButtons.clear();
	if (m_Map != nullptr)
	{
		std::vector<Box> walls = m_Map->GetColliders();
		Vector2 pos = {0,0};
		Vector2 size = { 100,30 };
		std::function<void(int)> func= std::bind(&MapHierachyWindow::SelectWall, this, std::placeholders::_1);
		for (unsigned int i = 0; i < walls.size(); ++i)
		{
			pos.y = i * 35;//hardcoded for now
			std::string name = "Wall" + std::to_string(i);
			
			Button* b =new Button(ButtonBuilder::BuildButtonWireFrameOrFilledRect(pos, size, 1, std::bind(&MapHierachyWindow::SelectWall, this, std::placeholders::_1), (int)i,name, Button::DrawMode::FILLEDRECT,
				m_Color, m_LightColor, m_DarkColor, SDL_Color{ 0xff,0xff,0xff,0xff }));

			b->SetFont(m_Font);

			m_WallButtons.push_back(b);
		}
		float m_MaxOffset = (walls.size() * 35)-m_ContentBox.h;
		m_ScrollBarObj->SetMinimumAndMaximum(0, m_MaxOffset);
		
	}
}
