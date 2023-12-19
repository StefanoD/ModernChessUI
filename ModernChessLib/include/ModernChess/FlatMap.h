#pragma once

#include <array>
#include <algorithm>
#include <stdexcept>

namespace ModernChess {
    // Code based on Jason Turner's YouTube video "C++ Weekly - Ep 233 - std::map vs constexpr map"
    template<typename Key, typename Value, std::size_t Size>
    struct FlatMap
    {
        constexpr explicit FlatMap(std::array<std::pair<Key, Value>, Size> data) :
                m_data(std::move(data))
        {}

        [[nodiscard]] constexpr Value at(const Key &key) const
        {
            const auto itr = std::find_if(begin(m_data), end(m_data),
                                          [&key](const auto &v)
                                          { return v.first == key; });
            if (itr != end(m_data))
            {
                return itr->second;
            }

            throw std::range_error("Not Found");
        }

    private:
        std::array<std::pair<Key, Value>, Size> m_data;
    };
}