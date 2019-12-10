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
    Matrix AR;
    Matrix CAR;
    vector<double> AAR;
    vector<double> ACAR;
    vector<double> stdAR;
    vector<double> stdCAR;
public:
    
    static Index * indexPtr;
    Group(const string & Name):groupName(Name)
    {}
    
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
    
    
    void BootStrap(int n =30); //Set SampleStockPtrs
    
    static void IndexSearch_CalReturn(){
        indexPtr->SearchPrice();
        indexPtr->CalReturn();
    }
    void CalAR(); // 60*30
    void CalCAR(); // 60*30
    
    void CalAAR(); //60*1
    void CalACAR();//60*1
    void  CalARstd();//60*1
    void CalCARstd();//60*1
    void Bootstap_Calculate_All();//Return [AAR,CAAR,AARstd,CAARstd], each element is a  60*1 vector
    
};
#endif /* Group_hpp */
