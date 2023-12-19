#include "ModernChess/BasicParser.h"

namespace ModernChess
{
    BasicParser::BasicParser(std::string_view fen) :
            m_beginPos(fen.begin()),
            m_currentPos(fen.begin()),
            m_endPos(fen.end())
    {}

    Square BasicParser::parseSquare()
    {
        skipWhiteSpaces();

        char character = *m_currentPos;
        Square square = Square::undefined;

        if (isAlphabetic(character))
        {
            // parse en passant file & rank
            const int file = character - 'a';

            character = getNextCharacter();

            if (!isNumerical(character))
            {
                throw std::range_error("Could not parse square from character '" +
                                       std::string(1, character) + "' at position " + getCurrentPosition() +
                                       "! Expected a number!");
            }

            const int rank = character - '1';

            // init en passant square
            square = BitBoardOperations::getSquare(rank, file);
        }
        else if (character == '-')
        {
            // Do nothing
        }
        else
        {
            throw std::range_error("Could not parse square from character '" +
                                   std::string(1, character) + "' at position " + getCurrentPosition() + "!");
        }

        ++m_currentPos;

        return square;
    }

    std::string BasicParser::getCurrentPosition() const
    {
        return std::to_string(m_currentPos - m_beginPos + 1);
    }

    bool BasicParser::isRankNumber(char character) const
    {
        if (character == '9')
        {
            throw std::range_error("Invalid rank number '9' at position " +
                                   getCurrentPosition() + "!");
        }

        return isNumerical(character);
    }

    void BasicParser::nextPosition()
    {
        ++m_currentPos;
        /*if (m_currentPos == m_endPos)
        {
            const std::string position = getCurrentPosition();
            throw std::range_error("Error at position " + position + ": Unexpected end of line!");
        }*/
    }

    char BasicParser::getNextCharacter()
    {
        ++m_currentPos;
        if (m_currentPos == m_endPos)
        {
            const std::string position = getCurrentPosition();
            throw std::range_error("Error at position " + position + ": Unexpected end of line!");
        }
        return *m_currentPos;
    }

    char BasicParser::currentCharacter()
    {
        if (m_currentPos >= m_endPos)
        {
            const std::string position = getCurrentPosition();
            throw std::range_error("Error at position " + position + ": Unexpected end of line!");
        }
        return *m_currentPos;
    }

    bool BasicParser::hasNextCharacter() const
    {
        return (m_currentPos+1) < m_endPos;
    }

    bool BasicParser::isAtEndOfString() const
    {
        return m_currentPos >= m_endPos;
    }

    std::string_view BasicParser::completeStringView() const
    {
        return {m_beginPos, m_endPos};
    }

    std::string_view BasicParser::currentStringView() const
    {
        if (m_currentPos >= m_endPos)
        {
            return {};
        }

        const size_t length = (m_endPos - m_currentPos);
        return {m_currentPos, length};
    }

    std::string_view BasicParser::getNextString()
    {
        skipWhiteSpaces();

        std::string_view::iterator endPos = m_currentPos;

        while (((endPos+1) < m_endPos) && *(endPos+1) != ' ')
        {
            ++endPos;
        }

        const std::string_view string {m_currentPos, endPos+1};

        // skip either whitespace or set to end position
        m_currentPos = endPos + 1;

        return string;
    }

    std::stringstream BasicParser::extractNumber()
    {
        std::stringstream strNumber;

        skipWhiteSpaces();

        for (char character = currentCharacter();
             isNumerical(character);
             character = getNextCharacter())
        {
            strNumber << character;

            if (!hasNextCharacter())
            {
                break;
            }
        }

        return strNumber;
    }

    void BasicParser::skipWhiteSpaces()
    {
        while (hasNextCharacter() && *m_currentPos == ' ')
        {
            nextPosition();
        }
    }
}