//
//  Group.hpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#ifndef Group_hpp
#define Group_hpp
#include <string>
#include <stdio.h>
#include <vector>
#include "Stock.hpp"
#include <algorithm>
#include <random>
#include  <vector>
#include <cmath>
#include "VectorCalculation.hpp"


using namespace std;
typedef vector<vector<double>> Matrix;
class Group{
private:
    string groupName;
    vector<Stock*> StockPtrs;
    vector<Stock*> SampleStockPtrs;
    Matrix AARm;
    Matrix CAARm;
    Matrix AR_all; //166*61
//     AR_sample_index; //  30 *61
    vector<double> avgAAR;
    vector<double> avgCAAR;
    vector<double> stdAAR;
    vector<double> stdCAAR;
public:
    Matrix getSampledARm(int n=30); //return 30*61 AR matrix
    static Index * indexPtr;
    Group(const string & Name):groupName(Name)
    { 
    }
    
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
    void Sample(int n =30); //Set SampleStockPtrs
    
    static void IndexSearch_CalReturn(){
        indexPtr->SearchPrice();
        indexPtr->CalReturn();
    }
    vector<double> CalAARv();//60*1
//    vector<double> CalCAARv();//60*1
    
    void Cal_AARm_CAARm(); // 60*30 //assign to AAR
    void CalAARavg(); //60*1
    void CalCAARavg();//60*1
    void CalAARstd();//60*1
    void CalCAARstd();//60*1
    void Bootstap30_Calculate_All();//Return [AAR,CAAR,AARstd,CAARstd], each element is a  60*1 vector
    vector<double> getAARavg(){return avgAAR;}
    vector<double> getCAARavg(){return avgCAAR;}
    vector<double> getAARstd(){return stdAAR;}
    vector<double> getCAARstd(){return stdCAAR;}
    
};
#endif /* Group_hpp */
