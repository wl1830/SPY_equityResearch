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
    //n is sample size default 30
    vector<int> sample_ind;
    for(int i = 0;i<StockPtrs.size();i++){
        sample_ind.push_back(i);
    }
    srand(int(time(NULL)));
    auto rng = default_random_engine {};
    std::shuffle(begin(sample_ind), end(sample_ind), rng);
//    vector<Stock*>::iterator itr;
    Matrix sample_ar_m;
    
     
    for(int i =0;i<n;i++){
            sample_ar_m.push_back(AR_all[i]);
        }
//    cout<<"\nGroup "<<groupName<<" bootstrap. "<<endl;
    return sample_ar_m;
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
//            double var = 0.0;
//            double mean = 0.0;
        double var = 0.0;
        double mean = 0.0;
            for(int s=0;s<m.size();s++){
                mean = (mean*s+m[s][d])/(s+1.0);
                var = (var*s+(m[s][d]*m[s][d]))/(s+1.0);
            }
        if(-mean*mean+var<0){
            cout<<var;
            cout<<"\n"<<mean;
            cout<<"(var-mean*mean)"<<(var-mean*mean)<<"\t";}
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
        vector<double> vStock =(*itr)->GetReturnVec();
        //return of stock for corresponding dates
        string d1 = (*itr)->GetReturnBeginDate();
        string d60 = (*itr)->GetReturnEndDate();
        vector<double> vIndex =indexPtr->GetReturnVec(d1,d60);
        AR_all.push_back((vStock-vIndex)); //AR vector for every stock
    }
}

// 60*30 //assign to AAR




                            //Return [AAR,CAAR,AARstd,CAARstd], each element is a  60*1 vector
//
void Group::Bootstap30_Calculate_All(){
    CalAR_all();
    for(int i = 0;i<30;i++){
       //For One bootstrap
        // sample 30 stocks get 30*60  AR vector
        Matrix sampledAR = getSampledARm();
        // take  their AAR(30*1) vector, store  in  AARm matrix, same  for CAAR
        vector<double> AARv  = CalavgAxis0(sampledAR); //60*1
        vector<double> CAARv  = VectoCumu(AARv);//60*1
        AARm.push_back(AARv);
        CAARm.push_back(CAARv);
    // take samples;
    }
    //After 30 bootstraps; AARm and CAARm  is 60*30
    avgAAR = CalavgAxis0(AARm);
    avgCAAR =CalavgAxis0(CAARm);
    stdAAR = CalStdAxis0(AARm);
    stdCAAR = CalStdAxis0(CAARm);
}
