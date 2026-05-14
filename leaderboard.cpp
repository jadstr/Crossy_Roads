#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include "leaderboard.h"

using namespace std;

void savePlayer(string playerName, int score) {

    ofstream file("leaderboard.txt", ios::app);

    if (!file.is_open()) return;

    file << playerName << "\n";
    file << score << "\n";

    file.close();
}

vector<playerLeaderboard> loadLeaderboard() {

    ifstream file("leaderboard.txt");

    vector<playerLeaderboard> players;

    if (!file.is_open()) return players;

    playerLeaderboard p;

    while (getline(file, p.playerName)) {

        if (!(file >> p.score)) break;
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        players.push_back(p);
    }

    file.close();

    sort(players.begin(), players.end(), [](const playerLeaderboard& a, const playerLeaderboard& b) {
        return a.score > b.score;
    });

    return players;
}