#pragma once

#include "SquareModel.h"

#include <QQmlListProperty>
#include <QVector>

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

    void generateBoard();
    [[nodiscard]] QString getSquare(int row, int column) const;
};
