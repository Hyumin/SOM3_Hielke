#pragma once

#include "SDLRenderer.h"

class Layer
{
public:
	Layer(std::string _name);
	Layer();
	~Layer();


	std::string& GetName() { return m_Name; }
	void SetName(std::string& _name) { m_Name = _name; }

	void ClearQueues();
	void AddInterface(RenderInterface& _interface);
	void AddText(TextRenderInterface& _textInterface);
	void AddLine(Line& _line);
	void AddFilledBox(FilledBox& _filledBox);

	std::vector<RenderInterface>& GetRenderQueue();
	std::vector<TextRenderInterface>& GetTextRenderQueue();
	std::vector<Line>& GetLineQueue();
	std::vector<FilledBox>& GetFilledBoxQueue();


private:
	std::string m_Name;

	std::vector<RenderInterface> m_RenderQueue;
	std::vector<TextRenderInterface> m_TextRenderQueue;
	std::vector<Line> m_LineQueue;
	std::vector< FilledBox> m_FilledBoxes;
};

