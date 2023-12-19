#pragma once

#include "Move.h"

#include <cinttypes>
#include <memory>
#include <functional>
#include <limits>

namespace ModernChess {

    ///< @see https://web.archive.org/web/20071031100051/http://www.brucemo.com/compchess/programming/hashing.htm

    enum class HashFlag : uint8_t {
        Exact,
        Alpha,
        Beta
    };

    class TranspositionTable {
    public:
        TranspositionTable();

        void addEntry(uint64_t hash, HashFlag flag, int32_t score, uint8_t depth);
        [[nodiscard]] int32_t getScore(uint64_t hash, int32_t alpha, int32_t beta, uint8_t depth) const;
        void clear();
        void resize(size_t mbSize);

        // This value has been chosen, because Evaluation::Infinity is defined as std::numeric_limits<int32_t>::max() / 2
        static constexpr int32_t NoHashEntryFound = std::numeric_limits<int32_t>::max();
    private:
        struct TTEntry {
            TTEntry(uint64_t hash, HashFlag hashFlag, int32_t score, uint8_t depth) :
                    hash(hash),
                    score(score),
                    depth(depth),
                    hashFlag(hashFlag)
            {}
            uint64_t hash{};
            int32_t score{};
            uint8_t depth{}; //< current search depth
            HashFlag hashFlag{};
        };

        std::unique_ptr<TTEntry[], std::function<void(TTEntry*)>> m_table;
        size_t m_numberEntries{};
    };
}