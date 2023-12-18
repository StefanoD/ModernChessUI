#include "Board.h"

constexpr int numberSquares = 64;

Board::Board(QObject *parent)
    : QObject{parent}
{
    m_squares.reserve(numberSquares);
    generateBoard();
}

QQmlListProperty<SquareModel> Board::squares()
{
    return QQmlListProperty(this, &m_squares);
}

void Board::generateBoard()
{
    // clear old board by deleting old parent
    delete m_boardParent;
    m_boardParent = new QObject(this);
    m_squares.clear();

    // generate new board
    for (int col = 0; col < 8; ++col)
    {
        for (int row = 0; row < 8; ++row)
        {
            m_squares.append(new SquareModel(getSquare(row, col), "/resources/white-king.svg", m_boardParent));
        }
    }
}

QString Board::getSquare(int row, int column) const
{
    if (m_flipped)
    {
        return QString("%1%2").arg(QChar('h'- column), QString::number(8 - row));
    }

    return QString("%1%2").arg(QChar('a'+ column), QString::number(row + 1));
}
