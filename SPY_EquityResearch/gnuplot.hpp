//
//  gnuplot.hpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/7/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#ifndef gnuplot_hpp
#define gnuplot_hpp
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

void PlotVectors( vector<double> v1,vector<double> v2,vector<double> v3,
                 int nIntervals = 59,double stepSize = 1.0,double x0= -30.0,
                 const char * FileName1 = "Beat",
                 const char * FileName2 = "Meet",
                 const char * FileName3 = "Miss");

#endif /* gnuplot_hpp */
