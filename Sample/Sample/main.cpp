#include <iostream>
#include "../../HeaderOnlyCsv.hpp"

int main()
{
    std::vector<std::vector<std::string>> csvData = csv::Read("test.csv");
    csv::Write("test2.csv", csvData);
    return 0;
}
