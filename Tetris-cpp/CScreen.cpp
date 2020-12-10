#include "CScreen.h"
#include <stdio.h>
#include <Windows.h>

CScreen::CScreen()
{
    score = 0;
    isGameOver = false;
    memset(screen, 0, sizeof(screen));
    memset(blockCntOfRow, 0, sizeof(blockCntOfRow));
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    HiddenCursor();

    Gotoxy(ROW + 1, 0);
    printf("       Score: 0");

    for (int i = 0; i < ROW; i++)
    {
        CScreen::Gotoxy(i, 0);
        for (int j = 0; j < COL; j++)
        {
            if (i == 0 || j == 0 || i == ROW - 1 || j == COL - 1)
            {
                screen[i][j] = 1;
                printf("��");
            }
            else
            {
                printf("  ");
            }
        }
    }
}

void CScreen::Gotoxy(int row, int col)
{
    COORD pos;
    pos.X = col * 2;
    pos.Y = row;
    SetConsoleCursorPosition(hOut, pos);
}

void CScreen::HiddenCursor()
{
    CONSOLE_CURSOR_INFO hidden;
    GetConsoleCursorInfo(hOut, &hidden);
    hidden.bVisible = 0;
    SetConsoleCursorInfo(hOut, &hidden);
}

// �������
void CScreen::ClearBlock()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            if (screen[x][y] == 0)
            {
                Gotoxy(x, y);
                printf("  ");
            }
        }
    }
}

// ��ӡ����
void CScreen::PrintBlock()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            if (CBlock::blocks[block.GetCurBlockIndex()][i][j])
            {
                Gotoxy(x, y);
                printf("��");
            }
        }
    }
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
        for (int i = 1; i < ROW - 1; i++)
        {
            for (int j = 1; j < COL - 1; j++)
            {
                Gotoxy(i, j);
                if (screen[i][j])
                {
                    printf("��");
                }
                else
                {
                    printf("  ");
                }
            }
        }
        // ���·���
        Gotoxy(ROW + 1, 0);
        printf("       Score: %d", score);
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
