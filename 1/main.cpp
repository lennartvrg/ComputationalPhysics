#include <iostream>
// #include "src/PlotHistogram.h"
#include "src/GenerateRandom.h"

int main(){
    std::cout<<"start the program" <<std::endl;
    GenerateRandom genRand(5);
std::vector<std::pair<double,double>> randNumPairs=genRand.GetRanVec();

for(int i=0;i<5;i++){
    std::cout<<"x="<<randNumPairs.at(i).first<<" y= "<<randNumPairs.at(i).second<<std::endl;
}


}