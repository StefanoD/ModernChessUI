#include "Board.h"
#include "ModernChess/FenParsing.h"

constexpr int numberSquares = 64;

Board::Board(QObject *parent)
    : QObject{parent},
    m_gameState(ModernChess::FenParsing::FenParser(ModernChess::FenParsing::startPosition).parse())
{
    m_squares.reserve(numberSquares);
    generateBoard();
}

QQmlListProperty<SquareModel> Board::squares()
{
    return QQmlListProperty<SquareModel>{this, &m_squares };
}

void Board::onRotate()
{
    m_rotated = not m_rotated;
    generateBoard();
    emit squaresChanged();
}

void Board::generateBoard()
{
    // clear old board by deleting old parent
    m_boardParent = std::make_unique<QObject>();
    m_squares.clear();

    // generate new board
    for (int row = 7; row >= 0; --row)
    {
        for (int col = 0; col < 8; ++col)
        {
            const int rank = m_rotated ? (7-row) : row;
            const int file = m_rotated ? (7-col) : col;
            const ModernChess::Square square = ModernChess::BitBoardOperations::getSquare(rank, file);

            ModernChess::Figure figureOnSquare = ModernChess::Figure::None;

            // loop over all figures bitboards
            for (ModernChess::Figure figure = ModernChess::WhitePawn; figure <= ModernChess::BlackKing; ++figure)
            {
                if (ModernChess::BitBoardOperations::isOccupied(m_gameState.board.bitboards[figure], square))
                {
                    figureOnSquare = figure;
                    break;
                }
            }

            m_squares.append(new SquareModel(getSquare(row, col), m_figureToResource[figureOnSquare], m_boardParent.get()));
        }
    }
}

QString Board::getSquare(int row, int column) const
{
    if (m_rotated)
    {
        return QString("%1%2").arg(QChar('h'- column), QString::number(8 - row));
    }

    return QString("%1%2").arg(QChar('a'+ column), QString::number(row + 1));
}
