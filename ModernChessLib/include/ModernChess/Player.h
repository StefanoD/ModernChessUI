#pragma once

#include "Board.h"
#include "Color.h"

namespace ModernChess
{
    class Player
    {
    public:

        Board board{};
        bool onTurn{};
        Color color = Color::UndefinedColor;
    };
}