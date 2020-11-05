#include "Editor.h"

Editor::Editor(ResourceManager* _resman)
{
	Init(_resman);
}

Editor::~Editor()
{
	delete m_AnimClipEditor;
	m_AnimClipEditor = nullptr;


}

void Editor::KeyDown(unsigned int _key)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->KeyDown(_key);
		break;
	case EditorState::Map:
		break;
	}
}

void Editor::KeyUp(unsigned int _key)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->KeyUp(_key);
		break;
	case EditorState::Map:
		break;
	}
}

void Editor::MouseDown(unsigned int _key)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->MouseDown(_key);
		break;
	case EditorState::Map:
		break;
	}
}

void Editor::MouseUp(unsigned int _key)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->MouseUp(_key);
		break;
	case EditorState::Map:
		break;
	}
}

void Editor::MouseMove(int _x, int _y)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->MouseMove(_x, _y);
		break;
	case EditorState::Map:
		break;
	}
}

void Editor::Update(float _dt)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->Update(_dt);
		break;
	case EditorState::Map:
		break;
	}
}

void Editor::Render(SDLRenderer* _renderer)
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->Render(_renderer);
		break;
	case EditorState::Map:
		break;
	}
	//Render default stuffs
}

void Editor::Init(ResourceManager* _resman)
{
	m_ResourceManager = _resman;
	m_AnimClipEditor = new AnimationClipEditor(m_ResourceManager);

	//Initialize buttons etc.

	//m_MapEditor = new MapEditor();
}

void Editor::SaveCallback()
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->SaveClip();
		break;
	case EditorState::Map:
		break;
	}
}

void Editor::LoadCallback()
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->LoadClip();
		break;
	case EditorState::Map:
		break;
	}
}

void Editor::ZoomInCallback()
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->ZoomIn();
		break;
	case EditorState::Map:
		break;
	}
}

void Editor::ZoomOutCallback()
{
	switch (m_EditorState)
	{
	case EditorState::AnimationClip:
		m_AnimClipEditor->ZoomOut();
		break;
	case EditorState::Map:
		break;
	}
}
