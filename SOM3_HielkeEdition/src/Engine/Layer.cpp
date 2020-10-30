#include "Layer.h"

Layer::Layer(std::string _name)
{
	SetName(_name);
}

Layer::Layer()
{
	std::string defName = "Default";
	SetName(defName);
}

Layer::~Layer()
{
}

void Layer::ClearQueues()
{
	m_RenderQueue.clear();
	m_TextRenderQueue.clear();
	m_LineQueue.clear();
	m_FilledBoxes.clear();
}

void Layer::AddInterface(RenderInterface& _interface)
{
	m_RenderQueue.push_back(_interface);
}

void Layer::AddText(TextRenderInterface& _textInterface)
{
	m_TextRenderQueue.push_back(_textInterface);
}

void Layer::AddLine(Line& _line)
{
	m_LineQueue.push_back(_line);
}

void Layer::AddFilledBox(FilledBox& _filledBox)
{
	m_FilledBoxes.push_back(_filledBox);
}

std::vector<RenderInterface>& Layer::GetRenderQueue()
{
	// TODO: insert return statement here
	return m_RenderQueue;
}

std::vector<TextRenderInterface>& Layer::GetTextRenderQueue()
{
	// TODO: insert return statement here
	return m_TextRenderQueue;
}

std::vector<Line>& Layer::GetLineQueue()
{
	// TODO: insert return statement here
	return m_LineQueue;
}

std::vector<FilledBox>& Layer::GetFilledBoxQueue()
{
	// TODO: insert return statement here
	return m_FilledBoxes;
}
