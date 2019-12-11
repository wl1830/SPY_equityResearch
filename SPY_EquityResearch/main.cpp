//
//  main.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Stock.hpp"
#include "Group.hpp"
#include <cstring>
#include <algorithm> //for std::sort
#include "gnuplot.hpp"

using namespace std;

//initialize the static variable indexitr
Index * Group::indexPtr = nullptr;

int main(int argc, const char * argv[]) {
    
    // insert code here...
    // Read in ticker, time, EPS and EEPS

        ifstream infile;
       /*
        If  cannt read, In Xcode go to Product > Scheme > Edit Scheme > Run test (on the right) > Options (middle top)
        Down under Options check “Use custom working directory” and set it to the directory where your files are located.
        */
        string epspath ="EPS_date_temp.csv";
        infile.open(epspath);
        string  ticker, actualEPSstr,estimateEPSstr;
        double actualEPS,estimateEPS;
        string  datetimezero;
        string  datezero,date_minus_30,date_30;
        string line;
        
        if (!infile.is_open()){
            cout<<"File failed to open"<<endl;
            return 1;
        }
        getline(infile, line);
        cout<< "Read the following information: ";
        cout << line << endl;
        double minDatenum =20191231;
        double maxDatenum = 20190101;
        vector<Stock*> Stocks;
        vector<double> Surprises;
        
        while (getline(infile,line)){
            //line 'Ticker', 'Actual', 'Estimate', 'Time','Date','date_minus_30', 'date_30'
            stringstream ss(line);
            getline(ss,ticker,',');
            getline(ss,actualEPSstr,',');
            getline(ss,estimateEPSstr,',');
            getline(ss,datetimezero,',');
            getline(ss,datezero,',');
            getline(ss,date_minus_30,',');
            getline(ss,date_30,',');
            actualEPS = stod(actualEPSstr);
            estimateEPS = stod(estimateEPSstr);
            double sdatenum,edatenum;
            // change the time format 2019-01-23 06:00:00 to 2019-01-23T06:00:00
            datetimezero = datetimezero.substr(0,10)+"T"+datetimezero.substr(11,8);
            
            //update the min max date
            sdatenum = (stod(date_minus_30.substr(0,4))*10000) +
                    (stod(date_minus_30.substr(5,2))*100) +
                    stod(date_minus_30.substr(8,9));
            edatenum = (stod(date_30.substr(0,4))*10000) +
                        (stod(date_30.substr(5,2))*100) +
                        stod(date_30.substr(8,9));
            if(sdatenum<minDatenum ){minDatenum=sdatenum;}
            if(edatenum>maxDatenum ){maxDatenum=edatenum;}
    
            
            // Create  stock objects
            Stock * stock= new Stock(ticker,datezero,date_minus_30,date_30,datetimezero,actualEPS,estimateEPS);// surprises is calculated in constructor
            Surprises.push_back(stock->getSurprise());
            Stocks.push_back(stock);
        }
    infile.close();
    
    // Find the 2 threshold of Surprise to group stocks
    sort(Surprises.begin(),Surprises.end());
    auto const Q1 = Surprises.size() / 3;
    auto const Q2 = 2 * Q1;
    double Thres1 = Surprises[Q1] ;
    double Thres2 = Surprises[Q2] ;
    cout<<"\nLower threshold of surprise:" <<Thres1<<endl;
    cout<<"Uper threshold of surprise:" <<Thres2<<endl;

    // Create 3 Groups and fill with corresponding stock pointers
    Group Beat("Beat");
    Group Meet("Meet");
    Group Miss("Miss");
    
    // store the stock pointers into the Group objects according to surprise value
    vector<Stock*>::iterator Stocksitr;
    for(Stocksitr=Stocks.begin();Stocksitr!=Stocks.end();Stocksitr++)
    {
        if((*Stocksitr)->getSurprise()>Thres2){
            Beat.addStock(*Stocksitr);
        }
        else if ((*Stocksitr)->getSurprise()>Thres1){
            Meet.addStock(*Stocksitr);
        }
        else{ Miss.addStock(*Stocksitr);
        }
    }
    cout<<"\nThere are "<<Beat.getStockPtr().size()<<" stocks in the Beat group."<<endl;
    cout<<"There are "<<Meet.getStockPtr().size()<<" stocks in the Meet group."<<endl;
    cout<<"There are "<<Miss.getStockPtr().size()<<" stocks in the Miss group."<<endl;
    
    
    // Min and Max date of all stocks, used for index to search adjClose
    string minDate = to_string(minDatenum).substr(0,4)+"-"+to_string(minDatenum).substr(4,2)+"-"+to_string(minDatenum).substr(6,2);
    string maxDate = to_string(maxDatenum).substr(0,4)+"-"+to_string(maxDatenum).substr(4,2)+"-"+to_string(maxDatenum).substr(6,2);
    /**/
    
    cout<< "\nThe earliest day -30 is "<< minDate<<endl;
    cout<< "The latest day 30 is "<< maxDate<<endl;
    
    
    // Create SPY index and add it to 3 groups
     Group::indexPtr = new Index("SPY",minDate,maxDate);
   
    // Calculation
    // Search price Caculate return for all dates;
    Group::IndexSearch_CalReturn();
    // Caculate all;
    Beat.Bootstap_Calculate_All();
//    Meet.Bootstap_Calculate_All();
//    Miss.Bootstap_Calculate_All();
////
    vector<double> BeatACAR = Beat.getACAR();
    vector<double> MeetACAR = Meet.getACAR();
    vector<double> MissACAR = Miss.getACAR();
//
    cout<<"Calculation for three groups done!"<<endl;
    if(BeatACAR.size()!=60|MeetACAR.size()!=60|MissACAR.size()!=60){
        cout<<"Dimension of  metrics have problem";
    }else{cout<<"Metrics checked!"<<endl;}
    // Bootstrap stocks
    cout<<"BeatACAR"<<endl;
    for(int i=0;i<BeatACAR.size();i++){
        cout<<BeatACAR[i]<<endl;
    }
    cout<<"MeetACAR"<<endl;
    for(int i=0;i<MeetACAR.size();i++){
        cout<<MeetACAR[i]<<endl;
    }
    cout<<"MissACAR"<<endl;
    for(int i=0;i<MissACAR.size();i++){
        cout<<MissACAR[i]<<endl;
    }
//
//    // For each stock, seach or enough prices, find date-30, date 30
//    // Yahoo get prices for sampled stocks
//
////    int n = 377;
////    cout<<Stocks[n]->getTicker()<<endl;
////    Stocks[n]->SearchPrice();
////    cout<<"Pricemap size:"<<Stocks[n]->GetPriceMap().size()<<endl;
////    Stocks[n]->CalReturn();
//
//
////    for(Stocksitr=(Stocks.begin()+n);Stocksitr!=Stocks.end();Stocksitr++){
////        cout<<(*Stocksitr)->getTicker()<<endl;
////        (*Stocksitr)->SearchPrice();
////        (*Stocksitr)->CalReturn();
////        cout<<(*Stocksitr)->GetReturnBeginDate()<<endl;
////        cout<<(*Stocksitr)->GetReturnEndDate()<<endl;
////        vector<double> partReturns =Group::indexPtr->GetReturnVec((*Stocksitr)->GetReturnBeginDate(), (*Stocksitr)->GetReturnEndDate());
////    }
//
////
//////
//// cout<<Stocks[n]->GetReturnBeginDate()<<" "<<Stocks[n]->GetReturnEndDate();
////    vector<double> partReturns =Group::indexPtr->GetReturnVec("2018-12-10",
////    "2019-03-07");
////
////    map<string,double>::iteratr it;
////    for(it= Stocks[100]->GetPriceMap().begin();it!=Stocks[100]->GetPriceMap().end();it++){
////        cout<<it->first<<endl;
////        cout<<it->second<<endl;
////    }
//
//    // Yahoo get prices for index
//    // Calculations
//    // Populate the stock maps and AAR/CAAR matrix
    // Plot
    int i = 0;
    int nIntervals = 59;
    double stepSize = 1.0;
    double* xData = (double*) malloc((nIntervals+1)*sizeof(double));
    double* yData = (double*) malloc((nIntervals+1)*sizeof(double));
    double* yData2 = (double*) malloc((nIntervals+1)*sizeof(double));
    double* yData3 = (double*) malloc((nIntervals+1)*sizeof(double));
    xData[0] = 1.0;
    double x0 = 1.0;
    for (i = 0; i < nIntervals; i++) {
        x0 = xData[i];
        xData[i+1] = x0 + stepSize;
    }
    for (i = 0; i <= nIntervals; i++) {
        yData[i] = BeatACAR[i];
    }
    for (i = 0; i <= nIntervals; i++) {

        yData2[i] = MeetACAR[i];
    }
    for (i = 0; i <= nIntervals; i++) {

           yData3[i] = MissACAR[i];
       }

    plotResults(xData, yData, yData2, yData3, nIntervals);
//
//    // Menu
    // Delete stocks and spy
        }
