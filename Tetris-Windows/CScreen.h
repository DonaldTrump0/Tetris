#pragma once
#include "CBlock.h" 
#include <Windows.h> 

// 游戏界面的行数和列数
#define ROW 22
#define COL 12
// 方块的边长
#define BLOCK_SIZE 20

class CScreen
{
public:
    CScreen(HWND hWnd);
    void DrawScreen();
    void DrawGameOver();
    void ClearBlock();
    void PrintBlock();
    void SaveBlock();
    void EliminateLine();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void SpinUp();
    bool IsGameOver();

    void DrawBlock(HDC hdc, int nRow, int nCol);
    void ClearBlock(HDC hdc, int nRow, int nCol);

private:
    HWND m_hWnd;
    CBlock block;
    // 记录屏幕情况
    char screen[ROW][COL];
    // 记录每一行的方块数
    char blockCntOfRow[ROW];
    // 分数
    int score;
    // 是否GameOver
    bool isGameOver;
};

