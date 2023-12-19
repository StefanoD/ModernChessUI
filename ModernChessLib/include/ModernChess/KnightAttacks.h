 #pragma once

#include "OneStepMoves.h"

namespace ModernChess::Attacks {

        namespace Knights
        {
            // See https://www.chessprogramming.org/Knight_Pattern#MultipleKnightAttacks
            constexpr BitBoardState attacks(BitBoardState knights)
            {
                const BitBoardState l1 = (knights >> 1) & BitBoardConstants::notHFile;
                const BitBoardState l2 = (knights >> 2) & BitBoardConstants::notGHFile;
                const BitBoardState r1 = (knights << 1) & BitBoardConstants::notAFile;
                const BitBoardState r2 = (knights << 2) & BitBoardConstants::notABFile;
                const BitBoardState h1 = l1 | r1;
                const BitBoardState h2 = l2 | r2;
                return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
            }

            // See https://www.chessprogramming.org/Knight_Pattern#Knight_Forks
            /**
             * @brief The intersection of those targets with squares not occupied by own pieces or attacked by opponent
             *        pawns and knights, but attacked by own knight(s) leaves a move target set with some forced properties.
             * @param targets
             * @return
             */
            constexpr BitBoardState forkTargetSquare(BitBoardState targets)
            {
                BitBoardState west = MoveGenerations::oneStepWest(targets);
                BitBoardState east = MoveGenerations::oneStepEast(targets);
                BitBoardState attack = east << 16;
                BitBoardState forks = (west << 16) & attack;
                attack |=  west << 16;
                forks |= (east >> 16) & attack;
                attack |=  east >> 16;
                forks |= (west >> 16) & attack;
                attack |=  west >> 16;
                east   = MoveGenerations::oneStepEast(east);
                west   = MoveGenerations::oneStepWest(west);
                forks |= (east <<  8) & attack;
                attack |=  east << 8;
                forks |= (west <<  8) & attack;
                attack |=  west << 8;
                forks |= (east >>  8) & attack;
                attack |=  east >> 8;
                forks |= (west >>  8) & attack;
                return forks;
            }
        }

        /**
         * @brief Pass the square as index for retrieving the attack map
         * @return Attack map for every square
         */
        constexpr std::array<BitBoardState, 64> generateKnightAttacks()
        {
            // multiple attacks table for every square
            std::array<BitBoardState, 64> attackTable{};

            // loop over 64 board squares
            for (Square square = Square::h8; square >= Square::a1; --square)
            {
                const BitBoardState state = BitBoardOperations::occupySquare(BoardState::empty, square);
                attackTable[square] = Knights::attacks(state);
            }

            return attackTable;
        }

    }