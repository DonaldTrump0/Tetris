#include "CScreen.h"
#include <stdio.h>
#include <time.h>
#include <Windows.h>

CScreen::CScreen(HWND hWnd)
{
    m_hWnd = hWnd;
    score = 0;
    isGameOver = false;
    memset(screen, 0, sizeof(screen));
    memset(blockCntOfRow, 0, sizeof(blockCntOfRow));

    srand(time(NULL));

    /*Gotoxy(ROW + 1, 0);
    printf("       Score: 0");*/

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (i == 0 || j == 0 || i == ROW - 1 || j == COL - 1)
            {
                screen[i][j] = 1;
            }
        }
    }
}

void CScreen::DrawScreen()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (screen[i][j] == 1)
            {
                DrawBlock(hdc, i, j);
            }
        }
    }

    EndPaint(m_hWnd, &ps);
}

void CScreen::DrawGameOver()
{
    HDC hdc = GetDC(m_hWnd);

    int left = COL * BLOCK_SIZE / 2;
    int top = ROW * BLOCK_SIZE / 2;
    RECT rect = { left - 40, top, left + 50, top + 50 };
    
    DrawText(hdc, TEXT("Game Over!"), 10, &rect, DT_LEFT);

    ReleaseDC(m_hWnd, hdc);
}

void CScreen::DrawBlock(HDC hdc, int nRow, int nCol)
{
    SelectObject(hdc, GetStockObject(WHITE_PEN));
    SelectObject(hdc, GetStockObject(DKGRAY_BRUSH));
    Rectangle(hdc, nCol * 20, nRow * 20, nCol * 20 + 20, nRow * 20 + 20);
}

void CScreen::ClearBlock(HDC hdc, int nRow, int nCol)
{
    SelectObject(hdc, GetStockObject(WHITE_PEN));
    SelectObject(hdc, GetStockObject(WHITE_BRUSH));
    Rectangle(hdc, nCol * 20, nRow * 20, nCol * 20 + 20, nRow * 20 + 20);
}

// �������
void CScreen::ClearBlock()
{
    HDC hdc = GetDC(m_hWnd);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            if (screen[x][y] == 0)
            {
                ClearBlock(hdc, x, y);
            }
        }
    }
    ReleaseDC(m_hWnd, hdc);
}

// ��ӡ����
void CScreen::PrintBlock()
{
    HDC hdc = GetDC(m_hWnd);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            if (CBlock::blocks[block.GetCurBlockIndex()][i][j])
            {
                DrawBlock(hdc, x, y);
            }
        }
    }
    ReleaseDC(m_hWnd, hdc);
}

// ���淽����Ϣ
void CScreen::SaveBlock()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            if (CBlock::blocks[block.GetCurBlockIndex()][i][j])
            {
                screen[x][y] = 1;
                // ��Ӧ�еķ�������һ
                blockCntOfRow[x]++;
            }
        }
    }
}

// ��Ⲣ����
void CScreen::EliminateLine()
{
    bool isRemove = false;
    // �����Ǳ߿�
    for (int i = ROW - 2; i >= 1; i--)
    {
        if (blockCntOfRow[i] == COL - 2)
        {
            score++;
            isRemove = true;
            memmove(screen[2], screen[1], (i - 1) * COL * sizeof(char));
            memmove(blockCntOfRow + 2, blockCntOfRow + 1, (i - 1) * sizeof(char));
            i++;
        }
    }
    // ������У������»���(�����Ʊ߿�)
    if (isRemove)
    {
        RECT rect;
        GetClientRect(m_hWnd, &rect);
        InvalidateRect(m_hWnd, &rect, TRUE);
        // ���·���
        //Gotoxy(ROW + 1, 0);
        //printf("       Score: %d", score);
    }
}

void CScreen::MoveDown()
{
    // ����·��Ƿ��з���
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            // ����·��з���
            if (CBlock::blocks[block.GetCurBlockIndex()][i][j] && screen[x + 1][y])
            {
                // �����Ϸ�Ƿ����
                if (block.GetCurBlockRow() == 0)
                {
                    isGameOver = true;
                    return;
                }
                // �洢��Screen����
                SaveBlock();
                // ��Ⲣ����
                EliminateLine();
                // ���²���һ������
                block.SetCurBlockRow(0);
                block.SetCurBlockCol(4);
                block.SetCurBlockIndex(rand() % 7 * 4);
                PrintBlock();
                return;
            }
        }
    }
    ClearBlock();
    block.SetCurBlockRow(block.GetCurBlockRow() + 1);
    PrintBlock();
}

void CScreen::MoveLeft()
{
    // �������Ƿ��з���
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            if (CBlock::blocks[block.GetCurBlockIndex()][i][j] && screen[x][y - 1])
            {
                return;
            }
        }
    }
    ClearBlock();
    block.SetCurBlockCol(block.GetCurBlockCol() - 1);
    PrintBlock();
}

void CScreen::MoveRight()
{
    // ����ұ��Ƿ��з���
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            if (CBlock::blocks[block.GetCurBlockIndex()][i][j] && screen[x][y + 1])
            {
                return;
            }
        }
    }
    ClearBlock();
    block.SetCurBlockCol(block.GetCurBlockCol() + 1);
    PrintBlock();
}

// ��ת
void CScreen::SpinUp()
{
    block.SetCurBlockIndex(block.GetCurBlockIndex() + 1);
    if (block.GetCurBlockIndex() % 4 == 0)
    {
        block.SetCurBlockIndex(block.GetCurBlockIndex() - 4);
    }
    // ����Ƿ������ת
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            if (CBlock::blocks[block.GetCurBlockIndex()][i][j] && screen[x][y])
            {
                // ����
                if (block.GetCurBlockIndex() % 4 == 0)
                {
                    block.SetCurBlockIndex(block.GetCurBlockIndex() + 4);
                }
                block.SetCurBlockIndex(block.GetCurBlockIndex() - 1);
                return;
            }
        }
    }
    ClearBlock();
    PrintBlock();
}

bool CScreen::IsGameOver()
{
    return isGameOver;
}
