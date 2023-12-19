#pragma once

#include "BitBoardOperations.h"
#include "Square.h"

#include <string_view>
#include <string>
#include <sstream>

namespace ModernChess
{
    class BasicParser
    {
    public:
        explicit BasicParser(std::string_view fen);

        [[nodiscard]] Square parseSquare();

        [[nodiscard]] static bool isAlphabetic(char character)
        {
            return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
        }

        [[nodiscard]] static bool isNumerical(char character)
        {
            return character >= '0' && character <= '9';
        }

        [[nodiscard]] std::string getCurrentPosition() const;

        [[nodiscard]] std::string_view completeStringView() const;

        [[nodiscard]] std::string_view currentStringView() const;

        [[nodiscard]] bool isRankNumber(char character) const;

        void nextPosition();

        [[nodiscard]] char getNextCharacter();

        [[nodiscard]] char currentCharacter();

        [[nodiscard]] bool hasNextCharacter() const;

        [[nodiscard]] bool isAtEndOfString() const;

        [[nodiscard]] std::string_view getNextString();

        template<typename Number>
        [[nodiscard]] Number parseNumber()
        {
            std::stringstream strNumber = extractNumber();

            double number = 0;
            strNumber >> number;

            if (strNumber.fail())
            {
                throw std::range_error("Could not parse number \"" + strNumber.str() +
                                       "\" at position " + getCurrentPosition() + "!");
            }

            nextPosition();

            return static_cast<Number>(number);
        }

        void skipWhiteSpaces();

    protected:
        std::string_view::iterator m_beginPos{};
        std::string_view::iterator m_currentPos{};
        std::string_view::iterator m_endPos{};

        [[nodiscard]] std::stringstream extractNumber();
    };
}