#include <cassert>
#include "SlcFileConvert.h"

bool SlcFileConvert::Convert()
{

    std::ofstream fs_out(file_name_out);
    if (!fs_out.is_open())
    {
        fs_out.clear();
        fs_out.open(file_name_out, std::ios::out); // create file
    }

    std::ifstream fs_in(file_name_in);

    // success ?
    if (!fs_in.is_open())
    {
        std::wcerr << L"Input file doesn't exist!" << std::endl;
        return false;
    }
    assert(fs_in);

    std::wcout << std::boolalpha;
    std::wcout << L"fs_out.is_open() = " << fs_out.is_open() << '\n';
    std::wcout << L"fs_out.good() = " << fs_out.good() << '\n';
    std::wcout << std::boolalpha;
    std::wcout << L"fs_in.is_open() = " << fs_in.is_open() << '\n';
    std::wcout << L"fs_in.good() = " << fs_in.good() << '\n';

    // fs_in empty ?
    if (fs_in.eof()) {
        fs_in.close();
        fs_out.close();
        return false;
    }

    // read file line by line
    auto code_page{ Bentley::LangCodePage::LatinI };
    for (std::string line_in; std::getline(fs_in, line_in); )
    {
        //! Converts a locale-encoded string to a WChar string.
        //! @note Up to count characters will be converted; less may be converted if a NULL is encountered earlier. If count is BeStringUtilities::AsManyAsPossible, the input is assumed to be NULL-terminated.
        //! @note The result is always cleared first, even if the input is NULL or empty. Further, if an error is encountered, the result will not contain partial results.
        //! @note The result will always be NULL-terminated, even if the input (limited by count) is not.

        WString line_out{};

        BeStringUtilities::LocaleCharToWChar
        (
            /* WString */  line_out
            /* CharCP  */, line_in.c_str()
            /* UInt32  */, static_cast<UInt32>(code_page)
            /* size_t count = AsManyAsPossible */
        );
        fs_out << line_out.c_str();
    }
    // done
    fs_out.close();
    fs_in.close();

    return true;
}
