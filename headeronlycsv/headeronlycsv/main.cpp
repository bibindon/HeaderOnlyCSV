#include "HeaderOnlyCsv.hpp"
#include <iostream>

int main()
{
    std::vector<std::vector<std::string>> csvData = csv::Read("test.csv");
    return 0;
}
