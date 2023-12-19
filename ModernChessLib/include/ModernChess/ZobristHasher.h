#pragma once

#include "GlobalConstants.h"

#include <array>

namespace ModernChess {
    class Board;

    class ZobristHasher {
    public:
        ZobristHasher();

        static uint64_t generateHash(const Board &board);

        // random piece keys [piece][square]
        static std::array<std::array<uint64_t, NumberOfSquares>, NumberOfFigureTypes> pieceKeys;

        // random en passant keys [square]
        static std::array<uint64_t, NumberOfSquares> enpassantKeys;

        // random castling keys
        static std::array<uint64_t, 16> castleKeys;

        // random side key
        static uint64_t sideKey;
    };
}