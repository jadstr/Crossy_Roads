#ifndef SCORES_H
#define SCORES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>

using namespace std;

// ==============================
// SCORE RECORD STRUCT
// ==============================
struct ScoreRecord
{
    string playerName;
    int    crossingNumber; // which crossing (1st, 2nd, ...)
    int    moves;          // moves taken for that crossing
    string date;
};

// ==============================
// GET CURRENT DATE STRING
// Format: YYYY-MM-DD
// ==============================
string getCurrentDate()
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    char buf[32];
    sprintf(buf, "%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
    return string(buf);
}

// ==============================
// CSV FILE NAME
// ==============================
const string CSV_FILE = "scores.csv";

// ==============================
// ENSURE CSV HEADER EXISTS
// Creates the file with header
// if it doesn't exist yet
// ==============================
void ensureCSVHeader()
{
    ifstream check(CSV_FILE.c_str());

    if (!check.good())
    {
        ofstream out(CSV_FILE.c_str());
        out << "PlayerName,CrossingNumber,Moves,Date\n";
        out.close();
    }
    else
    {
        check.close();
    }
}

// ==============================
// APPEND ONE CROSSING RECORD
// ==============================
void appendScoreRecord(const string& playerName,
                       int crossingNumber,
                       int moves,
                       const string& date)
{
    ensureCSVHeader();

    ofstream out(CSV_FILE.c_str(), ios::app);

    if (out.is_open())
    {
        out << playerName      << ","
            << crossingNumber  << ","
            << moves           << ","
            << date            << "\n";
        out.close();
    }
}

// ==============================
// LOAD ALL RECORDS FROM CSV
// ==============================
vector<ScoreRecord> loadAllRecords()
{
    vector<ScoreRecord> records;

    ifstream in(CSV_FILE.c_str());
    if (!in.is_open()) return records;

    string line;
    bool firstLine = true;

    while (getline(in, line))
    {
        if (firstLine)          // skip header
        {
            firstLine = false;
            continue;
        }

        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        ScoreRecord rec;

        // PlayerName
        if (!getline(ss, token, ',')) continue;
        rec.playerName = token;

        // CrossingNumber
        if (!getline(ss, token, ',')) continue;
        rec.crossingNumber = atoi(token.c_str());

        // Moves
        if (!getline(ss, token, ',')) continue;
        rec.moves = atoi(token.c_str());

        // Date
        if (!getline(ss, token, ',')) continue;
        rec.date = token;

        records.push_back(rec);
    }

    in.close();
    return records;
}

// ==============================
// DISPLAY LEADERBOARD
// Shows per-player best crossing
// (fewest moves), sorted asc
// ==============================
void displayLeaderboard(const string& currentPlayer)
{
    vector<ScoreRecord> all = loadAllRecords();

    if (all.empty())
    {
        cout << "(No records found.)\n";
        return;
    }

    // Collect unique player names
    vector<string> names;
    for (int i = 0; i < (int)all.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < (int)names.size(); j++)
        {
            if (names[j] == all[i].playerName)
            {
                found = true;
                break;
            }
        }
        if (!found) names.push_back(all[i].playerName);
    }

    // For each player: find their best (fewest moves) crossing
    // and total crossings
    struct PlayerSummary
    {
        string name;
        int    bestMoves;
        int    totalCrossings;
        string bestDate;
    };

    vector<PlayerSummary> summaries;

    for (int n = 0; n < (int)names.size(); n++)
    {
        PlayerSummary ps;
        ps.name           = names[n];
        ps.bestMoves      = 999999;
        ps.totalCrossings = 0;
        ps.bestDate       = "";

        for (int i = 0; i < (int)all.size(); i++)
        {
            if (all[i].playerName == names[n])
            {
                ps.totalCrossings++;
                if (all[i].moves < ps.bestMoves)
                {
                    ps.bestMoves = all[i].moves;
                    ps.bestDate  = all[i].date;
                }
            }
        }

        summaries.push_back(ps);
    }

    // Sort by best moves ascending (fewer = better)
    for (int i = 0; i < (int)summaries.size() - 1; i++)
    {
        for (int j = i + 1; j < (int)summaries.size(); j++)
        {
            if (summaries[j].bestMoves < summaries[i].bestMoves)
            {
                PlayerSummary tmp = summaries[i];
                summaries[i]      = summaries[j];
                summaries[j]      = tmp;
            }
        }
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Header row
    SetConsoleTextAttribute(hConsole, 14); // yellow
    cout << "\n";
    cout << "  RANK  ";
    cout << "NAME                 ";
    cout << "BEST MOVES  ";
    cout << "CROSSINGS  ";
    cout << "DATE\n";

    SetConsoleTextAttribute(hConsole, 8); // dark grey
    cout << "  ----  ";
    cout << "--------------------  ";
    cout << "----------  ";
    cout << "---------  ";
    cout << "----------\n";

    for (int i = 0; i < (int)summaries.size(); i++)
    {
        bool isCurrentPlayer = (summaries[i].name == currentPlayer);

        // Rank color: gold for #1, highlight for current player
        if (i == 0)
            SetConsoleTextAttribute(hConsole, 14); // yellow = gold
        else if (isCurrentPlayer)
            SetConsoleTextAttribute(hConsole, 11); // cyan = you
        else
            SetConsoleTextAttribute(hConsole, 7);  // white

        // Rank
        cout << "  #" << (i + 1);
        if (i + 1 < 10) cout << "    ";
        else             cout << "   ";

        // Name (pad to 21 chars)
        string displayName = summaries[i].name;
        if (isCurrentPlayer) displayName += " (you)";
        while ((int)displayName.size() < 21) displayName += ' ';
        if ((int)displayName.size() > 21)    displayName = displayName.substr(0, 21);
        cout << displayName << "  ";

        // Best moves
        string movStr = to_string(summaries[i].bestMoves);
        while ((int)movStr.size() < 10) movStr += ' ';
        cout << movStr << "  ";

        // Total crossings
        string cStr = to_string(summaries[i].totalCrossings);
        while ((int)cStr.size() < 9) cStr += ' ';
        cout << cStr << "  ";

        // Date
        cout << summaries[i].bestDate << "\n";
    }

    SetConsoleTextAttribute(hConsole, 7); // reset
}

// ==============================
// DISPLAY THIS SESSION'S SCORES
// Table of each crossing + moves
// ==============================
void displaySessionScores(const string& playerName,
                          const vector<int>& crossingMovesList)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 14); // yellow
    cout << "\n  YOUR CROSSINGS THIS SESSION:\n";

    SetConsoleTextAttribute(hConsole, 8);
    cout << "  Crossing  Moves\n";
    cout << "  --------  -----\n";

    int best = 999999;
    int bestIdx = -1;

    for (int i = 0; i < (int)crossingMovesList.size(); i++)
    {
        if (crossingMovesList[i] < best)
        {
            best    = crossingMovesList[i];
            bestIdx = i;
        }
    }

    for (int i = 0; i < (int)crossingMovesList.size(); i++)
    {
        if (i == bestIdx)
            SetConsoleTextAttribute(hConsole, 10); // green = best
        else
            SetConsoleTextAttribute(hConsole, 7);

        cout << "  #" << (i + 1);
        if (i + 1 < 10) cout << "       ";
        else             cout << "      ";
        cout << crossingMovesList[i];

        if (i == bestIdx)
        {
            SetConsoleTextAttribute(hConsole, 10);
            cout << "  <- best";
        }
        cout << "\n";
    }

    SetConsoleTextAttribute(hConsole, 7);
}

#endif
