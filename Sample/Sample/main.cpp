#include <iostream>
#include "../../HeaderOnlyCsv.hpp"
#include <cassert>

int main()
{
    {
        std::vector<std::vector<std::string>> csvData = csv::Read("test.csv");
        csv::Write("test.out.csv", csvData);
    }

    // カンマ「,」が含まれているcsvファイル
    {
        std::vector<std::vector<std::string>> csvData = csv::Read("test2.csv");
        assert(csvData.size() == 2);
        assert(csvData.at(0).size() == 4);
        assert(csvData.at(1).size() == 4);

        assert(csvData.at(0).at(2) == "\"c,d\"");
        assert(csvData.at(1).at(1) == "\"test,test\"");

        csv::Write("test2.out.csv", csvData);
    }

    {
        std::vector<std::vector<std::string>> csvData = csv::ReadFromString("a,b,c\nd,e,f\ng,h,i");
        std::size_t s =csvData.size();
    }
    // おまけ
    // シーザー暗号
    {
        std::string str("あいうえお");
        const char* sz = str.c_str();
        std::size_t _size = str.size();
        char* work = nullptr;
        work = new char[_size+1];
        strcpy_s(work, _size+1, str.c_str());

        // 暗号化
        for (int i = 0; i < _size + 1; ++i)
        {
            work[i] = work[i]+10;
        }
        std::string str2(work);
        // 複合化
        for (int i = 0; i < _size + 1; ++i)
        {
            work[i] = work[i]-10;
        }
        std::string str3(work);

        delete[] work;
    }

    return 0;
}
