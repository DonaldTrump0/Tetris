#include "CGame.h"
#include "CScreen.h"
#include "CBlock.h"
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

CGame::CGame()
{
    srand(time(NULL));
}

void CGame::PlayGame()
{
    StartGame();
    // ÓÎÏ·½áÊø
    screen.Gotoxy(ROW + 1, 0);
    printf("Game Over!\r\n");
    system("pause");
    system("cls");
}

void CGame::StartGame()
{
    clock_t PreTime = clock();
    while (!screen.IsGameOver())
    {
        if (clock() - PreTime >= 1000)
        {
            PreTime = clock();
            screen.MoveDown();
        }
        if (_kbhit())
        {
            char c = getch();
            switch (c)
            {
            case 'w':
            case 'W':
                screen.SpinUp();
                break;
            case 's':
            case 'S':
                screen.MoveDown();
                break;
            case 'a':
            case 'A':
                screen.MoveLeft();
                break;
            case 'd':
            case 'D':
                screen.MoveRight();
                break;
            default:
                break;
            }
        }
    } // while (!isGameOver)
}