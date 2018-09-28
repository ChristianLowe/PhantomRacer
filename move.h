#pragma once

#include <algorithm>

#include "types.h"

struct Move {
    u8 x1, y1, x2, y2;

    Move inverse() const {
        return Move {
                static_cast<u8>(6 - x1),
                static_cast<u8>(7 - y1),
                static_cast<u8>(6 - x2),
                static_cast<u8>(7 - y2),
        };
    }
};

std::ostream& operator<<(std::ostream &stream, const Move &move) {
    // Ignore IDE warning that casting is redundant (it isn't (seriously))
    return stream
            << static_cast<char>('A' + move.x1)
            << static_cast<char>('8' - move.y1)
            << static_cast<char>('A' + move.x2)
            << static_cast<char>('8' - move.y2);
}

std::istream& operator>>(std::istream &stream, Move &move) {
    // Read move in and make it upper-case
    std::string str;
    stream >> str;

    if (str.size() == 4) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        // Set move relative to ASCII codes used
        move.x1 = static_cast<u8>(str[0] - 'A');
        move.x2 = static_cast<u8>(str[2] - 'A');
        move.y1 = static_cast<u8>('8' - str[1]);
        move.y2 = static_cast<u8>('8' - str[3]);
    }

    return stream;
}

inline bool operator==(const Move& lhs, const Move& rhs)
{
    return lhs.x1 == rhs.x1 && lhs.x2 == rhs.x2
           && lhs.y1 == rhs.y1 && lhs.y2 == rhs.y2;
}
