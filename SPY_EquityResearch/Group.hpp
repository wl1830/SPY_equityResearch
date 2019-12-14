//
//  Group.hpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef Group_hpp
#define Group_hpp
#include <string>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <iostream>

#include "Stock.hpp"
#include <algorithm>
#include <random>
#include  <vector>
#include <cmath>
#include "VectorCalculation.hpp"
#include <stdlib.h>
#include <time.h>

using namespace std;
typedef vector<vector<double>> Matrix;
class Group{
private:
    string groupName;
    vector<Stock*> StockPtrs;
    
    Matrix AR_all; // n_stock*60
    
    Matrix AARm; // n_Boot*60
    Matrix CAARm; // n_Boot*60
    
    vector<double> avgAAR; //60*1
    vector<double> avgCAAR; //60*1
    vector<double> stdAAR; //60*1
    vector<double> stdCAAR; //60*1
public:
    static Index * indexPtr;
    
    Group(const string & Name):groupName(Name){}
    
    void addStock(Stock * stockptr){StockPtrs.push_back(stockptr);}
    vector<Stock*> getStockPtr()const{return StockPtrs;}
    
    // Calculate and store
    void CalAR_all(); // AR for all stock
    Matrix getSampledARm(int n=30); //return n_sample * 60 AR matrix
    void Bootstap1_Calculate();
    void Bootstap30_Calculate_All();//Calculate and store AAR,CAAR,AARstd,CAARstd, each element is a  60*1 vector
    
    
    // Get the calculated result
    vector<double> getAARavg(){return avgAAR;}
    vector<double> getCAARavg(){return avgCAAR;}
    vector<double> getAARstd(){return stdAAR;}
    vector<double> getCAARstd(){return stdCAAR;}
    
};
#endif /* Group_hpp */
