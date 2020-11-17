#pragma once
#include <string>


class WindowOpener
{
public :

	WindowOpener();
	~WindowOpener();
	std::string PrintAndOpenStuff();
	std::string PrintAndOpenPng();
	std::string SaveFileStuff();
	static std::string GetFilepath(const char* _extension);

private:

};

