#include "Tetris-MultiByte.h"
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

// 记录屏幕情况
char screen[ROW][COL] = { 0 };
//int screenColor[ROW][COL] = { 0 };
// 记录每一行的方块数
char blockCntOfRow[ROW] = { 0 };
// 当前方块类型
int curBlockIndex = 0;
// 当前方块所在行和列
int curBlockRow = 0;
int curBlockCol = 0;
// 分数
int score = 0;
// 是否GameOver
bool isGameOver = false;
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

// 颜色数组
//int colors[] = { FOREGROUND_RED,
//            FOREGROUND_RED | FOREGROUND_INTENSITY,
//            FOREGROUND_GREEN | FOREGROUND_INTENSITY,
//            FOREGROUND_BLUE | FOREGROUND_INTENSITY,
//            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
//            FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
//            FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY };
char blocks[][4][4] =
{
    // T
    {0,1,0,0,
     1,1,1,0,
     0,0,0,0,
     0,0,0,0},
    {0,1,0,0,
     1,1,0,0,
     0,1,0,0,
     0,0,0,0},
    {0,0,0,0,
     1,1,1,0,
     0,1,0,0,
     0,0,0,0},
    {0,1,0,0,
     0,1,1,0,
     0,1,0,0,
     0,0,0,0},
     // L1
     {0,0,1,0,
      1,1,1,0,
      0,0,0,0,
      0,0,0,0},
     {1,1,0,0,
      0,1,0,0,
      0,1,0,0,
      0,0,0,0},
     {0,0,0,0,
      1,1,1,0,
      1,0,0,0,
      0,0,0,0},
     {0,1,0,0,
      0,1,0,0,
      0,1,1,0,
      0,0,0,0},
      // L2
      {1,0,0,0,
       1,1,1,0,
       0,0,0,0,
       0,0,0,0},
      {0,1,0,0,
       0,1,0,0,
       1,1,0,0,
       0,0,0,0},
      {0,0,0,0,
       1,1,1,0,
       0,0,1,0,
       0,0,0,0},
      {0,1,1,0,
       0,1,0,0,
       0,1,0,0,
       0,0,0,0},
       // Z1
      {1,1,0,0,
       0,1,1,0,
       0,0,0,0,
       0,0,0,0},
      {0,1,0,0,
       1,1,0,0,
       1,0,0,0,
       0,0,0,0},
      {0,0,0,0,
       1,1,0,0,
       0,1,1,0,
       0,0,0,0},
      {0,0,1,0,
       0,1,1,0,
       0,1,0,0,
       0,0,0,0},
       // Z2
      {0,1,1,0,
       1,1,0,0,
       0,0,0,0,
       0,0,0,0},
      {1,0,0,0,
       1,1,0,0,
       0,1,0,0,
       0,0,0,0},
      {0,1,1,0,
       1,1,0,0,
       0,0,0,0,
       0,0,0,0},
      {1,0,0,0,
       1,1,0,0,
       0,1,0,0,
       0,0,0,0},
       // 一
      {0,0,0,0,
       1,1,1,1,
       0,0,0,0,
       0,0,0,0},
      {0,1,0,0,
       0,1,0,0,
       0,1,0,0,
       0,1,0,0},
      {0,0,0,0,
       1,1,1,1,
       0,0,0,0,
       0,0,0,0},
      {0,1,0,0,
       0,1,0,0,
       0,1,0,0,
       0,1,0,0},
       // 田
       {1,1,0,0,
        1,1,0,0,
        0,0,0,0,
        0,0,0,0},
       {1,1,0,0,
        1,1,0,0,
        0,0,0,0,
        0,0,0,0},
       {1,1,0,0,
        1,1,0,0,
        0,0,0,0,
        0,0,0,0},
       {1,1,0,0,
        1,1,0,0,
        0,0,0,0,
        0,0,0,0},
};

// 隐藏光标
void HiddenCursor()
{
    CONSOLE_CURSOR_INFO hidden;
    GetConsoleCursorInfo(hOut, &hidden);
    hidden.bVisible = 0;
    SetConsoleCursorInfo(hOut, &hidden);
}

// 移动光标
void Gotoxy(int row, int col)
{
    COORD pos;
    pos.X = col * 2;
    pos.Y = row;
    SetConsoleCursorPosition(hOut, pos);
}

// 初始化游戏
void InitGame()
{
    HiddenCursor();
    srand(time(NULL));

    // 随机选一种方块
    curBlockIndex = rand() % 7 * 4;
    curBlockRow = 0;
    curBlockCol = 4;
    score = 0;
    isGameOver = false;
    memset(screen, 0, sizeof(screen));
    memset(blockCntOfRow, 0, sizeof(blockCntOfRow));

    Gotoxy(ROW + 1, 0);
    printf("       Score: 0");

    for (int i = 0; i < ROW; i++)
    {
        Gotoxy(i, 0);
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

// 清除方块
void ClearBlock()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            if (screen[x][y] == 0)
            {
                Gotoxy(x, y);
                printf("  ");
            }
        }
    }
}

// 打印方块
void PrintBlock()
{
    //SetConsoleTextAttribute(hOut, colors[curBlockIndex / 4]);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            if (blocks[curBlockIndex][i][j])
            {
                Gotoxy(x, y);
                printf("■");
            }
        }
    }
    //SetConsoleTextAttribute(hOut, 0x0f);
}

// 保存方块信息
void SaveBlock()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            if (blocks[curBlockIndex][i][j])
            {
                screen[x][y] = 1;
                // 对应行的方块数加一
                blockCntOfRow[x]++;
            }
        }
    }
}

// 检测并消行
void EliminateLine()
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

void MoveDown()
{
    // 检测下方是否有方块
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            // 如果下方有方块
            if (blocks[curBlockIndex][i][j] && screen[x + 1][y])
            {
                // 检测游戏是否结束
                if (curBlockRow == 0)
                {
                    isGameOver = true;
                    return;
                }
                // 存储到Screen数组
                SaveBlock();
                // 检测并消行
                EliminateLine();
                // 重新产生一个方块
                curBlockRow = 0;
                curBlockCol = 4;
                curBlockIndex = rand() % 7 * 4;
                PrintBlock();
                return;
            }
        }
    }
    ClearBlock();
    curBlockRow++;
    PrintBlock();
}

void MoveLeft()
{
    // 检测左边是否有方块
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            if (blocks[curBlockIndex][i][j] && screen[x][y - 1])
            {
                return;
            }
        }
    }
    ClearBlock();
    curBlockCol--;
    PrintBlock();
}

void MoveRight()
{
    // 检测右边是否有方块
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            if (blocks[curBlockIndex][i][j] && screen[x][y + 1])
            {
                return;
            }
        }
    }
    ClearBlock();
    curBlockCol++;
    PrintBlock();
}

// 旋转
void SpinUp()
{
    curBlockIndex++;
    if (curBlockIndex % 4 == 0)
    {
        curBlockIndex -= 4;
    }
    // 检测是否可以旋转
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            if (blocks[curBlockIndex][i][j] && screen[x][y])
            {
                // 回退
                if (curBlockIndex % 4 == 0)
                {
                    curBlockIndex += 4;
                }
                curBlockIndex--;
                return;
            }
        }
    }
    ClearBlock();
    PrintBlock();
}

void StartGame()
{
    InitGame();
    clock_t PreTime = clock();
    while (!isGameOver)
    {
        if (clock() - PreTime >= 1000)
        {
            PreTime = clock();
            MoveDown();
        }
        if (_kbhit())
        {
            char c = getch();
            switch (c)
            {
            case 'w':
            case 'W':
                SpinUp();
                break;
            case 's':
            case 'S':
                MoveDown();
                break;
            case 'a':
            case 'A':
                MoveLeft();
                break;
            case 'd':
            case 'D':
                MoveRight();
                break;
            default:
                break;
            }
        }
    } // while (!isGameOver)
}

void PlayGame()
{
    while (true)
    {
        StartGame();
        // 游戏结束
        Gotoxy(ROW + 1, 0);
        printf("Game Over!\r\n");
        system("pause");
        system("cls");
    }
}