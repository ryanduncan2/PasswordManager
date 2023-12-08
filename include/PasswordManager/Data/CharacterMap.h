#ifndef PM_DATA_CHARACTERMAP_H_
#define PM_DATA_CHARACTERMAP_H_

#include <string>
#include <wx/wx.h>

#include "PasswordManager/Data/SecureString.h"

#define ASCII_ALPH_UPPER_START 65
#define ASCII_ALPH_UPPER_END 90
#define ASCII_ALPH_LOWER_START 97
#define ASCII_ALPH_LOWER_END 122
#define ASCII_NUMERALS_START 48
#define ASCII_NUMERALS_END 57
#define ASCII_SPECIAL_CHARS_1_START 33
#define ASCII_SPECIAL_CHARS_1_END 47
#define ASCII_SPECIAL_CHARS_2_START 58
#define ASCII_SPECIAL_CHARS_2_END 64
#define ASCII_SPECIAL_CHARS_3_START 91
#define ASCII_SPECIAL_CHARS_3_END 96
#define ASCII_SPECIAL_CHARS_4_START 123
#define ASCII_SPECIAL_CHARS_4_END 126

namespace PM
{
    class CharacterMap
    {
    private:
        std::string m_Map;

    public:
        CharacterMap(const bool allowCapitals, const bool allowNumerals, const bool allowSpecialChars, const SecureString& allowedChars, const SecureString& disallowedChars);
        std::uint8_t Map(const std::uint8_t value) const noexcept;

        inline void PrintMap() { wxMessageBox(m_Map); }
    };
}

#endif