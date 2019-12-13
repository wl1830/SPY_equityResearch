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
using namespace std::chrono;
//initialize the static variable indexitr
Index * Group::indexPtr = nullptr;

void PrintVecotr(vector<double> v){
    for(int i=0;i<v.size();i++){
        cout<<v[i]<<"\t";
    }
    cout<<endl;
}
void PrintMap(map<string, double> m){
    
    for(map<string, double>::iterator i=m.begin();i!=m.end();i++){
        cout<<i->first<<"\t"<<i->second<<endl;
    }
    cout<<endl;
}

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
        cout<< "\n\n\n\nRead in the following information: \n";
        cout << line << endl;
        double minDatenum =20191231;
        double maxDatenum = 20190101;

        map<string,Stock*> poolStocks;// Ticker: Stock* : all the stocks
        map<string,Equity*> poolAll;  // Ticker: Equity*: Index and stock all the stocks
        vector<vector<vector<double>>> Results;
        vector<double> Surprises;
        
        while (getline(infile,line)){
            //line 'Ticker', 'Actual', 'Estimate', 'Date','date_minus_30', 'date_30'
            stringstream ss(line);
            getline(ss,ticker,',');
            getline(ss,actualEPSstr,',');
            getline(ss,estimateEPSstr,',');
            getline(ss,datezero,',');
            getline(ss,date_minus_30,',');
            getline(ss,date_30,',');
            actualEPS = stod(actualEPSstr);
            estimateEPS = stod(estimateEPSstr);
            double sdatenum,edatenum;
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
            Stock * stock= new Stock(ticker,datezero,date_minus_30,date_30,actualEPS,estimateEPS);// surprises is calculated in constructor
            Surprises.push_back(stock->getSurprise());
            poolStocks[ticker] =stock;
            poolAll[ticker] =stock;
        }

    infile.close();
    
    // Find the 2 threshold of Surprise to group stocks
    sort(Surprises.begin(),Surprises.end());
    auto const Q1 = Surprises.size() / 3;
    auto const Q2 = 2 * Q1;
    double Thres1 = Surprises[Q1] ;
    double Thres2 = Surprises[Q2] ;
    cout<<"\nData from bloomberg passed in.\n";
    cout<<"\nLower threshold of surprise:" <<Thres1<<endl;
    cout<<"Uper threshold of surprise:" <<Thres2<<endl;

    // Create 3 Groups and fill with corresponding stock pointers
    Group Beat("Beat");
    Group Meet("Meet");
    Group Miss("Miss");
    
    // Store the stock pointers into the Group objects according to surprise value
    map<string,Stock*>::iterator Stocksitr;
    for(Stocksitr=poolStocks.begin();Stocksitr!=poolStocks.end();Stocksitr++)
    {
        if((Stocksitr->second)->getSurprise()>Thres2){
            Beat.addStock(Stocksitr->second);
        }
        else if ((Stocksitr->second)->getSurprise()>Thres1){
            Meet.addStock(Stocksitr->second);
        }
        else{ Miss.addStock(Stocksitr->second);
        }
        
    }
    cout<<"\nThere are "<<Beat.getStockPtr().size()<<" stocks in the Beat group."<<endl;
    cout<<"There are "<<Meet.getStockPtr().size()<<" stocks in the Meet group."<<endl;
    cout<<"There are "<<Miss.getStockPtr().size()<<" stocks in the Miss group."<<endl;
    
    
    // Min and Max date of all stocks, used for index to search adjClose
    string minDate = to_string(minDatenum).substr(0,4)+"-"+to_string(minDatenum).substr(4,2)+"-"+to_string(minDatenum).substr(6,2);
    string maxDate = to_string(maxDatenum).substr(0,4)+"-"+to_string(maxDatenum).substr(4,2)+"-"+to_string(maxDatenum).substr(6,2);
 
    
    cout<< "\nThe earliest day -30 is "<< minDate<<endl;
    cout<< "The latest day 30 is "<< maxDate<<endl;
    
    // Create SPY index and add it to 3 groups
    Group::indexPtr = new Index("SPY",minDate,maxDate);
    poolAll["SPY"] =Group::indexPtr;
    
    
// Menu

        cout << endl << endl << "--------------------------------menu--------------------------------" << endl << endl
        << "Please press 1 to retrieve historical all stocks."  << endl
        << "Please press 2 to pull information for one stock or index" << endl
        << "Please (after 1) press 3 to show AAR, CAAR, AARstd or CAARstd for a group."  << endl
        << "Please (after 1) press 4 to plot average CAAR for all groups." << endl
        << "Please press 0 to exit the program." << endl << endl;
        
        while (true)
        {
            char c;
            cout<<"Select an option:";
            c = getchar();
            if (c == '1')
            {
                cout<<"\nStart to search price for all stocks....\n\n";
                // Retrive historical prices for index and all stocks, store in the PriceMap of each Equity object
                auto start = high_resolution_clock::now();
                searchEquities(poolAll);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<seconds>(stop - start);
                cout <<"Time used for searching historical prices: " << duration.count() <<" s.\n"<< endl;
                // Calculate daily return for index and all stocks, store in the returnMap of each Equity object
                for(map<string,Equity*>::iterator itr=poolAll.begin();itr!=poolAll.end();itr++){
                    (itr->second)->CalReturn();
                }
                // Bootstrap 30 times and aggregate result
                start = high_resolution_clock::now();
                Beat.Bootstap30_Calculate_All();
                Meet.Bootstap30_Calculate_All();
                Miss.Bootstap30_Calculate_All();
                stop = high_resolution_clock::now();
                cout<<"\nCalculation for three groups done!"<<endl;
                auto duration2 = duration_cast<microseconds>(stop - start);
                cout <<"\nTime used for bootstrap and calculation: " << duration2.count()<<" * 10^-6 s.\n"<< endl;
                // Pop calculations into a matrix(3*4) of vecotors(60*1)
                Results.push_back({Beat.getAARavg(),Beat.getAARstd(),Beat.getCAARavg(),Beat.getCAARstd()}); // Beat results
                Results.push_back({Meet.getAARavg(),Meet.getAARstd(),Meet.getCAARavg(),Meet.getCAARstd()}); // Meet results
                Results.push_back({Miss.getAARavg(),Miss.getAARstd(),Miss.getCAARavg(),Miss.getCAARstd()}); // Miss results
                
            }
              else if (c == '2')
              {
                string ticker;
                cout<<"Please enter the stock symbol:"<<endl;
                cin>>ticker;
                //Not a stock
                    if ( poolStocks.find(ticker) == poolStocks.end() ){
                        // Index
                        if(ticker=="SPY"){
                            cout<<endl<<"Date\t\tAdjClose:"<<endl;
                            PrintMap(Group::indexPtr->GetPriceMap());
                        }else{cout<< endl<<"Stock not included."<<endl;}
                            }
                //Normal stock
                    else {
                        Stock* stkptr = poolStocks[ticker];
                        cout<<"Ticker:"<<stkptr->getTicker()<<endl;
                        cout<<"Start day: "<<stkptr->getDate_minus_30()<<endl;
                        cout<<"End day: "<<stkptr->getDetdate_30()<<endl;
                        cout<<"Announcement date: "<<stkptr->getDatezero()<<endl;
                        cout<<"Stock Estimated EPS: "<<stkptr->getEstimateEPS()<<endl;
                        cout<<"Stock actual EPS: "<<stkptr->getActualEPS()<<endl;
                        cout<<"Surprise:"<<stkptr->getSurprise()<<endl;
                        cout<<endl<<"Date\tAdjClose:"<<endl;
                        PrintMap(stkptr->GetPriceMap());
                        }
            }
           else if(c == '3' )
                {

                    if(Beat.getCAARavg().size()==0){
                        //Check if the caculation done.
                        cout<<"Please retrive historical price by pressing 1 first.\n";
                    }
                else{
                    string dataType;
                    string groupName;
                    cout<<"Please enter the metric type (AAR, CAAR, AARstd,CAARstd):"<<endl;
                    cin>> dataType;
                    // Make sure metrics type is correct
                    if (dataType =="AAR" || dataType == "CAAR"|| dataType =="AARstd" || dataType == "CAARstd") {
                        cout<<"Please enter the group name (Beat, Meet, Miss):"<<endl;
                        cin>>groupName;
                        // Make sure group name is correct
                        if (groupName == "Beat" || groupName == "Meet" || groupName == "Miss") {
                            int metricType,group;
                        
                            if(dataType =="AAR") metricType = 0;
                            else if(dataType !="AARstd") metricType = 1;
                            else if(dataType !="CAAR") metricType = 2;
                            else  metricType = 3;
                        
                            if (groupName == "Beat") group = 0;
                            else if (groupName == "Meet") group = 1;
                            else group = 2;
                        
                            cout<<endl<<"The "<<dataType<<" of group "<<groupName<<" is:"<<endl;
                            
                                switch (group) {
                                    case 0: //Beat
                                        switch (metricType) {
                                            case 0: //AAR
                                                PrintVecotr(Results[0][0]);
                                                break;
                                            case 1:  //AARstd
                                                PrintVecotr(Results[0][1]);
                                                break;
                                            case 2: //CAAR
                                                PrintVecotr(Results[0][2]);
                                                break;
                                            case 3: //CAARstd
                                                PrintVecotr(Results[0][3]);
                                                break;
                                            default:
                                                break;
                                        }
                                        break;
                                    case 1: //Meet
                                        switch (metricType) {
                                            case 0: //AAR
                                                PrintVecotr(Results[1][0]);
                                                break;
                                            case 1://AARstd
                                                PrintVecotr(Results[1][1]);
                                                break;
                                            case 2://CAAR
                                                PrintVecotr(Results[1][2]);
                                                break;
                                            case 3://CAARstd
                                                PrintVecotr(Results[1][3]);
                                                break;
                                            default:
                                                break;
                                        }
                                        break;
                                    case 2: //Miss
                                        switch (metricType) {
                                            case 0:  //AAR
                                                PrintVecotr(Results[2][0]);
                                                break;
                                            case 1://AARstd
                                                PrintVecotr(Results[2][1]);
                                                break;
                                            case 2://CAAR
                                                PrintVecotr(Results[2][2]);
                                                break;
                                            case 3://CAARstd
                                                PrintVecotr(Results[2][3]);
                                                break;
                                            default:
                                                break;
                                        }
                                        break;
                                        
                                    default:
                                        break;
                                }
                            
                        }else{cout<<endl<<"Wrong group name."<<endl;}
                        
                    }
                else{cout<<endl<<"Wrong type."<<endl;}
                }
            
                }
         else if (c == '4') {
                        //
                        if(Beat.getCAARavg().size()==0)
                        {   cout<<"\nPlease retrive historical price by pressing 1 first.\n";
                            }
                        else{
                            // Avg CAAR
                            vector<double> v1 = Results[0][2]; //Beat
                            vector<double> v2 = Results[1][2]; //Meet
                            vector<double> v3 = Results[2][2]; //Miss
                            // Use gnuPlot
                            PlotVectors(v1, v2, v3);
                            
                            }
                 }
        else if(c == '0') {break;}
                    
        }
        
    
        // Free the memory of stocks and SPY index
        for(Stocksitr=poolStocks.begin();Stocksitr!=poolStocks.end();Stocksitr++){
            delete Stocksitr->second;
            Stocksitr->second = NULL;
            }
        poolStocks.clear();
        delete Group::indexPtr;
        Group::indexPtr = NULL;
        poolAll.clear();
    }

