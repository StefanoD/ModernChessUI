#pragma once

#include "BishopAttacks.h"
#include "RookAttacks.h"

namespace ModernChess {

    class QueenAttacks
    {
    public:
        explicit QueenAttacks(const BishopAttacks &bishopAttacks,
                              const RookAttacks &rookAttacks) :
                m_bishopAttacks(bishopAttacks),
                m_rookAttacks(rookAttacks) {}

        [[nodiscard]] inline BitBoardState getAttacks(Square square, BitBoardState occupiedSquares) const
        {
            return m_bishopAttacks.getAttacks(square, occupiedSquares) | m_rookAttacks.getAttacks(square, occupiedSquares);
        }

    private:
        const BishopAttacks &m_bishopAttacks;
        const RookAttacks &m_rookAttacks;
    };
}