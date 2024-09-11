#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include "Shlwapi.h"
#pragma comment( lib, "Shlwapi.lib" ) 

class csv
{
public:

    static std::vector<std::vector<std::string> > Read(const std::string& filepath)
    {
        std::vector<std::vector<std::string> > csvData;
        int result = PathFileExists(filepath.c_str());
        if (result == 0)
        {
            return csvData;
        }

        std::ifstream ifs(filepath);
        std::string buffComma;
        bool doubleQuoteMode = false;
        std::vector<std::string> work;
        std::istreambuf_iterator<char> itBegin(ifs);
        std::istreambuf_iterator<char> itEnd;

        for (; itBegin != itEnd; itBegin++)
        {
            if (*itBegin != ',' && *itBegin != '\n')
            {
                buffComma += *itBegin;
                if (*itBegin == '"')
                {
                    if (doubleQuoteMode == false)
                    {
                        doubleQuoteMode = true;
                    }
                    else
                    {
                        doubleQuoteMode = false;
                    }
                }
            }
            else if (*itBegin == ',')
            {
                work.push_back(buffComma);
                buffComma.clear();
            }
            else if (*itBegin == '\n')
            {
                if (doubleQuoteMode == false)
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

private:
    csv();

    static void ltrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            [](unsigned char ch)
            {
                return !std::isspace(ch);
            }
        ));
    }

    static void rtrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(),
            [](unsigned char ch)
            {
                return !std::isspace(ch);
            }
        ).base(), s.end());
    }

    static void trim(std::string& s)
    {
        rtrim(s);
        ltrim(s);
    }

//    std::vector<std::vector<std::string> > m_csvData;
};

