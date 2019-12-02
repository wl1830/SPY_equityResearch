//
//  main.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Stock.hpp"
#include "Group.hpp"
#include <algorithm> //for std::sort
using namespace std;

//initialize the static variable indexitr
Index * Group::indexPtr = nullptr;
int main(int argc, const char * argv[]) {
    // insert code here...
    // Read in ticker, time, EPS and EEPS
    ifstream infile;
       /*
        If  cannt read, In Xcode go to Product > Scheme > Edit Scheme > Run test (on the right) > Options (middle top)

        Down under Options check “Use custom working directory” and set it to the directory where you .txt files are located.
        */
        string epspath ="EPS.csv";
        infile.open(epspath);
        string  ticker, actualEPSstr,estimateEPSstr;
        double actualEPS,estimateEPS;
        string  datetimezero;
        string  datezero;
        string line;
        
        if (!infile.is_open()){
            cout<<"File failed to open"<<endl;
            return 1;
        }
        getline(infile, line);
        cout << line << endl;
        double minDatenum = 12.31;
        double maxDatenum = 1.01;
        vector<Stock*> Stocks;
        vector<double> Surprises;
        double datenum;
        while (getline(infile,line)){
            stringstream ss(line);
            getline(ss,ticker,',');
            getline(ss,actualEPSstr,',');
            getline(ss,estimateEPSstr,',');
            actualEPS = stod(actualEPSstr);
            estimateEPS = stod(estimateEPSstr);
            getline(ss,datetimezero,',');
            // change the time format 2019-01-23 06:00:00 to 2019-01-23T06:00:00
            datetimezero = datetimezero.substr(0,10)+"T"+datetimezero.substr(11,8);
            // Get rid of time, only keep the date
            datezero = datetimezero.substr(0,10);
            //update the min max date
            datenum = stod(datezero.substr(5,2)+"." + datezero.substr(8,9));
            if(datenum<minDatenum){minDatenum=datenum;}
            if(datenum>maxDatenum){maxDatenum=datenum;}
            
            // Give a warning when estimate =0
            if(estimateEPS==0){cout<<ticker<<" has zero EEPS, its actual EPS is "<<actualEPS<<endl;}
            
            // Create  stock objects
            Stock * stock= new Stock(ticker,datezero,datetimezero,actualEPS,estimateEPS);// surprises is calculated in constructor
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
    cout<<"Lower threshold of surprise:" <<Thres1<<endl;
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
    cout<<"There are "<<Beat.getStockPtr().size()<<" stocks in  the Beat group."<<endl;
    cout<<"There are "<<Meet.getStockPtr().size()<<" stocks in  the Meet group."<<endl;
    cout<<"There are "<<Miss.getStockPtr().size()<<" stocks in  the Miss group."<<endl;
    
    
    // Min and Max date of all stocks, used for index to search adjClose
    string minDate = "2019-0"+to_string(minDatenum).substr(0,1)+"-"+to_string(minDatenum).substr(2,2);
    string maxDate = "2019-0"+to_string(maxDatenum).substr(0,1)+"-"+to_string(maxDatenum).substr(2,2);
    
    cout<< "The earliest release date is "<< minDate<<endl;
    cout<< "The latest release date is "<< maxDate<<endl;

    
    // Create SPY index and add it to 3 groups
     Group::indexPtr = new Index("SPY",minDate,maxDate);
    
    
    // Bootstrap stocks
    // For each stock, seach or enough prices, find date-30, date 30
    // Yahoo get prices for sampled stocks
    // Yahoo get prices for index
    // Calculations
    // Populate the stock maps and AAR/CAAR matrix
    // Plot
    // Menu
    // Delete stocks and spy
        }
