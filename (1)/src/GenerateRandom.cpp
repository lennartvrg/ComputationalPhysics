#include <iostream>
#include <vector>
#include <random>
#include "GenerateRandom.h"
#include <utility>


GenerateRandom::GenerateRandom(int N) {
    
    std::random_device rd;  
    std::mt19937 gen(rd()); 

    // Uniforme Verteilung zwischen -1 und 1
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // generate rand number 
 for (int i = 0; i < N; ++i) {
        std::pair<double, double> randomPair;
        randomPair.first = dis(gen);  
        randomPair.second = dis(gen);
        _randNumbers.push_back(randomPair);
    }
}