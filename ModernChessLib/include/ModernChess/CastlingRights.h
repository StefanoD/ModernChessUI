#pragma once

#include "Square.h"

#include <ostream>

namespace ModernChess
{
    /**
       binary | decimal

       0001     1  white king can castle to the king side
       0010     2  white king can castle to the queen side
       0100     4  black king can castle to the king side
       1000     8  black king can castle to the queen side
       examples
       1111       both sides can castle both directions
       1001       black king => queen side
                  white king => king side
    */
    enum CastlingRights : uint8_t
    {
        Gone = 0,
        WhiteKingSide = 1,
        WhiteQueenSide = 2,
        WhiteAnySide = WhiteKingSide + WhiteQueenSide,
        BlackKingSide = 4,
        BlackQueenSide = 8,
        BlackAnySide = BlackKingSide + BlackQueenSide
    };

    namespace LookUpTable
    {
        /*
         * For updating castle right in an efficient way when a piece has moved
         *                              castling    move     in      in
         *                              right       update   binary  decimal
         *   king & rooks didn't move:  1111    &   1111  =  1111    15
         *          white king  moved:  1111    &   1100  =  1100    12
         *    white king's rook moved:  1111    &   1110  =  1110    14
         *   white queen's rook moved:  1111    &   1101  =  1101    13
         *
         *           black king moved:  1111    &   0011  =  1011    3
         *    black king's rook moved:  1111    &   1011  =  1011    11
         *   black queen's rook moved:  1111    &   0111  =  0111    7
        */

        // castling rights update constants
        constexpr std::array<uint8_t, 64> castlingRights {
                13, 15, 15, 15, 12, 15, 15, 14,
                15, 15, 15, 15, 15, 15, 15, 15,
                15, 15, 15, 15, 15, 15, 15, 15,
                15, 15, 15, 15, 15, 15, 15, 15,
                15, 15, 15, 15, 15, 15, 15, 15,
                15, 15, 15, 15, 15, 15, 15, 15,
                15, 15, 15, 15, 15, 15, 15, 15,
                 7, 15, 15, 15,  3, 15, 15, 11
        };
    }

    constexpr bool whiteCanCastleKingSide(CastlingRights castleRights)
    {
        return (castleRights & CastlingRights::WhiteKingSide) == CastlingRights::WhiteKingSide;
    }

    constexpr bool whiteCanCastleQueenSide(CastlingRights castleRights)
    {
        return (castleRights & CastlingRights::WhiteQueenSide) == CastlingRights::WhiteQueenSide;
    }

    constexpr bool blackCanCastleKingSide(CastlingRights castleRights)
    {
        return (castleRights & CastlingRights::BlackKingSide) == CastlingRights::BlackKingSide;
    }

    constexpr bool blackCanCastleQueenSide(CastlingRights castleRights)
    {
        return (castleRights & CastlingRights::BlackQueenSide) == CastlingRights::BlackQueenSide;
    }

    constexpr CastlingRights addWhiteKingSideCastlingRights(CastlingRights castleRights)
    {
        return CastlingRights(castleRights | CastlingRights::WhiteKingSide);
    }

    constexpr CastlingRights addWhiteQueenSideCastlingRights(CastlingRights castleRights)
    {
        return CastlingRights(castleRights | CastlingRights::WhiteQueenSide);
    }

    constexpr CastlingRights addBlackKingSideCastlingRights(CastlingRights castleRights)
    {
        return CastlingRights(castleRights | CastlingRights::BlackKingSide);
    }

    constexpr CastlingRights addBlackQueenSideCastlingRights(CastlingRights castleRights)
    {
        return CastlingRights(castleRights | CastlingRights::BlackQueenSide);
    }

    constexpr CastlingRights updateCastlingRights(CastlingRights castleRights, Square movedFrom, Square movedTo)
    {
        return CastlingRights(castleRights &
                              LookUpTable::castlingRights[movedFrom] &
                              LookUpTable::castlingRights[movedTo]);
    }
}

std::ostream& operator<<(std::ostream& os, ModernChess::CastlingRights castleRights);