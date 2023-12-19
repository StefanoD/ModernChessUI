#pragma once

#include "BitBoardOperations.h"
#include "OneStepMoves.h"

namespace ModernChess::MoveGenerations
{
    namespace WhitePawns
    {
        // See https://www.chessprogramming.org/Pawn_Pushes_(Bitboards)#GeneralizedPush

        /**
         * @param whitePawns board with White pawns
         * @param emptySquares An inverted play board where empty square bits are set to 1.
         * @return Resulting board where all White pawns have been pushed by one square
         */
        constexpr BitBoardState singlePush(BitBoardState whitePawns, BitBoardState emptySquares)
        {
            return oneStepNorth(whitePawns) & emptySquares;
        }

        /**
         * @param whitePawns board with White pawns
         * @param emptySquares An inverted play board where empty square bits are set to 1.
         * @return Resulting board where all White pawns have been pushed by two square
         */
        constexpr BitBoardState doublePush(BitBoardState whitePawns, BitBoardState emptySquares)
        {
            const BitBoardState singlePushs = singlePush(whitePawns, emptySquares);
            return oneStepNorth(singlePushs) & emptySquares & BitBoardConstants::rank4;
        }

        constexpr bool singlePushPossible(BitBoardState whitePawns, BitBoardState emptySquares)
        {
            // To get the set of source squares of pawns able to push is about intersection of pawns with the shifted
            // empty squares in opposite direction:
            return (oneStepSouth(emptySquares) & whitePawns) != 0;
        }

        constexpr bool doublePushPossible(BitBoardState whitePawns, BitBoardState emptySquares)
        {
            const BitBoardState emptyRank3 = oneStepSouth(emptySquares & BitBoardConstants::rank4) & emptySquares;
            return singlePushPossible(whitePawns, emptyRank3);
        }
    }

    namespace BlackPawns
    {
        // See https://www.chessprogramming.org/Pawn_Pushes_(Bitboards)#GeneralizedPush

        /**
         * @param blackPawns board with Black pawns
         * @param emptySquares An inverted play board where empty square bits are set to 1.
         * @return Resulting board where all Black pawns have been pushed by one square
         */
        constexpr BitBoardState singlePush(BitBoardState blackPawns, BitBoardState emptySquares)
        {
            return oneStepSouth(blackPawns) & emptySquares;
        }

        /**
         * @param blackPawns board with Black pawns
         * @param emptySquares An inverted play board where empty square bits are set to 1.
         * @return Resulting board where all Black pawns have been pushed by two square
         */
        constexpr BitBoardState doublePush(BitBoardState blackPawns, BitBoardState emptySquares)
        {
            const BitBoardState singlePushs = singlePush(blackPawns, emptySquares);
            return oneStepSouth(singlePushs) & emptySquares & BitBoardConstants::rank5;
        }

        constexpr bool singlePushPossible(BitBoardState blackPawns, BitBoardState emptySquares)
        {
            // To get the set of source squares of pawns able to push is about intersection of pawns with the shifted
            // empty squares in opposite direction:
            return (oneStepNorth(emptySquares) & blackPawns) != 0;
        }

        constexpr bool doublePushPossible(BitBoardState blackPawns, BitBoardState emptySquares)
        {
            const BitBoardState emptyRank6 = oneStepNorth(emptySquares & BitBoardConstants::rank5) & emptySquares;
            return singlePushPossible(blackPawns, emptyRank6);
        }
    }
}