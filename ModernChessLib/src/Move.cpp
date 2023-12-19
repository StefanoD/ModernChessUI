#include "ModernChess/Move.h"

#include <array>

namespace ModernChess {
    /**
     * @brief UCI compliant promotion encoding
     */
    static constexpr std::array<char, 255> promotedPieceToCharacter = [] {
        std::array<char, 255> a{};
        a[Figure::WhiteKnight] = 'n';
        a[Figure::WhiteBishop] = 'b';
        a[Figure::WhiteRook] = 'r';
        a[Figure::WhiteQueen] = 'q';
        a[Figure::BlackKnight] = 'n';
        a[Figure::BlackBishop] = 'b';
        a[Figure::BlackRook] = 'r';
        a[Figure::BlackQueen] = 'q';
        return a;
    } ();
}

std::ostream& operator<<(std::ostream& os, ModernChess::Move move)
{
    using namespace ModernChess;

    if (move.isNullMove())
    {
        // UCI compliant NULL move
        os << "0000";
        return os;
    }

    os << squareToCoordinates[move.getFrom()];
    os << squareToCoordinates[move.getTo()];

    const char promotedPiece = promotedPieceToCharacter[move.getPromotedPiece()];

    if ('\0' != promotedPiece)
    {
        os << promotedPiece;
    }

    return os;
}

