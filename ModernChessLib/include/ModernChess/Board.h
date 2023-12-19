#pragma once

#include "BitBoardConstants.h"
#include "CastlingRights.h"
#include "Color.h"

#include <array>
#include <ostream>

namespace ModernChess
{
    class Board
    {
    public:
        // Every figure-type and every color has its own board representation
        std::array<BitBoardState, 12> bitboards{};
        // Occupancies for white, black and both colors
        std::array<BitBoardState, 3> occupancies{};

        Square enPassantTarget = Square::undefined;
        Color sideToMove = Color::White;
        CastlingRights castlingRights = CastlingRights::Gone;

        bool operator==(const Board &other) const = default;
    };
}

std::ostream& operator<<(std::ostream& os, const ModernChess::Board &bitBoard);