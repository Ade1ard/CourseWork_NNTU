#include <windows.h>

#include "Rocks.h"
#include "Constants.h"

int maxRocks = MAX_ROCKS;
float RockSpeed = ROCK_SPEED;
int spawnRate = SPAWN_RATE;

//meta
int RockSpeedAddPerTime = 2;
int RockCountAddPerTime = 2;
int RockStatsAddingRatePerSec = 5 * TARGET_FPS;
int ACR = 0;

RockList rocks;
int frameCounter = 0;

void rocklist_init(RockList* list, int initialCapacity)
{
    list->capacity = initialCapacity;
    list->count = 0;
    list->items = (Rock*)malloc(sizeof(Rock) * list->capacity);
}

Rock* rocklist_get(RockList* list, int index)
{
    if (index >= 0 && index < list->count)
    {
        return &list->items[index];
    }
    return NULL;
}

void rocklist_expand(RockList* list)
{
    list->capacity += 1;
    list->items = (Rock*)realloc(list->items, sizeof(Rock) * list->capacity);
}

void rocklist_add(RockList* list, Rock rock)
{
    if (list->count >= list->capacity)
    {
        rocklist_expand(list);
    }
    list->items[list->count] = rock;
    list->count++;
}

void rocklist_remove_at(RockList* list, int index)
{
    if (index < 0 || index >= list->count) return;

    for (int i = index; i < list->count - 1; i++)
    {
        list->items[i] = list->items[i + 1];
    }
    list->count--;
}

void rocklist_clear(RockList* list)
{
    list->count = 0;
}

void SpawnRock()
{
    Rock newRock;

    newRock.x = rand() % (WIDTH - 2);
    newRock.y = 0;
    newRock.size = rand() % 3 + 1;
    newRock.shape = rand() % 3;
    newRock.color = (rand() % 2 == 0) ? 7 : 8;
    newRock.ySpeed = 1.0f + (float)rand() / RAND_MAX * 1.0f;

    if (50 >= rand() % 100)
        newRock.xSpeed = -2.0f + (float)(rand() % 4001) / 1000.0f;
    else
        newRock.xSpeed = 0;

    rocklist_add(&rocks, newRock);
}