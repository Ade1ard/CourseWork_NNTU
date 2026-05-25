#ifndef VIEW_H
#define VIEW_H

#include <windows.h>
#include "Constants.h"

extern char currentScreen[HEIGHT][WIDTH];

void ClearNewBuffer();
void RenderChanges();
void DrawChar(int x, int y, char ch, int color);
void DrawTextt(int x, int y, const char st[], int color);
void DrawSquare(int x, int y, int size, char ch, int color);
void DrawText(int x, int y, const char st[], int color);
void DrawHP(int HP);
void DrawPlayer(int x, int y);
void DrawScore(float score, float combo);
void DrawRock(int x, int y, int size, int shape, int color);
void HideCursor();
void SetFullscreen();

#endif