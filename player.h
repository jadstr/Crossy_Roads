#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>
#include <conio.h>
#include <string>

using namespace std;

// ==============================
// PLAYER CLASS
// ==============================
class Player
{

private:
    int    x;
    int    y;
    string name;

    int    totalMoves;    // cumulative moves across whole game
    int    crossingMoves; // moves since last crossing reset
    int    lastkey; //added

public:

    // ==========================
    // CONSTRUCTOR
    // ==========================
    Player(int startX, int startY, string playerName = "Player")
    {
        x             = startX;
        y             = startY;
        name          = playerName;
        totalMoves    = 0;
        crossingMoves = 0;
        lastkey = 0;
    }

    // ==========================
    // GETTERS
    // ==========================
    int getX() { return x; }
    int getY() { return y; }

    string getName() { return name; }

    int getTotalMoves()    { return totalMoves;    }
    int getCrossingMoves() { return crossingMoves; }
    int getLastKey() { return lastkey; }

    // ==========================
    // RESET POSITION
    // Also resets crossing move
    // counter for the next run
    // ==========================
    void resetPosition(int startX, int startY)
    {
        x             = startX;
        y             = startY;
        crossingMoves = 0; // fresh count for next crossing
    }

    // ==========================
    // RESET CROSSING MOVES ONLY
    // Call after a successful
    // crossing is recorded
    // ==========================
    void resetCrossingMoves()
    {
        crossingMoves = 0;
    }

    // ==========================
    // CARRY PLAYER (LOG RIDING)
    // Log carry does NOT count
    // as a player move
    // ==========================
    bool carryX(int dir, int playableWidth)
    {
        x += dir;

        if (x < 1 || x > playableWidth)
        {
            return false;
        }

        return true;
    }

    // ==========================
    // HANDLE INPUT - REAL TIME
    // Counts every key press
    // that actually moves player
    // ==========================
    //game
    bool handleInput(int totalRows, int playableWidth)
    {
        if (_kbhit())
        {
            int key = _getch();

            while (_kbhit())
            {
                key = _getch();
            }

            if (key == 224)
            {
                key = _getch();
            }

            bool moved = false;

            switch (key)
            {
            case 72:
            case 'w':
            case 'W':
                if (y > 0)
                {
                    y--;
                    moved = true;
                    lastkey = 72;
                }
                break;

            case 80:
            case 's':
            case 'S':
                if (y < totalRows - 1)
                {
                    y++;
                    moved = true;
                    lastkey = 80;
                }
                break;

            case 75:
            case 'a':
            case 'A':
                if (x > 1)
                {
                    x--;
                    moved = true;
                    lastkey = 75;
                }
                break;

            case 77:
            case 'd':
            case 'D':
                if (x < playableWidth)
                {
                    x++;
                    moved = true;
                    lastkey = 77;
                }
                break;
            }

            if (moved)
            {
                totalMoves++;
                crossingMoves++;
                lastkey = key;
            }
            return moved;
        }
        return false;
    }
};

#endif