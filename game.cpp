#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <string>

#include "player.h"
#include "map.h"
#include "leaderboard.h"
#include "ui.h"

using namespace std;

// ==============================
// MAIN MENU
// Returns the sleep delay in ms
// based on chosen difficulty
// ==============================

void startGame(string playerName, int difficulty) {
    SetConsoleOutputCP(CP_UTF8);

    int sleepDelay;

    if (difficulty == 0)
        sleepDelay = 180; // EASY
    else if (difficulty == 1)
        sleepDelay = 120; // NORMAL
    else
        sleepDelay = 80;  // HARD

    int playableWidth = 40;
    int totalRows = 20;

    int zoneMap[20];

    vector<Obstacle> roadObstacles[20];
    vector<Obstacle> riverObstacles[20];

    Player player(21, 19, playerName);

    int lives = 3;
    int crossings = 0;

    srand(time(0));

    initializeZoneMap(zoneMap);

    initializeObstacles(
        zoneMap,
        roadObstacles,
        riverObstacles,
        totalRows
    );

    // Hide cursor for clean rendering
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    while (lives > 0)
    {
        Node* head = NULL;

        buildRoad(
            head,
            zoneMap,
            roadObstacles,
            riverObstacles,
            totalRows,
            playableWidth
        );

        // Save position before carryPlayerOnLog changes it
        int preCarryX = player.getX();
        int preCarryY = player.getY();

        carryPlayerOnLog(
            zoneMap,
            riverObstacles,
            player,
            lives,
            totalRows,
            playableWidth
        );

        // If carryPlayerOnLog killed the player (carried off edge),
        // play the death animation at their pre-carry position
        bool carriedOffEdge = (lives > 0 &&
                               player.getX() == 20 &&
                               player.getY() == 19 &&
                               preCarryY != 19);

        // Simpler check: if position reset happened due to carry
        // We detect it by checking if the player was in a river row
        // and is now back at start — handled by comparing positions
        // Actually: carryPlayerOnLog already decremented lives and reset.
        // We detect post-carry death by checking if player was in river
        // and is now at start row (19) after being elsewhere.
        // Use a flag approach instead.

        if (lives > 0)
        {
            checkCollision(
                zoneMap,
                head,
                lives,
                player,
                crossings
            );
        }

        placePlayer(head, player, playableWidth);

        displayRoad(head, lives, crossings, player.getName());

        freeList(head);

        player.handleInput(totalRows, playableWidth);

        moveObstacles(
            zoneMap,
            roadObstacles,
            riverObstacles,
            totalRows,
            playableWidth
        );

        Sleep(sleepDelay);
    }

    // Show cursor again on game over
    CONSOLE_CURSOR_INFO cursorInfo2;
    cursorInfo2.dwSize = 1;
    cursorInfo2.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo2);

    resetColor();
    savePlayer(playerName, crossings);
    gameOverScreen(playerName, crossings);
}

int main()
{
    runUI();   // start menu system
    return 0;
}