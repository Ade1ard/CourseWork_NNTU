#include <windows.h>
#include <stdio.h>

#include "Constants.h"

char currentScreen[HEIGHT][WIDTH];
char newScreen[HEIGHT][WIDTH];
int colorBuffer[HEIGHT][WIDTH];
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void ClearNewBuffer()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            newScreen[y][x] = ' ';
        }
    }
}

void RenderChanges()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (currentScreen[y][x] != newScreen[y][x])
            {
                COORD pos = { x, y };
                SetConsoleCursorPosition(hConsole, pos);
                SetConsoleTextAttribute(hConsole, colorBuffer[y][x]);
                printf("%c", newScreen[y][x]);
                currentScreen[y][x] = newScreen[y][x];
            }
        }
    }
}

void DrawChar(int x, int y, char ch, int color)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        newScreen[y][x] = ch;
        colorBuffer[y][x] = color;
    }
}

void DrawTextt(int x, int y, const char st[], int color)
{
    int len = strlen(st);
    for (int i = 0; i < len; i++)
    {
        DrawChar(x - len / 2 + i, y, st[i], color);
    }
}


void DrawScore(float score, float combo)
{
    char scoreStr[50];

    snprintf(scoreStr, sizeof(scoreStr), "SCORE: %.0f", score);
    DrawTextt(WIDTH - 25, 1, scoreStr, 14);

    if (combo > 0)
    {
        snprintf(scoreStr, sizeof(scoreStr), "COMBO x%.0f", combo);
        DrawTextt(WIDTH - 25, 3, scoreStr, 12);
    }
}

void DrawSquare(int x, int y, int size, char ch, int color)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            DrawChar(x + j, y + i, ch, color);
        }
    }
}

void DrawHP(int HP)
{
    static const char heartFull[3][4] = {
        {'/', '\\', '/', '\\'},
        {'\\', ' ', ' ', '/'},
        {' ', '\\', '/', ' '}
    };

    static const char heartEmpty[3][4] = {
        {'.', '.', '.', '.'},
        {'.', ' ', ' ', '.'},
        {' ', '.', '.', ' '}
    };

    for (int heartNum = 0; heartNum < 3; heartNum++)
    {
        int startX = 2 + heartNum * 5;
        int startY = 1;

        const char (*currentHeart)[4];
        int color;

        if (heartNum < HP)
        {
            currentHeart = heartFull;
            color = 12;
        }
        else
        {
            currentHeart = heartEmpty;
            color = 8;
        }

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
                if (currentHeart[i][j] != ' ')
                    DrawChar(startX + j, startY + i, currentHeart[i][j], color);
    }
}

void DrawPlayer(int x, int y)
{
    static const char player[3][3] = {
        {'/','-','\\'},
        {'|',' ','|'},
        {'\\','_','/'}
    };

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            DrawChar(x + j, y + i, player[i][j], 11);
}

void DrawRock(int x, int y, int size, int shape, int color)
{
    static const char rockShapes[3][3][3] = {
        {{'O','\\',' '},{'O','O','\\'},{'\\','0','/'}},
        {{'O','O','\\'},{'\\','O','O'},{' ','O','/'}},
        {{'O','O','\\'},{'O','O','O'},{'\\','O','/'}}
    };

    for (int i = 0; i < 3 && i < size; i++)
        for (int j = 0; j < 3 && j < size; j++)
            if (rockShapes[shape][i][j] != ' ')
                DrawChar(x + j, y + i, rockShapes[shape][i][j], color);
}

void HideCursor()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void SetFullscreen()
{
    keybd_event(VK_MENU, 0, 0, 0);
    keybd_event(VK_RETURN, 0, 0, 0);
    Sleep(50);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
}