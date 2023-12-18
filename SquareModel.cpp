#include "SquareModel.h"

#include <utility>

SquareModel::SquareModel(QString square, QString imageSource, QObject *parent)
    : QObject{parent},
    m_square(std::move(square)),
    m_imageSource(std::move(imageSource))
{}

void SquareModel::setSquare(QString square)
{
    m_square = std::move(square);
}

void SquareModel::setImageSource(QString imageSource)
{
    m_imageSource = std::move(imageSource);
}
