#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <conio.h>
#include <windows.h>
#include "leaderboard.h"
#include "game.h"
using namespace std;

const int WIDTH = 68;

//for entering name inside the design
void gotoxy(int x, int y) {
    COORD coord;

    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//to set colors
void color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void line() {
    color(13);
    cout << "[|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|]" << endl;
}

void printBorder(const string& content, int textColor = 15) {

    color(13); 
    cout << "[|]";

    color(textColor);
    cout << content;

    color(13); 
    cout << "[|]" << endl;

    color(7); 
}

void printCentered(const string& text, int textColor = 15) {

    int padding = (WIDTH - 2 - text.length()) / 2;

    string content =
        string(padding, ' ') +
        text +
        string(WIDTH - 2 - padding - text.length(), ' ');

    printBorder(content, textColor);
}

void emptyLine() {
    printBorder(string(WIDTH - 2, ' '));
}

void menuTitle(int select) {
    string choice[4] = {
        "S T A R T",
        "T U T O R I A L",
        "L E A D E R B O A R D",
        "E X I T"
    };

    for (int i = 0; i < 4; i++) {

        string label = (i == select ? ">> " : "   ") + choice[i];

        int padding = WIDTH - 2 - label.length();

        string content = string(padding / 2, ' ') + label + string(padding - padding / 2, ' ');

        int textColor;

        if (i == select)
            textColor = 11; // aqua
        else
            textColor = 8; 
        
            
        printBorder(content, textColor);

        if (i != 3)
            printBorder(string(WIDTH - 2, ' '), 7);
    }
    emptyLine();
    emptyLine();
    emptyLine();
    emptyLine();
}


void title() {
    emptyLine();
    printCentered("_________________________________________", 11);
    printCentered("|           ____                 _        |", 11);
    printCentered("|         |  _ | ___   __ _  __| |        |", 11);
    printCentered("|         | |_) / _ | / _` |/ _` |        |", 11);
    printCentered("|         |  _ < (_) | (_| | (_| |        |", 11);
    printCentered("|         |_| |_|___/|__,_||__,__|        |", 11);
    printCentered("|   ____                   _              |", 11);
    printCentered("|  / ___|_ __ ___  ___ ___(_)_ ___  __ _  |", 11);
    printCentered("| | |   | '__/ _ || __/ __| | '_  |/ _` | |", 11);
    printCentered("| | |___| | | (_) |__ |__ | | | | | (_| | |", 11);
    printCentered("| |_____|_| |___/|___/___/|_|_||_||__,  | |", 11);
    printCentered("|          C h a l l e n g e       |___/  |", 11);
    printCentered("|_________________________________________|", 11);
    emptyLine();
    emptyLine();
}


void console(string up = "    ", string down = "    ", string enter = "    ") {
    color(11);
    cout << "[|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|]" << endl;
    cout << "[|]                                                                  [|]" << endl;
    cout << "[|]                                                                  [|]" << endl;
    cout << "[|]           ____           __        __           ____             [|]" << endl;
    cout << "[|]          |" << up << "|         (__)      (__)         (" << enter << ")            [|]" << endl;
    cout << "[|]          |" << up << "|         START     MENU         (____)            [|]" << endl;
    cout << "[|]    ______|" << up << "|______                    ____          ____      [|]" << endl;
    cout << "[|]   |                  |                  (    )        (    )     [|]" << endl;
    cout << "[|]   |______      ______|                  (____)        (____)     [|]" << endl;
    cout << "[|]          |" << down << "|                                 ____             [|]" << endl;
    cout << "[|]          |" << down << "|                                (    )            [|]" << endl;
    cout << "[|]          |____|                                (____)            [|]" << endl;
    cout << "[|]                        ______    ______                          [|]" << endl;
    cout << "[|]                       (______)  (______)                         [|]" << endl;
    cout << "[|]                                                                  [|]" << endl;
    cout << "[|]                                                                  [|]" << endl;
    cout << "[|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|][|]" << endl;

    color(7);
}

void draw(int select, string up = "    ", string down = "    ", string enter = "    ") {
    system("cls");
    line();
    title();
    menuTitle(select);
    line();
    console(up, down, enter);
}


void startScreen(const string& name = "", bool showConsole = true) {
    system("cls");
    line();
    title();
    if (name == "") {
        printCentered("E N T E R   N A M E : ");
        emptyLine();
        emptyLine();
        emptyLine();
        emptyLine();
        emptyLine();
        emptyLine();
        emptyLine();
    }
    else {
        printCentered("P L A Y E R : " + name);
        emptyLine();
        printCentered("S E L E C T   D I F F I C U L T Y : ");
        printCentered(" ");
        emptyLine();
        emptyLine();
        emptyLine();
        emptyLine();
    }
    emptyLine();
    emptyLine();
    line();

    if (showConsole)
        console();
}

void backMessage() {
    emptyLine();

    color(11);
    printCentered("(P R E S S   A N Y   K E Y   T O   R E T U R N   M E N U...)");

    color(7);

    emptyLine();
    line();
    console();
    _getch();
}


void tutorialScreen() {
    system("cls");
    line();
    title();

    printCentered(" H O W  T O  P L A Y :");
    emptyLine();
    printCentered("___________________________________________");
    printCentered("| - Arrow Keys: UP / DOWN / LEFT / RIGHT   |");
    printCentered(" | - Dodge trucks (#####) in the ROAD ZONE  | ");
    printCentered(" | - Hop on logs (====) in the RIVER ZONE   | ");
    printCentered(" | - Reach the finish line 5 times to win!  | ");
    printCentered(" __________________________________________");

    backMessage();
}

void leaderScreen() {
    system("cls");

    line();
    title();

    printCentered(" L E A D E R B O A R D:");
    printCentered(" ");

    vector<playerLeaderboard> players = loadLeaderboard();

    int limit;

    if (players.size() < 5)
        limit = players.size();
    else
        limit = 5;

    for (int i = 0; i < limit; i++) {

        string text =
            to_string(i + 1) + ". " + players[i].playerName + " - CROSSINGS: " + to_string(players[i].score);

        printCentered(text);
    }

    emptyLine();
    backMessage();
}

void exitScreen() {
    system("cls");
    line();
    title();

    printCentered("T H A N K   Y O U   F O R   P L A Y I N G !", 15);
    emptyLine();
    printCentered("G O O D B Y E   P L A Y E R ! :)", 15);

    emptyLine();
    emptyLine();
    line();

    console("####", "####", "####");
}


void difficultyFlash(string up = "    ", string down = "    ", string enter = "    ", int delay = 100) {
    gotoxy(0, 28); 

    console(up, down, enter);

    Sleep(delay);

    gotoxy(0, 28);

    console();
}

int chooseDifficulty(string& playerName) {
    int difficulty = 0;

    string diff[3] = {
        "E A S Y",
        "N O R M A L",
        "H A R D"
    };

    while (true) {
        startScreen(playerName, false);
        console();

        //to print difficulty choices inside
        for (int i = 0; i < 3; i++) {
            string label;

            if (i == difficulty) {
                color(11);
                label = ">> " + diff[i];
            }
            else {
                color(8);
                label = "   " + diff[i];
            }
            
           // centered
            int x = (WIDTH / 2) - (label.length() / 2);

            // proper vertical spacing
            int y = 21 + i * 2;

            gotoxy(x, y);

            cout << label;
        }

        color(7);

        int key = _getch();

        if (key == 224) {

            key = _getch();

            if (key == 72) { // UP
                difficulty = (difficulty - 1 + 3) % 3;
                difficultyFlash("####");
            }

            else if (key == 80) { // DOWN
                difficulty = (difficulty + 1) % 3;
                difficultyFlash("    ","####");
            }
        }

        else if (key == 13) { // ENTER
            difficultyFlash("    ", "    ", "####");
            return difficulty;
        }
    }
}
void gameFrame() {
    system("cls");
    line();
    emptyLine();

    // GAME AREA
    for (int i = 0; i < 24; i++) {
        printBorder(string(WIDTH - 2, ' '));
    }

    line();
    console();
}

void clearGameArea() {

    for (int y = 12; y <= 35; y++) {

        gotoxy(10, y);

        cout << string(90, ' ');
    }
}

void gameOverScreen(const string& playerName, int crossings) {
    system("cls");
    line();
    title();

    printCentered("G A M E   O V E R", 12);
    emptyLine();
    printCentered("P L A Y E R : " + playerName, 14);
    printCentered("T O T A L   C R O S S I N G S : " + to_string(crossings), 10);
    emptyLine();

    //to show leaderboard only
    printCentered(" L E A D E R B O A R D:");
    printCentered(" ");

    vector<playerLeaderboard> players = loadLeaderboard();

    int limit;

    if (players.size() < 5)
        limit = players.size();
    else
        limit = 5;

    for (int i = 0; i < limit; i++) {

        string text =
            to_string(i + 1) + ". " + players[i].playerName + " - CROSSINGS: " + to_string(players[i].score);

        printCentered(text);
    }

    emptyLine();
    printCentered("(P R E S S   A N Y   K E Y   T O   R E T U R N   M E N U...)");
    emptyLine();
    line();

    console("####", "####", "####");

    _getch();
}

void startChoice() {

    string playerName;

    startScreen(playerName);

    // centered input
    gotoxy(32, 18);

    getline(cin, playerName);

    startScreen(playerName);

    int difficulty = chooseDifficulty(playerName);

    clearGameArea();
    gameFrame();
    startGame(playerName, difficulty);
}

void consoleFlash(int select, string up = "    ", string down = "    ", string enter = "    ", int delay = 100) {
    draw(select, up, down, enter);
    Sleep(delay);
    draw(select);
}

void runUI() {
    int select = 0;

    bool running = true;

    string up = "    ";
    string down = "    ";
    string enter = "    ";

    while (running) {

        draw(select, up, down, enter);

        int key = _getch();

        if (key == 224) {

            key = _getch();

            // UP
            if (key == 72) {

                select = (select - 1 + 4) % 4;
                consoleFlash(select, "####");
            }

            // DOWN
            else if (key == 80) {

                select = (select + 1) % 4;
                consoleFlash(select, "    ", "####");
            }
        }

        // ENTER
        else if (key == 13) {

            consoleFlash(select, "    ", "    ", "####");

            system("cls");

            if (select == 0) {

                startChoice();
            }

            else if (select == 1) {

                tutorialScreen();
            }

            else if (select == 2) {

                leaderScreen();
            }

            else if (select == 3) {
                exitScreen();
                Sleep(1800);
                running = false;
            }
        }
    }
}
