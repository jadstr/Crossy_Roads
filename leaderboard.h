#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>
#include <string>

using namespace std;

struct playerLeaderboard {
    string playerName;
    int score;
};

void savePlayer(string playerName, int score);
vector<playerLeaderboard> loadLeaderboard();

#endif