#pragma once

#include "SquareModel.h"
#include "ModernChess/GameState.h"

#include <QQmlListProperty>
#include <QVector>
#include <QMap>

class Board : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SquareModel> squares READ squares NOTIFY squaresChanged FINAL)
    QML_ELEMENT
public:
    explicit Board(QObject *parent = nullptr);

    QQmlListProperty<SquareModel> squares();

public slots:
    void onRotate();

signals:
    void squaresChanged();

private:
    std::unique_ptr<QObject> m_boardParent{};
    QVector<SquareModel*> m_squares;
    bool m_rotated{};
    ModernChess::GameState m_gameState;

    void generateBoard();
    [[nodiscard]] QString getSquare(int row, int column) const;

    QMap<ModernChess::Figure, QString> m_figureToResource {
        {ModernChess::Figure::WhiteKing, "/resources/white-king.svg"},
        {ModernChess::Figure::WhiteQueen, "/resources/white-queen.svg"},
        {ModernChess::Figure::WhiteBishop, "/resources/white-bishop.svg"},
        {ModernChess::Figure::WhiteKnight, "/resources/white-knight.svg"},
        {ModernChess::Figure::WhiteRook, "/resources/white-rook.svg"},
        {ModernChess::Figure::WhitePawn, "/resources/white-pawn.svg"},
        {ModernChess::Figure::BlackKing, "/resources/black-king.svg"},
        {ModernChess::Figure::BlackQueen, "/resources/black-queen.svg"},
        {ModernChess::Figure::BlackBishop, "/resources/black-bishop.svg"},
        {ModernChess::Figure::BlackKnight, "/resources/black-knight.svg"},
        {ModernChess::Figure::BlackRook, "/resources/black-rook.svg"},
        {ModernChess::Figure::BlackPawn, "/resources/black-pawn.svg"},
    };
};
