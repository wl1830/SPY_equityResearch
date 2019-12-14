//
//  Group.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#include "Group.hpp"
#include "VectorCalculation.hpp"




void Group::CalAR_all(){
    vector<Stock*>::iterator itr;
    for(itr=StockPtrs.begin();itr!=StockPtrs.end();itr++){
        
        // return of each stock
        vector<double> StockRetVec =(*itr)->GetReturnVec();
        
        string d1 = (*itr)->GetReturnBeginDate();
        string d60 = (*itr)->GetReturnEndDate();
        
        vector<double> IndexRetVec =indexPtr->GetReturnVec(d1, d60);
        
        AR_all.push_back(StockRetVec-IndexRetVec);
    }
}


Matrix Group::getSampledARm(int n){
    
    vector<int> sample_ind;
    for(int i = 0;i<StockPtrs.size();i++){
     sample_ind.push_back(i);
    }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int seed  = tv.tv_usec; // microseconds
    mt19937 eng(seed); // Mersenne-Twister
    std::shuffle(begin(sample_ind), end(sample_ind), eng);
    
    
    Matrix sample_ar_m;
    for(int i =0;i<n;i++)
    {sample_ar_m.push_back(AR_all[sample_ind[i]]);}
    return sample_ar_m; //30*60
}


void Group::Bootstap1_Calculate(){
    
    Matrix sampledAR = getSampledARm(); //30 * 60
    vector<double> AARv  = CalavgAxis0(sampledAR); //60*1
    vector<double> CAARv  = VectoCumu(AARv);//60*1
    AARm.push_back(AARv);  // Store in AARm matrix
    CAARm.push_back(CAARv); // Store in CAARm matrix
}

void Group::Bootstap30_Calculate_All()
{
    // Calculate AR for all stocks in the group
    // Store ARs in the AR_all matrix (n*60) if n stocks
    CalAR_all();
    AARm.clear();
    CAARm.clear();
    // Bootstrap for 30 times, store AAR CAAR of each bootstrap in AARm, CAARm
    for(int i = 0;i<30;i++){
       Bootstap1_Calculate();
        
    }
    cout<<"\nGroup "<<groupName<<" bootstrap 30 times. "<<endl;
    
    
    //After 30 bootstraps; AARm and CAARm  is 60*30
    avgAAR = CalavgAxis0(AARm); // Take the average of AARm and store in avgAAR
    avgCAAR =CalavgAxis0(CAARm); // Take the average of CAARm and store in avgCAAR
    stdAAR = CalStdAxis0(AARm); // Take the std of AARm and store in stdAAR
    stdCAAR = CalStdAxis0(CAARm); // Take the std of AARm and store in stdAAR
    
    
}
