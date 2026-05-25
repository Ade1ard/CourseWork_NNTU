#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "Rocks.h"
#include "View.h"
#include "Constants.h"

// Цвета:
    // 0 - черный      8 - серый
    // 1 - синий       9 - ярко-синий
    // 2 - зеленый     10 - ярко-зеленый
    // 3 - голубой     11 - ярко-голубой
    // 4 - красный     12 - ярко-красный
    // 5 - фиолетовый  13 - ярко-фиолетовый
    // 6 - желтый      14 - ярко-желтый
    // 7 - белый       15 - ярко-белый

// game state
int runningApp = 1;
int runningGame = 1;
float gameTime = 0;

//Input
int moveForward = 0;
int moveLeft = 0;
int moveRight = 0;
int moveBack = 0;

//Player
float playerX = WIDTH / 2, playerY = HEIGHT - 10;
float speed = 20;
int playerHP = 3;
float timeLastHit = 0;
float canHitDelay = 1;

//Score
float score = 0;
float bestScore = 0;
float combo = 0;
float comboTimer = 0;

//statistic
float bestTime = 0;

float Dist(float  p1x, float p1y, float p2x, float p2y)
{
    return sqrt(pow(p1x - p2x, 2) + pow(p1y - p2y, 2));
}

void GameOver()
{
    runningGame = 0;

    if (score > bestScore) bestScore = score;

    char bestScoreStr[50];
    snprintf(bestScoreStr, sizeof(bestScoreStr), "BEST SCORE: %.0f", bestScore);
    DrawTextt(WIDTH / 2, HEIGHT / 2 + 6, bestScoreStr, 11);

    char currentScoreStr[50];
    snprintf(currentScoreStr, sizeof(currentScoreStr), "SCORE: %.0f", score);
    DrawTextt(WIDTH / 2, HEIGHT / 2 + 8, currentScoreStr, 7);

    DrawTextt(WIDTH / 2, HEIGHT / 2, "GAME OVER", 12);
    DrawTextt(WIDTH / 2, HEIGHT - 6, "Press SPACE to restart", 7);
    DrawHP(playerHP);
    RenderChanges();
}

void Hit()
{
    playerHP -= 1;

    if (playerHP <= 0)
        GameOver();
}

void CheckCollisions()
{
    if (gameTime - timeLastHit <= canHitDelay) return;

    for (int i = 0; i < rocks.count; i++)
    {
        Rock* rock = rocklist_get(&rocks, i);
        if (rock->x < playerX + 3 && rock->x + rock->size > playerX &&
            rock->y < playerY + 3 && rock->y + rock->size > playerY)
        {
            timeLastHit = gameTime;
            Hit();
        }
    }
}

void Draw()
{
    if (!runningGame) return;

    ClearNewBuffer();

    for (int i = 0; i < rocks.count; i++)
    {
        Rock* rock = rocklist_get(&rocks, i);
        DrawRock(rock->x, rock->y, rock->size, rock->shape, rock->color);
    }

    DrawPlayer(playerX, playerY);
    DrawHP(playerHP);
    DrawScore(score, combo);

    char timeStr[10];
    snprintf(timeStr, sizeof(timeStr), "%d:%02d", (int)gameTime / 60, (int)gameTime % 60);
    DrawTextt(WIDTH / 2, HEIGHT - 2, timeStr, 7);

    RenderChanges();
}

void UpdateRocks(float deltaTime)
{
    frameCounter++;
    if (frameCounter >= spawnRate && rocks.count < maxRocks)
    {
        SpawnRock();
        frameCounter = 0;
    }

    if (ACR >= RockStatsAddingRatePerSec)
    {
        RockSpeed += RockSpeedAddPerTime;
        maxRocks += RockCountAddPerTime;
        if (spawnRate >= 1)
            spawnRate--;
        ACR = 0;
    }
    ACR++;

    for (int i = 0; i < rocks.count; i++)
    {
        Rock* rock = rocklist_get(&rocks, i);
        rock->y += RockSpeed * rock->ySpeed * deltaTime;
        rock->x += rock->xSpeed * deltaTime;

        if (Dist(rock->x, rock->y, playerX, playerY) < MANEUVER_DISTANCE)
        {
            combo += 0.01;
            comboTimer = COMBO_TIME;
            score += combo;
        }

        if (rock->y >= HEIGHT)
        {
            rocklist_remove_at(&rocks, i);
            i--;
        }
    }
}

///////////////////////
void Update(float deltaTime)
{
    gameTime += deltaTime;

    if (moveForward && playerY > 0)
    {
        playerY -= speed * deltaTime;
    }

    if (moveBack && playerY < HEIGHT - 3)
    {
        playerY += speed * deltaTime;
    }

    if (moveRight && playerX < WIDTH - 3)
    {
        playerX += speed * 2.5 * deltaTime;
    }

    if (moveLeft && playerX > 0)
    {
        playerX -= speed * 2.5 * deltaTime;
    }

    if (comboTimer > 0)
    {
        comboTimer -= deltaTime;
        if (comboTimer <= 0) combo = 0;
    }

    UpdateRocks(deltaTime);
    CheckCollisions();
    DrawScore(score, combo);
    Draw();
}

///////////////////////
void Start()
{
    HideCursor();
    SetFullscreen();
    memset(currentScreen, 0, sizeof(currentScreen));
    ClearNewBuffer();

    rocklist_init(&rocks, maxRocks);
}

void RestartGame()
{
    runningGame = 1;
    ClearNewBuffer();
    score = 0;
    combo = 0;
    comboTimer = 0;

    rocklist_clear(&rocks);

    playerX = WIDTH / 2;
    playerY = HEIGHT - 10;
    playerHP = 3;
    timeLastHit = 0;

    RockSpeed = ROCK_SPEED;
    maxRocks = MAX_ROCKS;
    spawnRate = SPAWN_RATE;

    gameTime = 0;
}

void HandleInput()
{
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        runningApp = 0;
    }

    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !runningGame)
    {
        RestartGame();
    }

    moveForward = ((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000)) ? 1 : 0;
    moveBack = ((GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000)) ? 1 : 0;
    moveLeft = ((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000)) ? 1 : 0;
    moveRight = ((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) ? 1 : 0;
}


int main()
{
    clock_t prev = clock();

    srand(time(0));
    Start();

    while (runningApp)
    {
        clock_t curr = clock();
        float deltaTime = (float)(curr - prev) / CLOCKS_PER_SEC;
        prev = curr;

        if (deltaTime > 0.1f) deltaTime = 0.1f;

        HandleInput();

        if (runningGame)
            Update(deltaTime);

        if (TARGET_FPS < 999)
        {
            clock_t after = clock();
            float frameTime = (float)(after - curr) / CLOCKS_PER_SEC;

            if (frameTime < TARGET_DELTA_TIME)
            {
                Sleep((DWORD)((TARGET_DELTA_TIME - frameTime) * 1000));
            }
        }
    }
    return 0;
}