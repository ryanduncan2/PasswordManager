#include "PasswordManager/Data/CharacterMap.h"

#include <cmath>

namespace PM
{
    CharacterMap::CharacterMap(const bool allowCapitals, const bool allowNumerals, const bool allowSpecialChars, const SecureString& allowedChars, const SecureString& disallowedChars)
      : m_Map()
    {
        // Lowercase Letters

        for (std::uint8_t i = ASCII_ALPH_LOWER_START; i <= ASCII_ALPH_LOWER_END; ++i)
        {
            if (disallowedChars.contains(i) && !allowedChars.contains(i))
            {
                continue;
            }

            m_Map += (char)i;
        }

        // Uppercase Letters
        
        for (std::uint8_t i = ASCII_ALPH_UPPER_START; i <= ASCII_ALPH_UPPER_END; ++i)
        {
            if ((!allowCapitals && !allowedChars.contains(i)) || disallowedChars.contains(i))
            {
                continue;
            }

            m_Map += (char)i;
        }

        // Numerals
        
        for (std::uint8_t i = ASCII_NUMERALS_START; i <= ASCII_NUMERALS_END; ++i)
        {
            if ((!allowNumerals && !allowedChars.contains(i)) || disallowedChars.contains(i))
            {
                continue;
            }

            m_Map += (char)i;
        }

        // Special Characters Group 1
        
        for (std::uint8_t i = ASCII_SPECIAL_CHARS_1_START; i <= ASCII_SPECIAL_CHARS_1_END; ++i)
        {
            if ((!allowSpecialChars && !allowedChars.contains(i)) || disallowedChars.contains(i))
            {
                continue;
            }

            m_Map += (char)i;
        }

        // Special Characters Group 2
        
        for (std::uint8_t i = ASCII_SPECIAL_CHARS_2_START; i <= ASCII_SPECIAL_CHARS_2_END; ++i)
        {
            if ((!allowSpecialChars && !allowedChars.contains(i)) || disallowedChars.contains(i))
            {
                continue;
            }

            m_Map += (char)i;
        }

        // Special Characters Group 3
        
        for (std::uint8_t i = ASCII_SPECIAL_CHARS_3_START; i <= ASCII_SPECIAL_CHARS_3_END; ++i)
        {
            if ((!allowSpecialChars && !allowedChars.contains(i)) || disallowedChars.contains(i))
            {
                continue;
            }

            m_Map += (char)i;
        }

        // Special Characters Group 4
        
        for (std::uint8_t i = ASCII_SPECIAL_CHARS_4_START; i <= ASCII_SPECIAL_CHARS_4_END; ++i)
        {
            if ((!allowSpecialChars && !allowedChars.contains(i)) || disallowedChars.contains(i))
            {
                continue;
            }

            m_Map += (char)i;
        }
    }

    std::uint8_t CharacterMap::Map(const std::uint8_t value) const noexcept
    {
        return (std::uint8_t)m_Map[std::floor(((float)value / 256.0f) * m_Map.length())];
    }
}