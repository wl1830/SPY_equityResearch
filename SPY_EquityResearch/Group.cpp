//
//  Group.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#include "Group.hpp"
#include "VectorCalculation.hpp"
void Group::BootStrap(int n){
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
    cout<<"The sampled stocks are"<<endl;
    for(itr=SampleStockPtrs.begin();itr!=SampleStockPtrs.end();itr++){
        cout<<(*itr)->getTicker()<<" ";
    }
    cout<<"Bootstrapped the stocks"<<endl;
    cout<<"The size of sampleStockPtrs vector: "<<SampleStockPtrs.size()<<endl;
}


void Group::CalAR(){
    
    
    vector<Stock*>::iterator itr;
    for(itr=SampleStockPtrs.begin();itr!=SampleStockPtrs.end();itr++){
        (*itr)->SearchPrice();
        (*itr)->CalReturn();
        string d1 = (*itr)->GetReturnBeginDate();
        string d60 = (*itr)->GetReturnBeginDate();
        vector<double> vStock =(*itr)->GetReturnVec();
        vector<double> vIndex =indexPtr->GetReturnVec(d1,d60);
        AR.push_back((vStock-vIndex));
        cout<<"The size of AR:"<<AR.size()<<" * "<<AR[0].size()<<endl;
    }
    
}

void  Group::CalCAR(){
    
    for(int s=0;s<AR.size();s++){
        double cu = 0;
        vector<double> temp;
        for(int d=0;d<AR[0].size();d++){
            cu += AR[s][d];
            temp.push_back(cu);
            cout<<"cu is "<<cu<<" ";
        }
        cout<<endl;
        CAR.push_back(temp);
    }
    cout<<"The size of AR:"<<CAR.size()<<" * "<<CAR[0].size()<<endl;
}

void Group::CalAAR(){ // 60*1average of abnormal return
        
    for(int d=0;d<AR[0].size();d++){
        double mean = 0;
        for(int s=0;s<AR.size();s++){
            mean = (mean*s+AR[s][d])/(s+1);
        }
        AAR.push_back(mean);
    }
    cout<<"The size of AAR:"<<AAR.size()<<endl;
}

void  Group::CalACAR(){//average of cumulative abnormal return
    
    for(int d=0;d<AR[0].size();d++){
        double mean = 0;
        for(int s=0;s<AR.size();s++){
            mean = (mean*s+AR[s][d])/(s+1);
        }
        AAR.push_back(mean);
    }
    cout<<"The size of AAR:"<<AAR.size()<<endl;
}
void  Group::CalARstd(){
    for(int d=0;d<AR[0].size();d++){
        double var = 0;
        double mean = 0;
        for(int s=0;s<AR.size();s++){
            mean = (mean*s+AR[s][d])/(s+1);
            var = (var*s+(AR[s][d])*(AR[s][d]))/(s+1);
        }
        stdAR.push_back(sqrt(var-mean*mean));
        
    }
}
void  Group::CalCARstd(){
    for(int d=0;d<CAR[0].size();d++){
        double var = 0;
        double mean = 0;
        for(int s=0;s<CAR.size();s++){
            mean = (mean*s+CAR[s][d])/(s+1);
            var = (var*s+(CAR[s][d])*(CAR[s][d]))/(s+1);
        }
        stdCAR.push_back(sqrt(var-mean*mean));
        
    }
}
void Group::Bootstap_Calculate_All(){
    this->BootStrap();
    this->CalAR();
    this->CalCAR();
    this->CalARstd();
    this->CalCARstd();
}
