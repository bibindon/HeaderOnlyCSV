#include <iostream>
#include "../../HeaderOnlyCsv.hpp"

int main()
{
    {
        std::vector<std::vector<std::string>> csvData = csv::Read("test.csv");
        csv::Write("test2.csv", csvData);
    }

    {
        std::vector<std::vector<std::string>> csvData = csv::ReadFromString("a,b,c\nd,e,f\ng,h,i");
        std::size_t s =csvData.size();
    }
    // ���܂�
    // �V�[�U�[�Í�
    {
        std::string str("����������");
        const char* sz = str.c_str();
        std::size_t _size = str.size();
        char* work = nullptr;
        work = new char[_size+1];
        strcpy_s(work, _size+1, str.c_str());

        // �Í���
        for (int i = 0; i < _size + 1; ++i)
        {
            work[i] = work[i]+10;
        }
        std::string str2(work);
        // ������
        for (int i = 0; i < _size + 1; ++i)
        {
            work[i] = work[i]-10;
        }
        std::string str3(work);

        delete[] work;
    }

    return 0;
}
