#include "ModernChess/CastlingRights.h"

std::ostream& operator<<(std::ostream& os, const ModernChess::CastlingRights castleRights)
{
    using namespace ModernChess;

    os << "Castling Rights:" << std::endl;

    os << "\tWhite:";

    if (whiteCanCastleKingSide(castleRights))
    {
        os << " king-side";
    }
    if (whiteCanCastleQueenSide(castleRights))
    {
        os << " queen-side";
    }

    if (not whiteCanCastleKingSide(castleRights) and
        not whiteCanCastleQueenSide(castleRights))
    {
        os << " castling not possible";
    }

    os << std::endl;
    os << "\tBlack:";

    if (blackCanCastleKingSide(castleRights))
    {
        os << " king-side";
    }
    if (blackCanCastleQueenSide(castleRights))
    {
        os << " queen-side";
    }

    if (not blackCanCastleKingSide(castleRights) and
        not blackCanCastleQueenSide(castleRights))
    {
        os << " castling not possible";
    }

    return os;
}