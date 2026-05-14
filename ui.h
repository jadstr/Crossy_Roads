#ifndef UI_H
#define UI_H

void runUI();
int chooseDifficulty(string& playerName);
void gotoxy(int x, int y);
void textCenter(const string& text, int textColor = 15);
void gameConsole(string up = "    ", string down = "    ", string enter = "    ", string right = "      ", string left = "      ", int delay = 60);
void gameOverScreen(const string& playerName, int crossings);

#endif