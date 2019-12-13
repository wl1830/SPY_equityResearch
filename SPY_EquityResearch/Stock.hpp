//
//  Stock.hpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#ifndef Stock_hpp
#define Stock_hpp
#include <map>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;
class Equity{
private:
    string ticker;
protected:
    map<string,double>priceMap;
    map<string,double>returnMap;
public:
    Equity(const string &Ticker){
        ticker  = Ticker;
    }
    map<string,double> GetPriceMap()const{
        return priceMap;
    }
    string getTicker() const{
        return ticker;
        
    }
    void SearchPrice();
    void CalReturn();
   virtual  string getSearchStartDate()const=0;
    virtual string getSearchEndDate()const=0;
    void setPriceMap(map<string,double> m){
        map<string,double>::iterator  itr;
        for(itr= m.begin();itr!=m.end();itr++){
            priceMap[itr->first] = itr->second;
        }
    }
};

class Stock:public Equity{
private:
    string date0,date_minus_30,date_30; //only date
    double EPS,EEPS,surprise;
    
public:
    string GetReturnBeginDate()const{
        return returnMap.begin()->first;
    }
    string GetReturnEndDate()const{
        return date_30;
    }
    
    Stock(const string &Ticker_,const string &datezero_,const string &date_minus_30_,const string &date_30_,const string &DateTimeZero_,double EPS_,double EEPS_):Equity(Ticker_),EPS(EPS_),EEPS(EEPS_),date0( datezero_),date_minus_30( date_minus_30_),date_30( date_30_)
    {
//        cout<<"Default constructor of stock"<<endl;
        if (EEPS == 0) {surprise  = (EPS- EEPS)/EPS;}
        else surprise  = (EPS- EEPS)/EEPS;

    }
    vector<double> GetReturnVec();

    double getSurprise()const{
        return surprise;
    }

    
    string getDate_minus_30()const{
        
       return date_minus_30;
   }
    string getDetdate_30()const{
        return date_30;
    }
    string getSearchStartDate()const{
         
        return date_minus_30;
    }
    string getSearchEndDate()const{
        return date_30;
        
    }
    string getDatezero()const{
        return date0;
    }
    double getEstimateEPS()const{
        return EEPS;
    }
    double getActualEPS()const{
        return EPS;
    }
    void SearchPrice();//get prices
};


class Index:public Equity{
private:
    string startdate,enddate;
public:
    Index(const string &Ticker_,const string & start,const string & end):Equity(Ticker_),startdate(start),enddate(end){}
    
    void Print(){
        cout<<startdate<<" "<<enddate<<endl;
    }
    vector<double> GetReturnVec(string date1,string date2)const;
    void SearchPrice();//get prices
    
    string getSearchStartDate()const{
        return startdate;
    }
    string getSearchEndDate()const{
        return enddate;
    }
};

void searchEquities(map<string,Equity*> pool);


#endif /* Stock_hpp */
