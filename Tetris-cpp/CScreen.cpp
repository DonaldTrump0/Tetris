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
                printf("■");
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

// 清除方块
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

// 打印方块
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
                printf("■");
            }
        }
    }
}

// 保存方块信息
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
                // 对应行的方块数加一
                blockCntOfRow[x]++;
            }
        }
    }
}

// 检测并消行
void CScreen::EliminateLine()
{
    bool isRemove = false;
    // 不考虑边框
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
    // 如果消行，则重新绘制(不绘制边框)
    if (isRemove)
    {
        for (int i = 1; i < ROW - 1; i++)
        {
            for (int j = 1; j < COL - 1; j++)
            {
                Gotoxy(i, j);
                if (screen[i][j])
                {
                    printf("■");
                }
                else
                {
                    printf("  ");
                }
            }
        }
        // 更新分数
        Gotoxy(ROW + 1, 0);
        printf("       Score: %d", score);
    }
}

void CScreen::MoveDown()
{
    // 检测下方是否有方块
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            // 如果下方有方块
            if (CBlock::blocks[block.GetCurBlockIndex()][i][j] && screen[x + 1][y])
            {
                // 检测游戏是否结束
                if (block.GetCurBlockRow() == 0)
                {
                    isGameOver = true;
                    return;
                }
                // 存储到Screen数组
                SaveBlock();
                // 检测并消行
                EliminateLine();
                // 重新产生一个方块
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
    // 检测左边是否有方块
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
    // 检测右边是否有方块
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

// 旋转
void CScreen::SpinUp()
{
    block.SetCurBlockIndex(block.GetCurBlockIndex() + 1);
    if (block.GetCurBlockIndex() % 4 == 0)
    {
        block.SetCurBlockIndex(block.GetCurBlockIndex() - 4);
    }
    // 检测是否可以旋转
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = block.GetCurBlockRow() + i;
            int y = block.GetCurBlockCol() + j;
            if (CBlock::blocks[block.GetCurBlockIndex()][i][j] && screen[x][y])
            {
                // 回退
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
