//
//  VectorCalculation.hpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/10/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#ifndef VectorCalculation_hpp
#define VectorCalculation_hpp
#include<vector>
#include <stdio.h>
#include<cmath>
using namespace std;
typedef vector<vector<double>> Matrix;

vector<double> operator-(const vector<double>& V,const vector<double>& M);
vector<double> CalavgAxis0(Matrix m);
vector<double> CalStdAxis0(Matrix m);
vector<double> VectoCumu(vector<double> v);

#endif /* VectorCalculation_hpp */
