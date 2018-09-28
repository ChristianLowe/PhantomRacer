#pragma once

#include <iostream>

#include "board.h"

using std::cin;
using std::cout;
using std::endl;
using std::flush;

void showIntroText() {
    cout << endl
         << "...What’s this? A mystery car has just entered the race" << endl
         << "      and he’s coming from behind, like a bullet!" << endl;
}

PieceRange getStartingParticipant() {
    while (true) {
        cout << endl << "Who goes first? Type cpu or human: " << flush;

        std::string first;
        cin >> first;
        std::transform(first.begin(), first.end(), first.begin(), ::tolower);

        if (first == "c" || first == "cpu" || first == "computer") {
            cout << "The computer goes first." << endl;
            return PieceRange::CpuAi;
        } else if (first == "h" || first == "human") {
            cout << "The player goes first." << endl;
            return PieceRange::Human;
        }
    }
}
