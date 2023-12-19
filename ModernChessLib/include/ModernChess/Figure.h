#pragma once

#include <cinttypes>

namespace ModernChess {

    enum Figure : uint8_t
    {
        WhitePawn,
        WhiteKnight,
        WhiteBishop,
        WhiteRook,
        WhiteQueen,
        WhiteKing,
        BlackPawn,
        BlackKnight,
        BlackBishop,
        BlackRook,
        BlackQueen,
        BlackKing,
        None
    };

    // This makes it possible to use Figure in for-loops
    inline Figure &operator++(Figure &state)
    {
        state = Figure(uint8_t(state) + 1);
        return state;
    }

    inline Figure &operator--(Figure &state)
    {
        state = Figure(uint8_t(state) - 1);
        return state;
    }
}