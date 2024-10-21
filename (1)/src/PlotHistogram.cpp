#include "PlotHistogram.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>



void PlotHistogram::DrawHistogram(){
FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe) {
        // Gnuplot-Befehle
        fprintf(gnuplotPipe, "set terminal pngcairo size 800,600\n");
        fprintf(gnuplotPipe, "set output 'vector_histogram.png'\n");
        fprintf(gnuplotPipe, "set title 'Histogram of Vector Data'\n");
        fprintf(gnuplotPipe, "set xlabel 'Values'\n");
        fprintf(gnuplotPipe, "set ylabel 'Frequency'\n");
        fprintf(gnuplotPipe, "bin_width = 5\n");
        fprintf(gnuplotPipe, "bin(x, width) = width * floor(x / width) + width / 2.0\n");
        
        // Beginne das Plotten der Daten
        fprintf(gnuplotPipe, "plot '-' using (bin($1,bin_width)):(1.0) smooth freq with boxes\n");

        // Übergib die Daten aus dem Vektor an gnuplot
        for (double value : _histVec) {
            fprintf(gnuplotPipe, "%f\n", value);
        }

        // Datenübergabe beenden
        fprintf(gnuplotPipe, "e\n");
        
        // Schließe die Pipe zu gnuplot
        pclose(gnuplotPipe);
    } else {
        std::cerr << "Fehler: Gnuplot konnte nicht geöffnet werden." << std::endl;
    }
}
