//
//  Group.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#include "Group.hpp"
#include "VectorCalculation.hpp"
Matrix Group::getSampledARm(int n){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int seed  = int(tv.tv_usec); // microseconds
    vector<int> sample_ind;
    for(int i = 0;i<StockPtrs.size();i++){
     sample_ind.push_back(i);
    }
    // Shuffle the index
    mt19937 eng(seed);
    std::shuffle(begin(sample_ind), end(sample_ind), eng);
    // Take rows from AR matrix, whose row number is the first 30 shuffled index
    // Form a 30*60 matrix
    Matrix sample_ar_m;
    for(int i =0;i<n;i++){
            sample_ar_m.push_back(AR_all[sample_ind[i]]);
        }
    return sample_ar_m; //30*60
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
void Printvecotr(vector<double> v){
    for(int i=0;i<v.size();i++){
        cout<<v[i]<<"\t";
    }
    cout<<endl;
}


void Group::CalAR_all(){
    vector<Stock*>::iterator itr;
    for(itr=StockPtrs.begin();itr!=StockPtrs.end();itr++){
        
        // return of each stock
        vector<double> vStock =(*itr)->GetReturnVec();
        
        //return of index for corresponding dates
        string d1 = (*itr)->GetReturnBeginDate();
        string d60 = (*itr)->GetReturnEndDate();
        vector<double> vIndex =indexPtr->GetReturnVec(d1,d60);
        
        // Calculate store the abnormal return for each stock, store them in a matrix
        AR_all.push_back(vStock-vIndex);
    }
}


void Group::Bootstap30_Calculate_All(){
    // Calculate AR for all stocks in the group and store it in the AR_all matrix (165*60) if there are 165 stocks
    CalAR_all();
    // Bootstrap for 30 times
    for(int i = 0;i<30;i++){
       //For One bootstrap
        // sample 30 stocks get 30*60  AR vector
        Matrix sampledAR = getSampledARm();
        // take  their AAR(30*1) vector, store  in  AARm matrix, same  for CAAR
        vector<double> AARv  = CalavgAxis0(sampledAR); //60*1
        vector<double> CAARv  = VectoCumu(AARv);//60*1
        AARm.push_back(AARv);
        CAARm.push_back(CAARv);
    }
    cout<<"\nGroup "<<groupName<<" bootstrap 30 times. "<<endl;
    //After 30 bootstraps; AARm and CAARm  is 60*30
    avgAAR = CalavgAxis0(AARm);
    avgCAAR =CalavgAxis0(CAARm);
    stdAAR = CalStdAxis0(AARm);
    stdCAAR = CalStdAxis0(CAARm);
}
