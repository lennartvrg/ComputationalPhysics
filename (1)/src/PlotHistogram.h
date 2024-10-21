#pragma once
#include <iostream>
#include <vector>

class PlotHistogram{
    public:
    PlotHistogram(std::vector<double> histVec){_histVec=histVec;};
    void DrawHistogram();
private:
std::vector<double> _histVec;

};