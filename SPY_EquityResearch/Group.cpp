//
//  Group.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#include "Group.hpp"

void Group::BootStrap(int n){
    srand(time(NULL));
    auto rng = default_random_engine {};
    std::shuffle(begin(StockPtrs), end(StockPtrs), rng);
    vector<Stock*>::iterator itr;
    int count=0;
    
        for(itr=StockPtrs.begin();itr!=StockPtrs.end();itr++){
            SampleStockPtrs.push_back(*itr);
        count++;
            if(count==30){
                break;
            }
        }
    cout<<"The size of sampleStockPtrs vector: "<<SampleStockPtrs.size()<<endl;
}
