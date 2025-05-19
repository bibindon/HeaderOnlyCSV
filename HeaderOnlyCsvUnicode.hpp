#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include "Shlwapi.h"
#include <cwctype>
#pragma comment( lib, "Shlwapi.lib" ) 

class csv
{
public:

    static std::wstring StringToWString(const std::string& str)
    {
        if (str.empty())
        {
            return std::wstring();
        }

        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        if (size_needed <= 0)
        {
            return std::wstring();
        }

        std::wstring wstr(size_needed - 1, 0); 
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
        return wstr;
    }

    static std::string WStringToString(const std::wstring& wstr)
    {
        if (wstr.empty())
        {
            return std::string();
        }

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (size_needed <= 0)
        {
            return std::string();
        }

        std::string str(size_needed - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
        return str;
    }

    // ファイルの中身がUTF-8であると想定。
    // UTF-8はUnicodeのような雰囲気だがマルチバイト文字セットでありUnicodeではない。
    // std::wstringはUTF-8と関係ありそうだがUTF-8ではなくUTF-16。
    static std::vector<std::vector<std::wstring> > Read(const std::wstring& filepath)
    {
        std::vector<std::vector<std::wstring> > csvData;

        int result = PathFileExists(filepath.c_str());
        if (result == FALSE)
        {
            return csvData;
        }

		std::string utf8FilePath = WStringToString(filepath);

        // 「"」記号で囲まれているとセル内改行ができることに注意
        // 「"」記号で囲まれているとセル内で「,」が使用できることに注意
		// テキストファイルがUTF-8である場合、std::wifstreamではなく、std::ifstreamを使用する必要がある。
        std::ifstream ifs(utf8FilePath);
        std::string content = { std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };

        // BOM削除（0xEF 0xBB 0xBF）
        if (content.size() >= 3 &&
            static_cast<unsigned char>(content[0]) == 0xEF &&
            static_cast<unsigned char>(content[1]) == 0xBB &&
            static_cast<unsigned char>(content[2]) == 0xBF)
        {
            content = content.substr(3);
        }

        std::wstring wcontent(StringToWString(content));

        std::wstring buffComma;
        bool doubleQuoteMode = false;
        std::vector<std::wstring> work;

        std::wstring::const_iterator itBegin(wcontent.cbegin());
        std::wstring::const_iterator itEnd(wcontent.cend());;

        for (; itBegin != itEnd; itBegin++)
        {
            if (*itBegin != L',' && *itBegin != L'\n')
            {
                buffComma += *itBegin;
                if (*itBegin == L'"')
                {
                    if (!doubleQuoteMode)
                    {
                        doubleQuoteMode = true;
                    }
                    else
                    {
                        doubleQuoteMode = false;
                    }
                }
            }
            else if (*itBegin == L',')
            {
                if (!doubleQuoteMode)
                {
                    work.push_back(buffComma);
                    buffComma.clear();
                }
                else
                {
                    buffComma += *itBegin;
                }
            }
            else if (*itBegin == L'\n')
            {
                if (!doubleQuoteMode)
                {
                    work.push_back(buffComma);
                    buffComma.clear();
                    csvData.push_back(work);
                    work.clear();
                }
                else
                {
                    buffComma += *itBegin;
                }
            }
        }

        return csvData;
    }

    static std::vector<std::vector<std::wstring> > ReadFromString(const std::wstring& text)
    {
        std::vector<std::vector<std::wstring> > csvData;

        // 「"」記号で囲まれているとセル内改行ができることに注意
        // 「"」記号で囲まれているとセル内で「,」が使用できることに注意
        std::wstring buffComma;
        bool doubleQuoteMode = false;
        std::vector<std::wstring> work;
        std::wstring::const_iterator itBegin(text.cbegin());
        std::wstring::const_iterator itEnd(text.cend());;

        for (; itBegin != itEnd; itBegin++)
        {
            if (*itBegin != L',' && *itBegin != L'\n')
            {
                buffComma += *itBegin;
                if (*itBegin == L'"')
                {
                    if (!doubleQuoteMode)
                    {
                        doubleQuoteMode = true;
                    }
                    else
                    {
                        doubleQuoteMode = false;
                    }
                }
            }
            else if (*itBegin == L',')
            {
                if (!doubleQuoteMode)
                {
                    work.push_back(buffComma);
                    buffComma.clear();
                }
                else
                {
                    buffComma += *itBegin;
                }
            }
            else if (*itBegin == L'\n')
            {
                if (!doubleQuoteMode)
                {
                    work.push_back(buffComma);
                    buffComma.clear();
                    csvData.push_back(work);
                    work.clear();
                }
                else
                {
                    buffComma += *itBegin;
                }
            }
        }

        return csvData;
    }

    static void Write(const std::wstring& filepath, const std::vector<std::vector<std::wstring> >& csvData)
    {
        std::ofstream ofs(filepath, std::ios::binary);
        if (!ofs.is_open())
        {
            throw std::runtime_error("Cannot open file.");
        }

        // BOMを書き込む
        const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
        ofs.write((const char*)bom, sizeof(bom));

        for (const auto& row : csvData)
        {
            std::wstring line;
            for (std::size_t j = 0; j < row.size(); ++j)
            {
                line += row[j];
                if (j != row.size() - 1)
                {
                    line += L",";
                }
            }
            line += L"\n";

            // UTF-8に変換して出力
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, line.c_str(), -1, nullptr, 0, nullptr, nullptr);
            if (size_needed > 1)
            {
                std::string utf8str(size_needed - 1, 0); // null文字除く
                WideCharToMultiByte(CP_UTF8, 0, line.c_str(), -1, &utf8str[0], size_needed, nullptr, nullptr);
                ofs.write(utf8str.c_str(), utf8str.size());
            }
        }

        ofs.close();
    }

private:
    csv();
};

