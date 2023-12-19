#pragma once

#include "BitBoardConstants.h"
#include "Square.h"

#include <array>
#include <cassert>

namespace ModernChess::BitBoardOperations
{

    [[nodiscard]] constexpr BitBoardState occupySquare(BitBoardState board, Square square)
    {
        const BitBoardState state = uint64_t(1) << square;
        board |= state;

        return board;
    }

    [[nodiscard]] constexpr BitBoardState eraseSquare(BitBoardState board, Square square)
    {
        const BitBoardState state = ~(uint64_t(1) << square);
        board &= state;

        return board;
    }

    [[nodiscard]] constexpr bool isOccupied(BitBoardState board, Square square)
    {
        const BitBoardState state = uint64_t(1) << square;
        board &= state;

        return board == state;
    }

    /**
     * @brief Deducts square from rank and file
     * @param rank within the range of [0 - 7]
     * @param file within the range of [0 - 7]
     * @see https://www.chessprogramming.org/Square_Mapping_Considerations#Deduction_on_Files_and_Ranks
     */
    [[nodiscard]] constexpr Square getSquare(int rank, int file)
    {
        return Square{rank * 8 + file};
    }

    /**
     * @brief Mirror a bitboard horizontally about the center files.
     *        File a is mapped to file h and vice versa.
     * @param board any bitboard
     * @see https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating
     * @return bitboard board mirrored horizontally
     */
    [[nodiscard]] constexpr BitBoardState mirrorHorizontal (BitBoardState board)
    {
        constexpr BitBoardState k1 = 0x5555555555555555;
        constexpr BitBoardState k2 = 0x3333333333333333;
        constexpr BitBoardState k4 = 0x0f0f0f0f0f0f0f0f;
        board = ((board >> 1) & k1) + 2 * (board & k1);
        board = ((board >> 2) & k2) + 4 * (board & k2);
        board = ((board >> 4) & k4) + 16 * (board & k4);
        return board;
    }

    /**
     * @brief Flip a bitboard vertically about the centre ranks.
     *        Rank 1 is mapped to rank 8 and vice versa.
     * @param board any bitboard
     * @see https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating
     * @return bitboard board flipped vertically
     */
    [[nodiscard]] constexpr BitBoardState flipVertical(BitBoardState board)
    {
        constexpr BitBoardState k1 = 0x00FF00FF00FF00FF;
        constexpr BitBoardState k2 = 0x0000FFFF0000FFFF;
        board = ((board >> 8) & k1) | ((board & k1) << 8);
        board = ((board >> 16) & k2) | ((board & k2) << 16);
        board = ( board >> 32) | ( board << 32);
        return board;
    }

    /**
     * Rotate a bitboard by 180 degrees.
     * Square a1 is mapped to h8, and a8 is mapped to h1.
     * @param board any bitboard
     * @see https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating
     * @return bitboard board rotated 180 degrees
     */
    [[nodiscard]] constexpr BitBoardState rotate180(BitBoardState board)
    {
        return mirrorHorizontal(flipVertical(board));
    }

    constexpr std::array<int32_t, 64> bitScanLookup =
    {
            0, 47,  1, 56, 48, 27,  2, 60,
            57, 49, 41, 37, 28, 16,  3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11,  4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30,  9, 24,
            13, 18,  8, 12,  7,  6,  5, 63
    };

    /**
     * bitScanForward
     * @author Kim Walisch (2012)
     * @param bitBoard bitboard to scan
     * @precondition bitBoard != 0
     * @see https://www.chessprogramming.org/BitScan
     * @return index (0..63) of least significant one bit
     */
    [[nodiscard]] constexpr Square bitScanForward(BitBoardState bitBoard)
    {
        constexpr BitBoardState debruijn64 = 0x03f79d71b4cb0a89;
        assert (bitBoard != BoardState::empty);
        const uint64_t index = ((bitBoard ^ (bitBoard - 1)) * debruijn64) >> 58;
        return Square(bitScanLookup[index]);
    }


    /**
     * bitScanReverse
     * @authors Kim Walisch, Mark Dickinson
     * @param bitBoard bitboard to scan
     * @precondition bitBoard != 0
     * @see https://www.chessprogramming.org/BitScan
     * @return index (0..63) of most significant one bit
     */
    [[nodiscard]] constexpr Square bitScanReverse(BitBoardState bitBoard)
    {
        constexpr BitBoardState debruijn64 = 0x03f79d71b4cb0a89;
        assert (bitBoard != BoardState::empty);
        bitBoard |= bitBoard >> 1;
        bitBoard |= bitBoard >> 2;
        bitBoard |= bitBoard >> 4;
        bitBoard |= bitBoard >> 8;
        bitBoard |= bitBoard >> 16;
        bitBoard |= bitBoard >> 32;
        return Square(bitScanLookup[(bitBoard * debruijn64) >> 58]);
    }

    /**
     * @brief Counts the set bits (state is 1) within a bitboard
     * @param bitBoard
     * @see https://www.chessprogramming.org/Population_Count
     * @return Number of bits where state is 1
     */
    [[nodiscard]] constexpr uint32_t countBits(BitBoardState bitBoard)
    {
        uint32_t count = 0;

        while (bitBoard != BoardState::empty)
        {
            ++count;
            bitBoard &= bitBoard - 1; // reset LS1B
        }
        return count;
    }

    /**
     * @brief Used for magic bitboard generation
     * @param index
     * @param numberBitsInMask
     * @param attackMask
     * @return occupancy map
     */
    [[nodiscard]] constexpr BitBoardState setOccupancy(uint32_t index, uint32_t numberBitsInMask, BitBoardState attackMask)
    {
        BitBoardState occupancyMap = BoardState::empty;

        // loop over the range of bits within attack mask
        for (uint32_t count = 0; count < numberBitsInMask; ++count)
        {
            // get LS1B index of attacks mask
            const Square square = bitScanForward(attackMask);

            // erase LS1B in attack map
            attackMask = eraseSquare(attackMask, square);

            // make sure occupancy is on board
            const uint32_t occupancyBit = 1U << count;
            if ((index & occupancyBit) == occupancyBit)
            {    // populate occupancyMap map
                occupancyMap = occupySquare(occupancyMap, square);
            }
        }

        // return occupancyMap map
        return occupancyMap;
    }

    [[nodiscard]] constexpr Square getNorthSquareFromGivenSquare(Square square)
    {
        return Square(square + Square(RayDirections::North));
    }

    [[nodiscard]] constexpr Square getSouthSquareFromGivenSquare(Square square)
    {
        return Square(square + Square(RayDirections::South));
    }
}