#pragma once

#include "PawnPushes.h"
#include "Color.h"

namespace ModernChess {
    namespace Attacks {

        namespace WhitePawnsAttacks {
            // See https://www.chessprogramming.org/Pawn_Attacks_(Bitboards)

            constexpr BitBoardState east(BitBoardState whitePawns)
            {
                return MoveGenerations::oneStepNorthEast(whitePawns);
            }

            constexpr BitBoardState west(BitBoardState whitePawns)
            {
                return MoveGenerations::oneStepNorthWest(whitePawns);
            }

            constexpr BitBoardState any(BitBoardState whitePawns)
            {
                return east(whitePawns) | west(whitePawns);
            }

            constexpr BitBoardState two(BitBoardState whitePawns)
            {
                return east(whitePawns) & west(whitePawns);
            }

            constexpr BitBoardState single(BitBoardState whitePawns)
            {
                return east(whitePawns) ^ west(whitePawns);
            }
        }

        namespace BlackPawnsAttacks {
            // See https://www.chessprogramming.org/Pawn_Attacks_(Bitboards)

            constexpr BitBoardState east(BitBoardState blackPawns)
            {
                return MoveGenerations::oneStepSouthEast(blackPawns);
            }

            constexpr BitBoardState west(BitBoardState blackPawns)
            {
                return MoveGenerations::oneStepSouthWest(blackPawns);
            }

            constexpr BitBoardState any(BitBoardState blackPawns)
            {
                return east(blackPawns) | west(blackPawns);
            }

            constexpr BitBoardState two(BitBoardState blackPawns)
            {
                return east(blackPawns) & west(blackPawns);
            }

            constexpr BitBoardState single(BitBoardState blackPawns)
            {
                return east(blackPawns) ^ west(blackPawns);
            }
        }

        /**
         * @brief Pass the color and square as indices for retrieving the attack map
         * @return Attack map for every color and square
         */
        constexpr std::array<std::array<BitBoardState, 64>, 2> generatePawnAttacks()
        {
            // pawn attacks table [color][square]
            std::array<std::array<BitBoardState, 64>, 2> pawnAttacks{};

            // loop over 64 board squares
            for (Square square = Square::h8; square >= Square::a1; --square)
            {
                const BitBoardState state = BitBoardOperations::occupySquare(BoardState::empty, square);
                pawnAttacks[White][square] = WhitePawnsAttacks::any(state);
                pawnAttacks[Black][square] = BlackPawnsAttacks::any(state);
            }

            return pawnAttacks;
        }

    }
}