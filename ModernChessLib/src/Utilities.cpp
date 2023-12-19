#include "ModernChess/Utilities.h"
#include "ModernChess/AttackQueries.h"
#include "ModernChess/FenParsing.h"

using namespace ModernChess;

std::ostream &print(std::ostream &os, const BitBoardState bitBoardState)
{
    // The enumeration is actually mirrored. So correct this for printing.
    const BitBoardState mirroredBoard = BitBoardOperations::mirrorHorizontal(bitBoardState);

    os << "8 | ";

    for (Square square = Square::h8; square >= Square::a1; --square)
    {
        os << BitBoardOperations::isOccupied(mirroredBoard, square) << " ";

        if (square % 8 == 0)
        {
            os << std::endl;
            const int rank = square / 8;

            if (rank != 0)
            {
                os << rank << " | ";
            }
        }
    }
    os << "-------------------" << std::endl;
    os << "    a b c d e f g h" << std::endl;

    return os;
}

std::ostream& printAttackedSquares(std::ostream& os, const Board &board, Color attacker)
{
    for (int rank = 7; rank >=0; --rank)
    {
        for (int file = 0; file < 8; ++file)
        {
            const Square square = BitBoardOperations::getSquare(rank, file);

            // print ranks
            if (file == 0)
            {
                os << "  " << (rank + 1);
            }

            const bool isAttacked = (Color::White == attacker) ? AttackQueries::squareIsAttackedByWhite(board, square) :
                    AttackQueries::squareIsAttackedByBlack(board, square);

            os << " " << isAttacked;
        }

        // print new line every rank
        os << std::endl;
    }

    os << std::endl << "    a b c d e f g h" << std::endl << std::endl;

    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<ModernChess::Move> &moves)
{
    using namespace ModernChess;

    os << "#\t\tmove\tfigure\tcapture\tdouble-push\ten-passant\tcastling\n\n";

    int32_t moveCounter = 0;

    for (const Move move : moves)
    {
        os << moveCounter << "\t\t";
        os << move << "\t";

#ifdef WIN64
        os << "  " << FenParsing::asciiFigures[move.getMovedFigure()] << "\t\t";
#else
        os << "  " << FenParsing::unicodeFigures[move.getMovedFigure()] << "\t\t";
#endif

        os << (move.isCapture() ? "true" : "false") << "\t";
        os << (move.isDoublePawnPush() ? "true" : "false") << "\t\t";
        os << (move.isEnPassantCapture() ? "true" : "false") << "\t\t";
        os << (move.isCastlingMove() ? "true" : "false") << std::endl;

        ++moveCounter;
    }

    return os;
}