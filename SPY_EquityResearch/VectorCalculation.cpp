
//  VectorCalculation.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/10/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#include "VectorCalculation.hpp"

vector<double> operator-(const vector<double>& V,const vector<double>& M){
    
    size_t s = V.size();
    vector<double> W(s);
    
for (int j=0; j<V.size(); j++)
{
    W[j]=V[j]-M[j];
}
return W;
}
