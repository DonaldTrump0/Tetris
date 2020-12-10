#pragma once
#include "CBlock.h" 
#include <Windows.h> 

// ��Ϸ���������������
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

    // �ƶ����
    void Gotoxy(int row, int col);
    void HiddenCursor();

private:
    CBlock block;
    HANDLE hOut;
    // ��¼��Ļ���
    char screen[ROW][COL];
    // ��¼ÿһ�еķ�����
    char blockCntOfRow[ROW];
    // ����
    int score;
    // �Ƿ�GameOver
    bool isGameOver;
};

