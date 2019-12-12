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
};

class Stock:public Equity{
private:
    string date0,date_minus_30,date_30; //only date
//    string datetime0; //date+time
    double EPS,EEPS,surprise;
    
public:
    string GetReturnBeginDate()const{
        map<string,double>::iterator itr;
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
    vector<double> GetReturnVec ();
    void Print(){
        cout<<getTicker()<< " "<< date0<<" "<< EPS<<" "<<EEPS<<endl;
        cout<<surprise<<endl;
    }
    double getSurprise()const{
        return surprise;
    }
    void setPriceMap(map<string,double> m){
        map<string,double>::iterator  itr;
        for(itr= m.begin();itr!=m.end();itr++){
            priceMap[itr->first] = itr->second;
        }
    }
    
    string getDate_minus_30()const{
        
       return date_minus_30;
   }
    string getDetdate_30()const{
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
   
};
void  searchStocks(map<string,Stock*> pool);

#endif /* Stock_hpp */
