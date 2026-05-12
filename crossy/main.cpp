#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <string>

#include "player.h"
#include "map.h"

using namespace std;

// ==============================
// MAIN MENU
// Returns the sleep delay in ms
// based on chosen difficulty
// ==============================
int showMainMenu(string& playerName)
{
    system("cls");

    cout << "============================================\n";
    cout << "        ROAD CROSSING CHALLENGE             \n";
    cout << "============================================\n\n";

    cout << "Enter your name: ";
    getline(cin, playerName);

    if (playerName.empty())
    {
        playerName = "Player";
    }

    cout << "\nHello, " << playerName << "!\n\n";

    cout << "Select Difficulty:\n";
    cout << "  [1] Easy   (slow traffic)\n";
    cout << "  [2] Medium (normal traffic)\n";
    cout << "  [3] Hard   (fast traffic)\n\n";
    cout << "Your choice: ";

    int choice = 0;

    while (choice < 1 || choice > 3)
    {
        string input;
        getline(cin, input);

        if (!input.empty())
        {
            choice = input[0] - '0';
        }

        if (choice < 1 || choice > 3)
        {
            cout << "Please enter 1, 2, or 3: ";
        }
    }

    int delay = 120;

    switch (choice)
    {
    case 1:
        delay = 180;
        cout << "\nDifficulty: Easy\n";
        break;
    case 2:
        delay = 120;
        cout << "\nDifficulty: Medium\n";
        break;
    case 3:
        delay = 100;
        cout << "\nDifficulty: Hard\n";
        break;
    }

    cout << "\nControls: W/A/S/D or Arrow Keys to move.\n";
    cout << "Reach the finish line (top) to score a crossing!\n";
    cout << "Avoid trucks (#####) and don't fall in the river (~~~~)!\n";
    cout << "Ride logs (====) to cross the river safely.\n\n";

    cout << "Press ENTER to start...";
    string dummy;
    getline(cin, dummy);

    return delay;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    string playerName;
    int sleepDelay = showMainMenu(playerName);

    system("cls");

    int playableWidth = 40;
    int totalRows = 20;

    int zoneMap[20];

    vector<Obstacle> roadObstacles[20];
    vector<Obstacle> riverObstacles[20];

    Player player(20, 19, playerName);

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

    cout << "\n\n========== GAME OVER ==========\n";
    cout << "Thanks for playing, " << playerName << "!\n";
    cout << "Crossings completed: " << crossings << endl;

    return 0;
}
