#pragma once
#include <iostream>
#include <vector>
#include <utility>

class GenerateRandom{
    public:
    GenerateRandom(int N);
    std::vector<std::pair<double,double>> GetRanVec(){return _randNumbers;};
private:
std::vector<std::pair<double,double>> _randNumbers;

};