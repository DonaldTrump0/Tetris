#include "Tetris-MultiByte.h"
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

// ��¼��Ļ���
char screen[ROW][COL] = { 0 };
//int screenColor[ROW][COL] = { 0 };
// ��¼ÿһ�еķ�����
char blockCntOfRow[ROW] = { 0 };
// ��ǰ��������
int curBlockIndex = 0;
// ��ǰ���������к���
int curBlockRow = 0;
int curBlockCol = 0;
// ����
int score = 0;
// �Ƿ�GameOver
bool isGameOver = false;
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

// ��ɫ����
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
       // һ
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
       // ��
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

// ���ع��
void HiddenCursor()
{
    CONSOLE_CURSOR_INFO hidden;
    GetConsoleCursorInfo(hOut, &hidden);
    hidden.bVisible = 0;
    SetConsoleCursorInfo(hOut, &hidden);
}

// �ƶ����
void Gotoxy(int row, int col)
{
    COORD pos;
    pos.X = col * 2;
    pos.Y = row;
    SetConsoleCursorPosition(hOut, pos);
}

// ��ʼ����Ϸ
void InitGame()
{
    HiddenCursor();
    srand(time(NULL));

    // ���ѡһ�ַ���
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
                printf("��");
            }
            else
            {
                printf("  ");
            }
        }
    }
}

// �������
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

// ��ӡ����
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
                printf("��");
            }
        }
    }
    //SetConsoleTextAttribute(hOut, 0x0f);
}

// ���淽����Ϣ
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
                // ��Ӧ�еķ�������һ
                blockCntOfRow[x]++;
            }
        }
    }
}

// ��Ⲣ����
void EliminateLine()
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

void MoveDown()
{
    // ����·��Ƿ��з���
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            // ����·��з���
            if (blocks[curBlockIndex][i][j] && screen[x + 1][y])
            {
                // �����Ϸ�Ƿ����
                if (curBlockRow == 0)
                {
                    isGameOver = true;
                    return;
                }
                // �洢��Screen����
                SaveBlock();
                // ��Ⲣ����
                EliminateLine();
                // ���²���һ������
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
    // �������Ƿ��з���
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
    // ����ұ��Ƿ��з���
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

// ��ת
void SpinUp()
{
    curBlockIndex++;
    if (curBlockIndex % 4 == 0)
    {
        curBlockIndex -= 4;
    }
    // ����Ƿ������ת
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int x = curBlockRow + i;
            int y = curBlockCol + j;
            if (blocks[curBlockIndex][i][j] && screen[x][y])
            {
                // ����
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
        // ��Ϸ����
        Gotoxy(ROW + 1, 0);
        printf("Game Over!\r\n");
        system("pause");
        system("cls");
    }
}