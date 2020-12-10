#pragma once
#include "CBlock.h" 
#include <Windows.h> 

// ��Ϸ���������������
#define ROW 22
#define COL 12
// ����ı߳�
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
    // ��¼��Ļ���
    char screen[ROW][COL];
    // ��¼ÿһ�еķ�����
    char blockCntOfRow[ROW];
    // ����
    int score;
    // �Ƿ�GameOver
    bool isGameOver;
};

