#pragma once

#include "OneStepMoves.h"

namespace ModernChess::Attacks {

    namespace King {
        /**
         * @see https://www.chessprogramming.org/King_Pattern
         * @param kingSet
         * @return attack set
         */
        constexpr BitBoardState kingAttacks(BitBoardState kingSet)
        {
            BitBoardState attacks = MoveGenerations::oneStepEast(kingSet) | MoveGenerations::oneStepWest(kingSet);
            kingSet |= attacks;
            attacks |= MoveGenerations::oneStepNorth(kingSet) | MoveGenerations::oneStepSouth(kingSet);
            return attacks;
        }
    }

    /**
     * @brief Pass the square as index for retrieving the attack map
     * @return Attack map for every square
     */
    constexpr std::array<BitBoardState, 64> generateKingAttacks()
    {
        // multiple attacks table for every square
        std::array<BitBoardState, 64> attackTable{};

        // loop over 64 board squares
        for (Square square = Square::h8; square >= Square::a1; --square)
        {
            const BitBoardState state = BitBoardOperations::occupySquare(BoardState::empty, square);
            attackTable[square] = King::kingAttacks(state);
        }

        return attackTable;
    }
}