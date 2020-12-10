#pragma once
#include "CScreen.h"

class CGame
{
public:
    CGame();
    void PlayGame();

private:
    void StartGame();

private:
    CScreen screen;
};

