//
//  plot.hpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/7/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#ifndef plot_hpp
#define plot_hpp

#include <stdio.h>

#include <string>
#include <iostream>
using namespace std;
class Gnuplot {
public:
Gnuplot() ;
~Gnuplot();
void operator ()(const string & command);
// send any command to gnuplot
protected:
FILE *gnuplotpipe;
};
Gnuplot::Gnuplot() {
// with -persist option you will see the windows as your program ends
//gnuplotpipe=_popen("gnuplot -persist","w");
//without that option you will not see the window
 // because I choose the terminal to output files so I don't want to see the window
 gnuplotpipe= popen("gnuplot","w");
if (!gnuplotpipe) {
 cerr<< ("Gnuplot not found !"); }
}
Gnuplot::~Gnuplot() {
fprintf(gnuplotpipe,"exit\n");
pclose(gnuplotpipe);
}
void Gnuplot::operator()(const string & command) {
fprintf(gnuplotpipe,"%s\n",command.c_str());
fflush(gnuplotpipe);
// flush is necessary, nothing gets plotted else
};
#endif /* plot_hpp */
