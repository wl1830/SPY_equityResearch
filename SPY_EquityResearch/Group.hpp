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
using namespace std;
typedef vector<vector<double>> Matrix;
class Group{
private:
    string groupName;
    vector<Stock*> StockPtrs;
    vector<Stock*> SampleStockPtrs;
    
public:
    static Index * indexPtr;
    Group(const string & Name):groupName(Name)
    {cout<<" df";}
    
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
    
    
    void BootStrap(int n = 30); //Set SampleStockPtrs
    Matrix CalAR(); //return 60*30
    vector<double> CalAAR(); //return 60*1
    vector<double> CalCAAR(); //return 60*1
    vector<double> CalAARstd(); //return 60*1
    vector<double> CalCAARstd(); //return 60*1
    Matrix Calculate(); //Return [AAR,CAAR,AARstd,CAARstd], each element is a  60*1 vector
    
};
#endif /* Group_hpp */
