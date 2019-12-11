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
        cout<< "Read the following information: ";
        cout << line << endl;
        double minDatenum =20191231;
        double maxDatenum = 20190101;
        vector<Stock*> Stocks;
        map<string,Stock*> pool;
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
            pool[ticker] =stock;
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
    
    // store the stock pointers into the Group objects according to surprise value
    vector<Stock*>::iterator Stocksitr;
    for(Stocksitr=Stocks.begin();Stocksitr!=Stocks.end();Stocksitr++)
    {
//        (*Stocksitr)->SearchPrice();
//        (*Stocksitr)->CalReturn();
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
     
    
// Menu

        cout << endl << endl << "--------------------------------menu--------------------------------" << endl << endl
        << "Please press 1 to retrieve historical all stocks."  << endl
        <<"Please press 2 to pull information for one stock  from one group" << endl
       << "Please press 3(after 1)to show AAR,CAAR,AARstd or CAARstd for one group."  << endl
      << "Please press 4(after 1) to show the Excel graph with CAAR for all 3 groups." << endl
        << "Please press 0 to exit the program." << endl << endl;
        
        while (true)
        {
            char c;
            cout<<"select an option:";
            c = getchar();
            if (c == '1')
            {

                cout<<"\nStart to search price for sampled stocks and calculate. \nPlease hold...\n";
                Group::IndexSearch_CalReturn();
                Beat.Bootstap_Calculate_All();
                Meet.Bootstap_Calculate_All();
                Miss.Bootstap_Calculate_All();

                    cout<<"\nCalculation for three groups done!\n"<<endl;

                    }
//             if (c == '1')
              else if (c == '2')
            {
                string ticker;
                cout<<"Please enter the stock symbol:"<<endl;
                cin>>ticker;
            
//                   // Not a stock
                    if ( pool.find(ticker) == pool.end() ){
                        //check if it is a index
                        if(ticker=="SPY"){
                            cout<<endl<<"Date\tAdjClose:"<<endl;
                            PrintMap(Group::indexPtr->GetPriceMap());
                        }
                        else{cout<< endl<<"Stock not included."<<endl;}
                        }
                    else { //Normal stock
                        Stock* stkptr = pool[ticker];
                        cout<<"Ticker:"<<stkptr->getTicker()<<endl;
                        cout<<"Start day:"<<stkptr->getDate_minus_30()<<endl;
                        cout<<"End day:"<<stkptr->getDetdate_30()<<endl;
                        cout<<"Announcement date:"<<stkptr->getDatezero()<<endl;
                        cout<<"Stock Estimated EPS"<<stkptr->getEstimateEPS()<<endl;
                        cout<<"Stock actual EPS:"<<stkptr->getActualEPS()<<endl;
                        cout<<"Surprise:"<<stkptr->getSurprise()<<endl;
    //                    map<string,double>price = priceMap(ticker);
                        cout<<endl<<"Date\tAdjClose:"<<endl;
    //                    for (map<string,double>::SearchPrice()) cout<<itr->first<<" "<<itr->second <<endl;
                        // Search and print prices
                        stkptr->SearchPrice();
                        PrintMap(stkptr->GetPriceMap());
                        }
                
                
            }
                           else if(c == '3' )
    {

        if(Beat.getACAR().size()==0){
            //Check if the caculation done.
            cout<<"Please retrive historical price by pressing 1 first.\n";
        }
        else{
                      string dataType;
                                string groupName;
                                cout<<"Please enter the metric type (AAR, CAAR, AARstd,CAARstd):"<<endl;
                                cin>> dataType;
                
            //                    if (dataType !="AAR" && dataType != "CAAR"&& dataType !="AARstd" && dataType != "CAARstd") {}
                                if (dataType =="AAR" || dataType == "CAAR"|| dataType =="AARstd" || dataType == "CAARstd") {
                                    cout<<"Please enter the group name (Beat, Meet, Miss):"<<endl;
                                    cin>>groupName;
                                    if (groupName == "Beat" || groupName == "Meet" || groupName == "Miss") {
                                        int metricType;
                                            if(dataType =="AAR"){ metricType = 0;}
                                            else if(dataType !="CAAR"){ metricType = 1;}
                                            else if(dataType !="AARstd"){ metricType = 2;}
                                            else { metricType = 3;}
                                            int group;
                                            if (groupName == "Beat") group = 0;
                                            else if (groupName == "Meet") group = 1;
                                            else group = 2;
                                                

                                        
                                            cout<<endl<<"The "<<dataType<<" of group "<<groupName<<" is:"<<endl;
                                            switch (group) {
                                                case 0: //Beat
                                                    switch (metricType) {
                                                        case 0: //AAR
                                                            PrintVecotr(Beat.getAAR());
                                                            break;
                                                        case 1:  //CAAR
                                                        PrintVecotr(Beat.getACAR());
                                                        break;
                                                        case 2: //AARstd
                                                        PrintVecotr(Beat.getstdAR());
                                                        break;
                                                        case 3: //CAARstd
                                                        PrintVecotr(Beat.getstdCAR());
                                                        break;
                                                        default:
                                                            break;
                                                    }
                                                    break;
                                                    case 1: //Meet
                                                        switch (metricType) {
                                                            case 0: //AAR
                                                                PrintVecotr(Meet.getAAR());
                                                                break;
                                                            case 1://CAAR
                                                            PrintVecotr(Meet.getACAR());
                                                            break;
                                                            case 2://AARstd
                                                            PrintVecotr(Meet.getstdAR());
                                                            break;
                                                            case 3://CAARstd
                                                            PrintVecotr(Meet.getstdCAR());
                                                            break;
                                                            default:
                                                                break;
                                                        }
                                                        break;
                                                case 2: //Miss
                                                    switch (metricType) {
                                                        case 0:  //AAR
                                                            PrintVecotr(Miss.getAAR());
                                                            break;
                                                        case 1://CAAR
                                                        PrintVecotr(Miss.getACAR());
                                                        break;
                                                        case 2://AARstd
                                                        PrintVecotr(Miss.getstdAR());
                                                        break;
                                                        case 3://CAARstd
                                                        PrintVecotr(Miss.getstdCAR());
                                                        break;
                                                        default:
                                                            break;
                                                    }
                                                    break;
                                                    
                                                default:
                                                    break;
                                            }
                                        
                                    }else{cout<<endl<<"Wrong Group."<<endl;}
                                    
                                }
                                            
                        //                    if (groupName != "Beat" && groupName != "Meet" && groupName != "Miss") {cout<<endl<<"Wrong name."<<endl;valid_and_group = 0;}
                                            else{cout<<endl<<"Wrong Type."<<endl;}
          

        }
        

        
    }
 else if (c == '4') {
                //gnuplot:
                if(Beat.getACAR().size()==0){
                                   cout<<"Please retrive historical price by pressing 1 first.\n";
                               }
                else{
                vector<double> v1 = Beat.getACAR();
                vector<double> v2 = Meet.getACAR();
                vector<double> v3 = Miss.getACAR();
                                   Plot(v1, v2,v3);
                                   
                               }

            }
//
            else if(c == '0') {
                
                break;}
            
            
    }
            


    

    
    for(Stock* stk_ptr:Stocks){
        delete stk_ptr;
    }
    
   Stocks.clear();
   delete Group::indexPtr;
   Group::indexPtr = NULL;

    
    // Delete stocks and spy
    
    
    
        }

