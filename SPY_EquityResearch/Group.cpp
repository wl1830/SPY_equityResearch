//
//  Group.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#include "Group.hpp"
#include "VectorCalculation.hpp"
void Group::Sample(int n){
    
    srand(int(time(NULL)));
    auto rng = default_random_engine {};
    std::shuffle(begin(StockPtrs), end(StockPtrs), rng);
    vector<Stock*>::iterator itr;
    int count=0;
    
        for(itr=StockPtrs.begin();itr!=StockPtrs.end();itr++){
            SampleStockPtrs.push_back(*itr);
        count++;
            if(count==n){
                break;
            }
        }
    cout<<"\nGroup "<<groupName<<" bootstrap: "<<endl;
    cout<<"The sampled stocks in are"<<endl;
    for(itr=SampleStockPtrs.begin();itr!=SampleStockPtrs.end();itr++){
        cout<<(*itr)->getTicker()<<" ";
    }
    cout<<endl;

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
                mean = (mean*s+m[s][d])/(s+1);
                var = (var*s+(m[s][d])*(m[s][d]))/(s+1.0);
            }
            stdV.push_back(sqrt(var-mean*mean));

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

vector<double> Group::CalAARv()
    {
    // Used for one bootstrap after calling Boot
    vector<Stock*>::iterator itr;
    Matrix ARM; //A matrix for AR of 30 stocks,30* 60
    for(itr=SampleStockPtrs.begin();itr!=SampleStockPtrs.end();itr++){
            vector<double> vStock =(*itr)->GetReturnVec();
            //return of stock for corresponding dates
            string d1 = (*itr)->GetReturnBeginDate();
            string d60 = (*itr)->GetReturnEndDate();
            vector<double> vIndex =indexPtr->GetReturnVec(d1,d60);
            ARM.push_back((vStock-vIndex)); //AR vector for every stock
    }
 
    return CalavgAxis0(ARM);   //AAR
}//60*1



// 60*30 //assign to AAR




                            //Return [AAR,CAAR,AARstd,CAARstd], each element is a  60*1 vector
//
void Group::Bootstap30_Calculate_All(){
    for(int i = 0;i<30;i++){
       //For One bootstrap
        // sample 30 stocks get 30*60  AR vector
        this->Sample();
        // take  their AAR(30*1) vector, store  in  AARm matrix, same  for CAAR
        vector<double> AARv  = this->CalAARv(); //60*1
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
