#include "WindowOpener.h"
#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>
WindowOpener::WindowOpener()
{
}

WindowOpener::~WindowOpener()
{
}

std::string WindowOpener::PrintAndOpenStuff()
{
    std::string returnString= "CANCELED";
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL, "($ProjectDir)", &outPath);

    if (result == NFD_OKAY) {
        puts("Success!");
        returnString = outPath;
        puts(outPath);
        free(outPath);
        return returnString;
    }
    else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
        return returnString;
    }
    else {
        printf("Error: %s\n", NFD_GetError());
        throw(std::exception("Error!"));
    }

}
