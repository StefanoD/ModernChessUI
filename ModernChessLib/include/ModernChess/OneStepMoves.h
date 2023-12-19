#pragma once

#include "BitBoardConstants.h"

namespace ModernChess::MoveGenerations
{
    constexpr BitBoardState oneStepNorth(BitBoardState state)
    {
        return state << 8;
    }

    constexpr BitBoardState oneStepSouth(BitBoardState state)
    {
        return state >> 8;
    }

    constexpr BitBoardState oneStepEast(BitBoardState state)
    {
        return (state << 1) & BitBoardConstants::notAFile;
    }

    constexpr BitBoardState oneStepNorthEast(BitBoardState state)
    {
        return (state << 9) & BitBoardConstants::notAFile;
    }

    constexpr BitBoardState oneStepSouthEast(BitBoardState state)
    {
        return (state >> 7) & BitBoardConstants::notAFile;
    }

    constexpr BitBoardState oneStepWest(BitBoardState state)
    {
        return (state >> 1) & BitBoardConstants::notHFile;
    }

    constexpr BitBoardState oneStepSouthWest(BitBoardState state)
    {
        return (state >> 9) & BitBoardConstants::notHFile;
    }

    constexpr BitBoardState oneStepNorthWest(BitBoardState state)
    {
        return (state << 7) & BitBoardConstants::notHFile;
    }
}