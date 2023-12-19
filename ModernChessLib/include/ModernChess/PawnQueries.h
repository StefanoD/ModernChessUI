#pragma once

#include "PawnAttacks.h"

namespace ModernChess {
    namespace WhitePawnsQueries {
        // See https://www.chessprogramming.org/Pawn_Attacks_(Bitboards)

        constexpr bool ableToCaptureEast(BitBoardState whitePawns, BitBoardState blackFigures)
        {
            return (whitePawns & Attacks::BlackPawnsAttacks::west(blackFigures)) != 0;
        }

        constexpr bool ableToCaptureWest(BitBoardState whitePawns, BitBoardState blackFigures)
        {
            return (whitePawns & Attacks::BlackPawnsAttacks::east(blackFigures)) != 0;
        }

        constexpr bool ableToCaptureAny(BitBoardState whitePawns, BitBoardState blackFigures)
        {
            return (whitePawns & Attacks::BlackPawnsAttacks::any(blackFigures)) != 0;
        }

        /**
         * @brief A kind if of set-wise static exchange evaluation - only considering pawnAttackTable-attacks from both sides.
         *        A square is assumed safe, if the number of own pawnAttackTable defends if greater or equal than opponent
         *        pawnAttackTable attacks. That is true if the own side defends a square twice, or the opposite side has no
         *        attacks at all, or own side attacks once and opponent not twice.
         * @param whitePawns
         * @param blackPawns
         * @return true if White pawns are on safe square, false otherwise
         */
        constexpr bool onSafePawnSquares(BitBoardState whitePawns, BitBoardState blackPawns)
        {
            const BitBoardState whitePawnsEastAttacks = Attacks::WhitePawnsAttacks::east(whitePawns);
            const BitBoardState whitePawnsWestAttacks = Attacks::WhitePawnsAttacks::west(whitePawns);
            const BitBoardState blackPawnsEastAttacks = Attacks::BlackPawnsAttacks::east(blackPawns);
            const BitBoardState blackPawnsWestAttacks = Attacks::BlackPawnsAttacks::west(blackPawns);
            const BitBoardState whitePawnsTwiceAttacks = whitePawnsEastAttacks & whitePawnsWestAttacks;
            const BitBoardState whitePawnsSingleAttacks = whitePawnsEastAttacks ^ whitePawnsWestAttacks;
            const BitBoardState blackPawnsTwiceAttacks = blackPawnsEastAttacks & blackPawnsWestAttacks;
            const BitBoardState blackPawnsAnyAttacks = blackPawnsEastAttacks | blackPawnsWestAttacks;
            return (whitePawnsTwiceAttacks | ~blackPawnsAnyAttacks |
                    (whitePawnsSingleAttacks & ~blackPawnsTwiceAttacks)) != 0;
        }
    }

    namespace BlackPawnsQueries {
        // See https://www.chessprogramming.org/Pawn_Attacks_(Bitboards)

        constexpr bool ableToCaptureEast(BitBoardState blackPawns, BitBoardState whiteFigures)
        {
            return (blackPawns & Attacks::WhitePawnsAttacks::west(whiteFigures)) != 0;
        }

        constexpr bool ableToCaptureWest(BitBoardState blackPawns, BitBoardState whiteFigures)
        {
            return (blackPawns & Attacks::WhitePawnsAttacks::east(whiteFigures)) != 0;
        }

        constexpr bool ableToCaptureAny(BitBoardState blackPawns, BitBoardState whiteFigures)
        {
            return (blackPawns & Attacks::WhitePawnsAttacks::any(whiteFigures)) != 0;
        }

        /**
         * @brief A kind if of set-wise static exchange evaluation - only considering pawnAttackTable-attacks from both sides.
         *        A square is assumed safe, if the number of own pawnAttackTable defends if greater or equal than opponent
         *        pawnAttackTable attacks. That is true if the own side defends a square twice, or the opposite side has no
         *        attacks at all, or own side attacks once and opponent not twice.
         * @param blackPawns
         * @param whitePawns
         * @return true if Black pawns are on safe square, false otherwise
         */
        constexpr bool onSafePawnSquares(BitBoardState blackPawns, BitBoardState whitePawns)
        {
            // TODO Should maybe be tested
            const BitBoardState whitePawnsEastAttacks = Attacks::WhitePawnsAttacks::east(whitePawns);
            const BitBoardState whitePawnsWestAttacks = Attacks::WhitePawnsAttacks::west(whitePawns);
            const BitBoardState whitePawnsAnyAttacks = whitePawnsEastAttacks | whitePawnsWestAttacks;
            const BitBoardState whitePawnsTwiceAttacks = whitePawnsEastAttacks & whitePawnsWestAttacks;

            const BitBoardState blackPawnsEastAttacks = Attacks::BlackPawnsAttacks::east(blackPawns);
            const BitBoardState blackPawnsWestAttacks = Attacks::BlackPawnsAttacks::west(blackPawns);
            const BitBoardState blackPawnsSingleAttacks = blackPawnsEastAttacks ^ blackPawnsWestAttacks;
            const BitBoardState blackPawnsTwiceAttacks = blackPawnsEastAttacks & blackPawnsWestAttacks;
            return (blackPawnsTwiceAttacks | ~whitePawnsAnyAttacks |
                    (blackPawnsSingleAttacks & ~whitePawnsTwiceAttacks)) != 0;
        }
    }
}