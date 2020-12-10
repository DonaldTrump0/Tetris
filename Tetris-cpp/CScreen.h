#pragma once
#include "CBlock.h" 
#include <Windows.h> 

// 游戏界面的行数和列数
#define ROW 22
#define COL 12

class CScreen
{
public:
    CScreen();
    void ClearBlock();
    void PrintBlock();
    void SaveBlock();
    void EliminateLine();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void SpinUp();
    bool IsGameOver();

    // 移动光标
    void Gotoxy(int row, int col);
    void HiddenCursor();

private:
    CBlock block;
    HANDLE hOut;
    // 记录屏幕情况
    char screen[ROW][COL];
    // 记录每一行的方块数
    char blockCntOfRow[ROW];
    // 分数
    int score;
    // 是否GameOver
    bool isGameOver;
};

