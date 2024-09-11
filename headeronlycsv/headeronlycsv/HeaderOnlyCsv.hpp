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
        std::string buff;
        std::string buffComma;
        int headerColSize = 0; // 先頭行の列数
        int row = 0;
        bool doubleQuoteMode = false;
        std::vector<std::string> work;

        // csvファイルは「"」記号で囲まれていればセル内で改行ができることを考慮する
        // 以下の3つの行はどれも3列となる。
        // a,b,c    ①
        // a,b,"c
        // d
        // e"       ②
        // a,"b",c  ③
        // ③のように「"」で囲まれているが改行しない場合もある

        // 改行まで読む
        while (std::getline(ifs, buff))
        {
            std::istringstream iss(buff);

            // カンマまで読む
            while (std::getline(iss, buffComma, ','))
            {
                // 先頭行だったら列数をカウントする
                if (row == 0)
                {
                    headerColSize++;
                }
                trim(buffComma);
                // 「"」記号がある
                if (buffComma.find('"') != std::string::npos)
                {
                    // 「"」記号が先頭にある
                    if (buffComma.at(0) == '"')
                    {
                        // 「"」記号が最後にない（③のパターンではない）
                        if (buffComma.find_last_of('"') != buffComma.size() - 1)
                        {
                            doubleQuoteMode = true;
                            std::string work2 = buffComma;
                            // ②のような「"」で囲まれ、改行を含む要素を読み取る
                            while (std::getline(ifs, buff))
                            {
                                iss = std::istringstream(buff);
                                while (std::getline(iss, buffComma, ','))
                                {
                                    work2 = work2 + "\n" + buffComma;
                                    if (buffComma.find_last_of('"') == buffComma.size()-1)
                                    {
                                        doubleQuoteMode = false;
                                        break;
                                    }
                                }
                                if (doubleQuoteMode == false)
                                {
                                    buffComma = work2;
                                    break;
                                }
                            }
                        }
                    }
                }
                work.push_back(buffComma);
            }
            if (doubleQuoteMode == false)
            {
                csvData.push_back(work);
                work.clear();
                row++;
            }
        }

        // 行の最後の要素がなかった場合、空文字を追加する
        // 以下のデータはどちらも3列あるとする。
        // a,b,c
        // a,b,
        // そのため、空文字のデータを追加する
        for (std::size_t i = 0; i < csvData.size(); ++i)
        {
            if (csvData.at(i).size() != headerColSize)
            {
                csvData.at(i).push_back("");
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

