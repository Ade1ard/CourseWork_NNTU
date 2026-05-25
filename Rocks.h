#ifndef ROCKS_H
#define ROCKS_H

#include <windows.h>

typedef struct
{
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;
    int shape;
    int color;
} Rock;

typedef struct
{
    Rock* items;
    int count;
    int capacity;
} RockList;

extern RockList rocks;
extern int maxRocks;
extern float RockSpeed;
extern int spawnRate;
extern int frameCounter;
extern int ACR;
extern int RockSpeedAddPerTime;
extern int RockCountAddPerTime;
extern int RockStatsAddingRatePerSec;

void rocklist_init(RockList* list, int initialCapacity);
Rock* rocklist_get(RockList* list, int index);
void rocklist_add(RockList* list, Rock rock);
void rocklist_remove_at(RockList* list, int index);
void rocklist_clear(RockList* list);
void SpawnRock();
void UpdateRocks(float deltaTime);
void CheckCollisions();

#endif