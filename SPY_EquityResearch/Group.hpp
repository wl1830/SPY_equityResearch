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
    vector<Stock*> SampleStockPtrs;
    Matrix AR_all; //166*61
    Matrix AARm;
    Matrix CAARm;
    vector<double> avgAAR;
    vector<double> avgCAAR;
    vector<double> stdAAR;
    vector<double> stdCAAR;
public:
    static Index * indexPtr;
    Group(const string & Name):groupName(Name){}
    
    void addStock(Stock * stockptr){
        StockPtrs.push_back(stockptr);
    }

    vector<Stock*> getStockPtr()const{
        return StockPtrs;
    }
    Index * getIndexPtr()const{
        return indexPtr;
    }
    string getGroupName() const{
        return groupName;
    }
    
    void CalAR_all(); //for all stocks
    
    Matrix getSampledARm(int n=30); //return n*60 AR matrix
    
    void Bootstap30_Calculate_All();//Calculate AAR,CAAR,AARstd,CAARstd, each element is a  60*1 vector
    vector<double> getAARavg(){return avgAAR;}
    vector<double> getCAARavg(){return avgCAAR;}
    vector<double> getAARstd(){return stdAAR;}
    vector<double> getCAARstd(){return stdCAAR;}
    
};
#endif /* Group_hpp */
