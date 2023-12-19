#include "ModernChess/BishopAttacks.h"

using namespace ModernChess::BishopAttackHelperFunctions;

namespace ModernChess {

    BishopAttacks::BishopAttacks()
    {
        // loop over all squares
        for (Square square = Square::a1; square <= Square::h8; ++square)
        {
            // init bishop attack masks
            attackMasks[square] = maskBishopAttacks(square);

            // init current mask
            const uint64_t attackMask = attackMasks[square];

            // init relevant occupancy bit count
            const uint32_t relevantBitsCount = BishopMetaData::relevantBits[square];

            // init occupancy indices
            const uint32_t occupancyIndices = (1 << relevantBitsCount);

            // loop over occupancy indices
            for (uint32_t index = 0; index < occupancyIndices; ++index)
            {
                // init current occupancy variation
                const uint64_t occupancy = BitBoardOperations::setOccupancy(index, relevantBitsCount, attackMask);

                // init magic index
                const uint32_t magicIndex = (occupancy * BishopMetaData::magicNumbers[square]) >> (64 - relevantBitsCount);

                // init figure attacks
                bishopAttacks[square][magicIndex] = bishopAttacksOnTheFly(occupancy, square);
            }
        }
    }
}