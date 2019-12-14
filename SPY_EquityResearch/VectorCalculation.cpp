
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

vector<double> CalavgAxis0(Matrix m){
    vector<double> meanV;
    for(int d=0;d<m[0].size();d++){
        double mean = 0.0;
        for(int s=0;s<m.size();s++){
            mean = (mean*s+m[s][d])/(s+1.0);
        }
        meanV.push_back(mean);
    }
    return meanV;
}
vector<double> CalStdAxis0(Matrix m){
    vector<double> stdV;
    for(int d=0;d<m[0].size();d++){
        double var = 0.0;
        double mean = 0.0;
            for(int s=0;s<m.size();s++){
                mean = (mean*s+m[s][d])/(s+1.0);
                var = (var*s+(m[s][d]*m[s][d]))/(s+1.0);
            }
            stdV.push_back(sqrt(-mean*mean+var));
    }
    return stdV;
}

vector<double> VectoCumu(vector<double> v)
{
    double cu = 0;
    vector<double>  cuV;
    for(int i = 0;i<v.size();i++){
        cu+=v[i];
        cuV.push_back(cu);
    }
    return  cuV;
}

