#include "Tetris-Auto.h"
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <float.h>
#include <Windows.h>

// 记录屏幕占用情况
char screen[ROW][COL] = { 0 };
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
      {0,0,0,0,
       0,1,1,0,
       1,1,0,0,
       0,0,0,0},
      {0,1,0,0,
       0,1,1,0,
       0,0,1,0,
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

    // 打印边框
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

    Gotoxy(ROW + 1, 0);
    printf("       Score: 0");
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

// 取消保存的方块信息
void UnSaveBlock()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            if (blocks[curBlockIndex][i][j])
            {
                screen[x][y] = 0;
                blockCntOfRow[x]--;
            }
        }
    }
}

// 更新屏幕(不打印边框)，更新分数
void UpdateScreen()
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

// 检测并消行
bool EliminateLine()
{
    bool isEliminated = false;
    for (int i = ROW - 2; i >= 1; i--)
    {
        if (blockCntOfRow[i] == COL - 2)
        {
            score++;
            isEliminated = true;
            memmove(screen[2], screen[1], (i - 1) * COL * sizeof(char));
            memmove(blockCntOfRow + 2, blockCntOfRow + 1, (i - 1) * sizeof(char));
            i++;
        }
    }
    return isEliminated;
}

bool MoveDown()
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
                    return false;
                }
                // 存储到screen数组
                SaveBlock();
                // 检测并消行
                if (EliminateLine())
                {
#ifdef PRINT_PROCESS
                    UpdateScreen();
#endif
                }
                return false;
            }
        }
    }

#ifdef PRINT_PROCESS
    ClearBlock();
#endif
    curBlockRow++;
#ifdef PRINT_PROCESS
    PrintBlock();
#endif
    return true;
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


// 检测是否重叠
bool IsOverlap()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            if (blocks[curBlockIndex][i][j] && screen[x][y])
            {
                return true;
            }
        }
    }
    return false;
}

bool _MoveDown()
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
                    return false;
                }
                // 存储到Screen数组
                SaveBlock();
                return false;
            }
        }
    }
    curBlockRow++;
    return true;
}

// 移动到底部
void MoveToBottom()
{
    while (_MoveDown())
    {
        ;
    }
}

// Landing Height 表示当前方块放置之后的高度。这个高度的计算方法是使用当前容器内方块的高度加上当前下落方块的一半
int GetLandingHeight()
{
    int landingHeight = ROW - curBlockRow - 1;
    bool flag = false;
    for (int i = 0; i < 4; i++)
    {
        if (blocks[curBlockIndex][0][i] == 1)
        {
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        landingHeight--;
    }
    return landingHeight;
}

// Rows eliminated 表示当前方块落下后被消减的行数
int GetRowsEliminated()
{
    int rowsEliminated = 0;
    for (int i = 1; i < ROW - 1; i++)
    {
        if (blockCntOfRow[i] == COL - 2)
        {
            rowsEliminated++;
        }
    }
    //if (rowsEliminated)
    return rowsEliminated;
}

//Row Transitions 代表容器中水平方向上变换的次数
int GetRowTransitions()
{
    int rowTransitions = 0;
    for (int i = 1; i < ROW - 1; i++)
    {
        for (int j = 2; j < COL - 1; j++)
        {
            if (screen[i][j] != screen[i][j - 1])
            {
                rowTransitions++;
            }
        }
    }
    return rowTransitions;
}

//Column Transitions 和第三个特征点类似，只不过这一次算的是列变换而已
int GetColumnTransitions()
{
    int columnTransitions = 0;
    for (int j = 1; j < COL - 1; j++)
    {
        for (int i = 2; i < ROW - 1; i++)
        {
            if (screen[i][j] != screen[i - 1][j])
            {
                columnTransitions++;
            }
        }
    }
    return columnTransitions;
}

//Number of Holes 代表图中空洞的个数
int GetNumberOfHoles()
{
    int numberOfHoles = 0;
    for (int j = 1; j < COL - 1; j++)
    {
        bool flag = false;
        for (int i = 1; i < ROW - 1; i++)
        {
            if (screen[i][j] == 1)
            {
                flag = true;
            }
            else
            {
                if (flag)
                {
                    numberOfHoles++;
                }
            }
        }
    }
    return numberOfHoles;
}

//Well Sums 表示 “井” 深的连加和
int GetWellSums()
{
    int wellSums = 0;
    for (int j = 1; j < COL - 1; j++)
    {
        int add = 1;
        for (int i = 1; i < ROW - 1; i++)
        {
            if (screen[i][j] == 0)
            {
                if (screen[i][j - 1] == 1 && screen[i][j + 1] == 1)
                {
                    wellSums += add;
                    add++;
                }
            }
            else
            {
                add = 1;
            }
        }
    }
    return wellSums;
}

// 计算评估值
double CalEvaluation()
{
    int landingHeight = GetLandingHeight();
    int rowsEliminated = GetRowsEliminated();
    char tempScreen[ROW][COL] = { 0 };
    char tempBlockCntRow[ROW] = { 0 };
    if (rowsEliminated)
    {
        landingHeight -= rowsEliminated;
        memmove(tempScreen, screen, sizeof(screen));
        memmove(tempBlockCntRow, blockCntOfRow, sizeof(blockCntOfRow));
        EliminateLine();
    }
    int rowTransitions = GetRowTransitions();
    int columnTransitions = GetColumnTransitions();
    int numberOfHoles = GetNumberOfHoles();
    int wellSums = GetWellSums();

    if (rowsEliminated)
    {
        // 恢复
        memmove(screen, tempScreen, sizeof(screen));
        memmove(blockCntOfRow, tempBlockCntRow, sizeof(blockCntOfRow));
        score -= rowsEliminated;
    }

#ifdef PRINT_DEBUG
    Gotoxy(0, COL * 2 + 4);
    printf("landingHeight = %d", landingHeight);
    Gotoxy(1, COL * 2 + 4);
    printf("rowsEliminated = %d", rowsEliminated);
    Gotoxy(2, COL * 2 + 4);
    printf("rowTransitions = %d", rowTransitions);
    Gotoxy(3, COL * 2 + 4);
    printf("columnTransitions = %d", columnTransitions);
    Gotoxy(4, COL * 2 + 4);
    printf("numberOfHoles = %d", numberOfHoles);
    Gotoxy(5, COL * 2 + 4);
    printf("wellSums = %d", wellSums);
#endif

    return landingHeight * (-4.500158825082766)
        + rowsEliminated * 3.4181268101392694
        + rowTransitions * (-3.2178882868487753)
        + columnTransitions * (-9.348695305445199)
        + numberOfHoles * (-7.899265427351652)
        + wellSums * (-3.3855972247263626);
}

void _PrintScreen()
{
    for (int i = 0; i < ROW; i++)
    {
        Gotoxy(i, COL + 2);
        for (int j = 0; j < COL; j++)
        {
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
}

void AutoPlayGame()
{
    InitGame();

    while (!isGameOver)
    {
        curBlockIndex = rand() % 7 * 4;
        double maxEvaluation = -DBL_MAX;
        int maxBlockIndex = 0;
        int maxBlockCol = 0;

        // 遍历每种形状
        for (int i = 0; i < 4; i++)
        {
            curBlockIndex += i;
            // 遍历每个起始列
            for (int j = 0; j < COL - 2; j++)
            {
                curBlockRow = 1;
                curBlockCol = j;
                if (IsOverlap())
                {
                    continue;
                }
                MoveToBottom(); 
                double evaluation = CalEvaluation();
                
                if (evaluation > maxEvaluation)
                {
                    maxBlockIndex = i;
                    maxBlockCol = j;
                    maxEvaluation = evaluation;
                }

#ifdef PRINT_DEBUG
                _PrintScreen();
                Gotoxy(6, COL * 2 + 4);
                printf("weight = %llf", evaluation);
                Gotoxy(7, COL * 2 + 4);
                printf("maxWeight = %llf", maxEvaluation);
#endif

                UnSaveBlock();
            }
            curBlockIndex -= i;
        }

        curBlockIndex += maxBlockIndex;
        curBlockRow = 0;
        curBlockCol = maxBlockCol;
        while (MoveDown())
        {
#ifdef PRINT_PROCESS
            Sleep(TIME_INTERVAL);
#endif
        }

        Gotoxy(ROW + 1, 0);
        printf("       Score: %d", score);
    } // while (!isGameOver)
}

void PlayGame()
{
    while (true)
    {
        AutoPlayGame();

        _PrintScreen(); 
        Gotoxy(ROW + 1, 0);
        printf("       Score: %d", score);

        // 游戏结束
        Gotoxy(ROW + 3, 0);
        printf("Game Over!\r\n");
        system("pause");
        system("cls");
    }
}