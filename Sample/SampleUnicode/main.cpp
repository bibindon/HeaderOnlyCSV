#include <iostream>
#include "../../HeaderOnlyCsvUnicode.hpp"
#include <cassert>

int main()
{
    {
        std::vector<std::vector<std::wstring>> csvData = csv::Read(L"test.csv");
        assert(!csvData.empty());
        csv::Write(L"test.out.csv", csvData);
    }

    // カンマ「,」が含まれているcsvファイル
    {
        std::vector<std::vector<std::wstring>> csvData = csv::Read(L"test2.csv");
        assert(csvData.size() == 2);
        assert(csvData.at(0).size() == 4);
        assert(csvData.at(1).size() == 4);

        assert(csvData.at(0).at(2) == L"\"c,d\"");
        assert(csvData.at(1).at(1) == L"\"test,test\"");

        csv::Write(L"test2.out.csv", csvData);
    }

    {
        std::vector<std::vector<std::wstring>> csvData = csv::ReadFromString(L"a,b,c\nd,e,f\ng,h,i");
        std::size_t s =csvData.size();
    }

    return 0;
}
