#pragma once

#include "GlobalConstants.h"
#include "Move.h"

#include <array>

namespace ModernChess
{
    /**
     * @see https://www.chessprogramming.org/Triangular_PV-Table
     */
    struct PrincipalVariationTable {
        /*
         * ================================
         *       Triangular PV table
         * --------------------------------
         *   PV line: e2e4 e7e5 g1f3 b8c6
         * ================================
         *      0    1    2    3    4    5
         *
         * 0    m1   m2   m3   m4   m5   m6
         *
         * 1    0    m2   m3   m4   m5   m6
         *
         * 2    0    0    m3   m4   m5   m6
         *
         * 3    0    0    0    m4   m5   m6
         *
         * 4    0    0    0    0    m5   m6
         *
         * 5    0    0    0    0    0    m6
         */

        explicit PrincipalVariationTable(int32_t rootHalfMoveClock) :
                rootHalfMoveClock(rootHalfMoveClock) {}

        std::array<std::array<Move, MaxHalfMoves>, MaxHalfMoves> pvTable{};
        std::array<int32_t, MaxHalfMoves> pvLength{};

        void addPrincipalVariation(const Move move, int32_t ply)
        {
            pvTable[ply][ply] = move;

            // loop over the next ply
            for (int nextPly = ply + 1; nextPly < pvLength[ply + 1]; ++nextPly)
            {
                // copy move from deeper ply into a current ply's line
                pvTable[ply][nextPly] = pvTable[ply + 1][nextPly];
            }

            // adjust PV length
            pvLength[ply] = pvLength[ply + 1];
        }

        using ConstIterator = const Move*;

        [[nodiscard]] ConstIterator begin() const
        {
            return &pvTable[rootHalfMoveClock][rootHalfMoveClock];
        }

        [[nodiscard]] ConstIterator end() const
        {
            const size_t end = pvLength[rootHalfMoveClock];
            return &pvTable[rootHalfMoveClock][end];
        }

        [[nodiscard]] size_t size() const
        {
            return pvLength[rootHalfMoveClock];
        }

    private:
        int32_t rootHalfMoveClock{};
    };
}