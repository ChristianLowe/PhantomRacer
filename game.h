#pragma once

enum class GameState {
    IsPlaying,
    BlackWins,
    WhiteWins,
};

enum class PieceRange {
    NoRange = 0,
    Black = 10,
    White = 20,
};
