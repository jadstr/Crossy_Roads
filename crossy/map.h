#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <windows.h>
#include "player.h"

using namespace std;

// ==============================
// ANSI COLOR HELPER
// Much faster than Win32 API per-
// character calls. Call once at
// startup to enable VT sequences.
// ==============================
void enableANSI()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode  = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

// Map Win32 color index → ANSI fg code
static const char* ANSI_FG[] = {
    "\033[30m",   // 0  Black
    "\033[34m",   // 1  Dark Blue
    "\033[32m",   // 2  Dark Green
    "\033[36m",   // 3  Dark Cyan
    "\033[31m",   // 4  Dark Red
    "\033[35m",   // 5  Dark Magenta
    "\033[33m",   // 6  Dark Yellow
    "\033[37m",   // 7  Light Grey
    "\033[90m",   // 8  Dark Grey
    "\033[94m",   // 9  Blue
    "\033[92m",   // 10 Green
    "\033[96m",   // 11 Cyan
    "\033[91m",   // 12 Red
    "\033[95m",   // 13 Magenta
    "\033[93m",   // 14 Yellow
    "\033[97m",   // 15 White
};

static const char* ANSI_BG[] = {
    "\033[40m",   // 0  Black bg
    "\033[44m",   // 1  Blue bg
    "\033[42m",   // 2  Green bg
    "\033[46m",   // 3  Cyan bg
    "\033[41m",   // 4  Red bg
};

// Append color escape to a string buffer (no cout call)
inline void appendColor(string& buf, int fg, int bg = 0)
{
    if (fg >= 0 && fg < 16) buf += ANSI_FG[fg];
    if (bg >= 1 && bg <= 4) buf += ANSI_BG[bg];
    else if (bg == 0)       buf += "\033[40m";
}

inline void appendReset(string& buf)
{
    buf += "\033[0m";
}

// Legacy wrappers kept so death animation & game-over still compile
void setColor(int fg, int bg = 0)
{
    if (fg >= 0 && fg < 16)   cout << ANSI_FG[fg];
    if (bg >= 1 && bg <= 4)   cout << ANSI_BG[bg];
    else if (bg == 0)         cout << "\033[40m";
}

void resetColor()
{
    cout << "\033[0m";
}

// Windows console color constants (foreground)
// 0  = Black        8  = Dark Grey
// 1  = Dark Blue    9  = Blue
// 2  = Dark Green   10 = Green
// 3  = Dark Cyan    11 = Cyan
// 4  = Dark Red     12 = Red
// 5  = Dark Magenta 13 = Magenta
// 6  = Dark Yellow  14 = Yellow
// 7  = Light Grey   15 = White

// ==============================
// OBSTACLE STRUCT
// ==============================
struct Obstacle
{
    int pos;
    int dir;
};

// ==============================
// LINKED LIST NODE
// ==============================
struct Node
{
    string lane;
    int    zoneType; // 0=finish,1=road,2=buffer,3=river,4=start
    Node*  next;
};

// ==============================
// CREATE NODE
// ==============================
Node* createNode(string text, int zone)
{
    Node* newNode = new Node;
    newNode->lane     = text;
    newNode->zoneType = zone;
    newNode->next     = NULL;
    return newNode;
}

// ==============================
// APPEND NODE
// ==============================
void appendNode(Node*& head, string text, int zone)
{
    Node* newNode = createNode(text, zone);

    if (head == NULL)
    {
        head = newNode;
        return;
    }

    Node* temp = head;

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
}

// ==============================
// FREE LIST
// ==============================
void freeList(Node*& head)
{
    while (head != NULL)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// ==============================
// ROAD LANE
// ==============================
string buildRoadLane(int row,
                     vector<Obstacle> roadObstacles[],
                     int playableWidth)
{
    string lane = "|";

    for (int i = 0; i < playableWidth; i++)
        lane += '.';

    lane += "|";

    for (int obsIndex = 0;
         obsIndex < (int)roadObstacles[row].size();
         obsIndex++)
    {
        Obstacle& obs = roadObstacles[row][obsIndex];

        for (int j = 0; j < 5; j++)
        {
            int p = obs.pos + j;

            if (p >= 1 && p <= playableWidth)
                lane[p] = '#';
        }
    }

    return lane;
}

// ==============================
// RIVER LANE
// ==============================
string buildRiverLane(int row,
                      vector<Obstacle> riverObstacles[],
                      int playableWidth)
{
    string lane = "|";

    for (int i = 0; i < playableWidth; i++)
        lane += '~';

    lane += "|";

    for (int obsIndex = 0;
         obsIndex < (int)riverObstacles[row].size();
         obsIndex++)
    {
        Obstacle& obs = riverObstacles[row][obsIndex];

        for (int j = 0; j < 4; j++)
        {
            int p = obs.pos + j;

            if (p >= 1 && p <= playableWidth)
                lane[p] = '=';
        }
    }

    return lane;
}

// ==============================
// OTHER LANES
// ==============================
string buildBufferLane()
{
    return "|........................................|";
}

string buildFinishLine()
{
    return "|========================================|";
}

string buildStartLane()
{
    return "|........................................|";
}

// ==============================
// ZONE MAP
// ==============================
void initializeZoneMap(int zoneMap[])
{
    zoneMap[0]  = 0;   // finish line
    zoneMap[1]  = 1;   // road
    zoneMap[2]  = 1;   // road
    zoneMap[3]  = 1;   // road
    zoneMap[4]  = 1;   // road
    zoneMap[5]  = 1;   // road
    zoneMap[6]  = 2;   // buffer / grass
    zoneMap[7]  = 3;   // river
    zoneMap[8]  = 3;   // river
    zoneMap[9]  = 2;   // buffer / grass
    zoneMap[10] = 1;   // road
    zoneMap[11] = 1;   // road
    zoneMap[12] = 1;   // road
    zoneMap[13] = 1;   // road
    zoneMap[14] = 1;   // road
    zoneMap[15] = 2;   // buffer / grass
    zoneMap[16] = 3;   // river
    zoneMap[17] = 3;   // river
    zoneMap[18] = 2;   // buffer / grass
    zoneMap[19] = 4;   // start
}

// ==============================
// INITIALIZE OBSTACLES
// ==============================
void initializeObstacles(int zoneMap[],
                         vector<Obstacle> roadObstacles[],
                         vector<Obstacle> riverObstacles[],
                         int totalRows)
{
    const int PLAYABLE = 40;

    for (int row = 0; row < totalRows; row++)
    {
        if (zoneMap[row] == 1)
        {
            int dir = (row % 2 == 0) ? 1 : -1;

            const int TRUCK_W = 5;
            const int GAP     = 2;
            const int STEP    = TRUCK_W + GAP;

            vector<int> candidates;
            for (int p = 1; p + TRUCK_W - 1 <= PLAYABLE; p += STEP)
                candidates.push_back(p);

            random_shuffle(candidates.begin(), candidates.end());

            int count = 2 + rand() % 2;
            if (count > (int)candidates.size())
                count = (int)candidates.size();

            for (int t = 0; t < count; t++)
            {
                Obstacle obs;
                obs.pos = candidates[t];
                obs.dir = dir;
                roadObstacles[row].push_back(obs);
            }
        }
        else if (zoneMap[row] == 3)
        {
            int dir = (row % 2 == 0) ? -1 : 1;

            const int LOG_W = 4;
            const int GAP   = 2;
            const int STEP  = LOG_W + GAP;

            vector<int> candidates;
            for (int p = 1; p + LOG_W - 1 <= PLAYABLE; p += STEP)
                candidates.push_back(p);

            random_shuffle(candidates.begin(), candidates.end());

            int count = 2 + rand() % 2;
            if (count > (int)candidates.size())
                count = (int)candidates.size();

            for (int t = 0; t < count; t++)
            {
                Obstacle obs;
                obs.pos = candidates[t];
                obs.dir = dir;
                riverObstacles[row].push_back(obs);
            }
        }
    }
}

// ==============================
// BUILD ROAD
// ==============================
void buildRoad(Node*& head,
               int zoneMap[],
               vector<Obstacle> roadObstacles[],
               vector<Obstacle> riverObstacles[],
               int totalRows,
               int playableWidth)
{
    head = NULL;

    for (int row = 0; row < totalRows; row++)
    {
        switch (zoneMap[row])
        {
        case 0:
            appendNode(head, buildFinishLine(), 0);
            break;
        case 1:
            appendNode(head, buildRoadLane(row, roadObstacles, playableWidth), 1);
            break;
        case 2:
            appendNode(head, buildBufferLane(), 2);
            break;
        case 3:
            appendNode(head, buildRiverLane(row, riverObstacles, playableWidth), 3);
            break;
        case 4:
            appendNode(head, buildStartLane(), 4);
            break;
        }
    }
}

// ==============================
// PLACE PLAYER
// ==============================
void placePlayer(Node* head,
                 Player& player,
                 int playableWidth)
{
    Node* temp = head;
    int row = 0;

    while (temp != NULL)
    {
        if (row == player.getY())
        {
            if (player.getX() >= 1 &&
                player.getX() <= playableWidth)
            {
                temp->lane[player.getX()] = 'P';
            }
        }

        temp = temp->next;
        row++;
    }
}

// ==============================
// DISPLAY ROAD (buffered, fast)
// Builds entire frame into one
// string then flushes once —
// eliminates per-char API calls.
// ==============================
void displayRoad(Node* head,
                 int lives,
                 int crossings,
                 const string& playerName)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(hConsole, cursorPosition);

    // Reserve ~4 KB up front to avoid reallocs
    string buf;
    buf.reserve(4096);

    // ---- Header (written into buf) ----
    appendColor(buf, 15, 0);
    buf += "---------- Road Crossing Challenge ----------\n";
    appendColor(buf, 14, 0);
    buf += "Player: ";
    buf += playerName;
    appendColor(buf, 12, 0);
    buf += " | Lives: ";
    buf += to_string(lives);
    appendColor(buf, 10, 0);
    buf += " | Crossings: ";
    buf += to_string(crossings);
    appendReset(buf);
    buf += "   \n";

    // ---- Lanes ----
    Node* temp = head;
    while (temp != NULL)
    {
        const string& lane = temp->lane;
        int zone = temp->zoneType;

        appendColor(buf, 7, 0);
        buf += lane[0];

        for (int i = 1; i < (int)lane.size() - 1; i++)
        {
            char c = lane[i];
            switch (c)
            {
            case 'P': appendColor(buf, 15, 1); break;
            case '#': appendColor(buf, 12, 0); break;
            case '~': appendColor(buf, 11, 0); break;
            case '=': appendColor(buf, (zone == 3) ? 6 : 10, 0); break;
            case '.': appendColor(buf, (zone == 1) ? 8 : 2, 0);  break;
            default:  appendReset(buf); break;
            }
            buf += c;
        }

        appendColor(buf, 7, 0);
        buf += lane[lane.size() - 1];
        appendReset(buf);
        buf += '\n';

        temp = temp->next;
    }

    appendReset(buf);

    // Single write for the whole frame — no per-char API calls
    cout.write(buf.c_str(), buf.size());
    cout.flush();

}

// ==============================
// MOVE OBSTACLES
// ==============================
void moveObstacles(int zoneMap[],
                   vector<Obstacle> roadObstacles[],
                   vector<Obstacle> riverObstacles[],
                   int totalRows,
                   int playableWidth)
{
    for (int row = 0; row < totalRows; row++)
    {
        if (zoneMap[row] == 1)
        {
            for (int obsIndex = 0;
                 obsIndex < (int)roadObstacles[row].size();
                 obsIndex++)
            {
                Obstacle& obs = roadObstacles[row][obsIndex];
                obs.pos += obs.dir;

                if (obs.pos > playableWidth)
                    obs.pos = 1 - 5;
                else if (obs.pos + 5 - 1 < 1)
                    obs.pos = playableWidth;
            }
        }
        else if (zoneMap[row] == 3)
        {
            for (int obsIndex = 0;
                 obsIndex < (int)riverObstacles[row].size();
                 obsIndex++)
            {
                Obstacle& obs = riverObstacles[row][obsIndex];
                obs.pos += obs.dir;

                if (obs.pos > playableWidth)
                    obs.pos = 1 - 4;
                else if (obs.pos + 4 - 1 < 1)
                    obs.pos = playableWidth;
            }
        }
    }
}

// ==============================
// CARRY PLAYER ON LOG
// ==============================
void carryPlayerOnLog(int zoneMap[],
                      vector<Obstacle> riverObstacles[],
                      Player& player,
                      int& lives,
                      int totalRows,
                      int playableWidth)
{
    int row = player.getY();

    if (row < 0 || row >= totalRows) return;
    if (zoneMap[row] != 3)           return;

    const int LOG_W = 4;

    for (int obsIndex = 0;
         obsIndex < (int)riverObstacles[row].size();
         obsIndex++)
    {
        Obstacle& obs = riverObstacles[row][obsIndex];

        if (player.getX() >= obs.pos &&
            player.getX() <= obs.pos + LOG_W - 1)
        {
            bool safe = player.carryX(obs.dir, playableWidth);

            if (!safe)
            {
                lives--;
                player.resetPosition(20, 19);
            }

            return;
        }
    }
}

// ==============================
// DEATH ANIMATION
// Flashes the player's position
// red/white 3 times, ~400 ms
// ==============================
void playDeathAnimation(Node* head,
                        int lives,
                        int crossings,
                        const string& playerName,
                        int px,
                        int py,
                        int playableWidth)
{
    // Blink the player cell between red 'X' and normal
    for (int flash = 0; flash < 3; flash++)
    {
        // Flash: red X
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD cur; cur.X = 0; cur.Y = 0;
            SetConsoleCursorPosition(hConsole, cur);

            setColor(15, 0);
            cout << "---------- Road Crossing Challenge ----------\n";

            setColor(14, 0);
            cout << "Player: " << playerName;
            setColor(12, 0);
            cout << " | Lives: " << lives;
            setColor(10, 0);
            cout << " | Crossings: " << crossings;
            resetColor();
            cout << "   " << endl;

            Node* temp = head;
            int row = 0;

            while (temp != NULL)
            {
                const string& lane = temp->lane;
                int zone = temp->zoneType;

                setColor(7, 0);
                cout << lane[0];

                for (int i = 1; i < (int)lane.size() - 1; i++)
                {
                    char c = lane[i];

                    // Flash the player tile
                    if (row == py && i == px)
                    {
                        setColor(15, 4); // white on red
                        cout << 'X';
                    }
                    else if (c == '#')
                    {
                        setColor(12, 0); cout << c;
                    }
                    else if (c == '~')
                    {
                        setColor(11, 0); cout << c;
                    }
                    else if (c == '=')
                    {
                        if (zone == 3) { setColor(6,  0); cout << c; }
                        else           { setColor(10, 0); cout << c; }
                    }
                    else if (c == '.')
                    {
                        if (zone == 1) { setColor(8, 0); cout << c; }
                        else           { setColor(2, 0); cout << c; }
                    }
                    else
                    {
                        resetColor(); cout << c;
                    }
                }

                setColor(7, 0);
                cout << lane[lane.size() - 1];
                resetColor();
                cout << endl;

                temp = temp->next;
                row++;
            }
        }

        Sleep(130);

        // Flash off: just re-render normal (player tile as blank)
        displayRoad(head, lives, crossings, playerName);
        Sleep(130);
    }
}

// ==============================
// CHECK COLLISION
// ==============================
void checkCollision(int zoneMap[],
                    Node* head,
                    int& lives,
                    Player& player,
                    int& crossings)
{
    int z = zoneMap[player.getY()];

    Node* temp = head;
    int row = 0;

    while (temp != NULL && row != player.getY())
    {
        temp = temp->next;
        row++;
    }

    if (temp == NULL) return;

    int px = player.getX();
    char cell = ' ';

    if (px >= 0 && px < (int)temp->lane.size())
        cell = temp->lane[px];

    if (z == 1)
    {
        if (cell == '#')
        {
            // Save position before reset for animation
            int deathX = player.getX();
            int deathY = player.getY();

            lives--;
            playDeathAnimation(head, lives, crossings,
                               player.getName(),
                               deathX, deathY, 40);
            player.resetPosition(20, 19);
        }
    }
    else if (z == 3)
    {
        if (cell == '~')
        {
            int deathX = player.getX();
            int deathY = player.getY();

            lives--;
            playDeathAnimation(head, lives, crossings,
                               player.getName(),
                               deathX, deathY, 40);
            player.resetPosition(20, 19);
        }
    }
    else if (z == 0)
    {
        crossings++;
        player.resetPosition(20, 19);
    }
}

#endif
