#pragma once

#include <cinttypes>
#include <limits>

namespace ModernChess {
    // Constants from https://www.chessprogramming.org/Square_Mapping_Considerations#LittleEndianRankFileMapping
    // We use Little-Endian Rank-File Mapping

    using BitBoardState = uint64_t;

    namespace BoardState {
        constexpr BitBoardState empty = 0;
        constexpr BitBoardState allSquaresOccupied = std::numeric_limits<BitBoardState>::max();
    }

    enum RayDirections : int32_t
    {
        North = 8,
        NorthEast = 9,
        NorthWest = 7,
        West = -1,
        East = 1,
        SouthWest = -9,
        South = -8,
        SouthEast = -7,
    };

    namespace BitBoardConstants {
        constexpr BitBoardState aFile = 0x0101010101010101;
        constexpr BitBoardState bFile = 0x0202020202020202;
        constexpr BitBoardState notAFile = ~aFile;
        constexpr BitBoardState notABFile = ~(aFile | bFile);
        constexpr BitBoardState gFile = 0x4040404040404040;
        constexpr BitBoardState hFile = 0x8080808080808080;
        constexpr BitBoardState notHFile = ~hFile;
        constexpr BitBoardState notGHFile = ~(gFile | hFile);
        constexpr BitBoardState rank1 = 0x00000000000000FF;
        constexpr BitBoardState rank4 = 0x00000000FF000000;
        constexpr BitBoardState rank5 = 0x000000FF00000000;
        constexpr BitBoardState rank8 = 0xFF00000000000000;
        constexpr BitBoardState a1H8Diagonal = 0x8040201008040201;
        constexpr BitBoardState h1A8Antidiagonal = 0x0102040810204080;
        constexpr BitBoardState lightSquares = 0x55AA55AA55AA55AA;
        constexpr BitBoardState darkSquares = ~lightSquares;
    }
}