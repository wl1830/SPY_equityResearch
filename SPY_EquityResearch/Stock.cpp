//
//  Stock.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/2/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#include "Stock.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include "curl/curl.h"
#include "VectorCalculation.hpp"
using namespace std;


void Equity::CalReturn(){
    map<string,double> ::iterator itr;
    
    for (itr = priceMap.begin(); itr!= prev(priceMap.end());itr++){
        double stockr = (next(itr) ->second - itr->second)/(itr->second);
        returnMap[next(itr) ->first] = stockr;//SR:stock return
    }
//
    if(returnMap.size()!=60){
        
        cout<<"The "<<ticker<<" has "<<returnMap.size()<<" returns."<<endl;
        
    }
    
//    if(returnMap.size()!=60){cout<<"Warning!  There are "<<returnMap.size()<<" days"<<endl;}

}


vector<double> Index::GetReturnVec(string date1,string date2)const{
    vector<double> returns;
    map<string,double> ::const_iterator itr;
    
    
    for(itr=returnMap.find(date1);itr!=next(returnMap.find(date2));itr++){
        returns.push_back(itr->second);
    }
    
    if(returns.size()!=60){
        cout<<"In the Index::GetReturnVec(string date1,string date2)"<<" Two dates are "<<date1<<" "<<date2<<endl;
        cout<<"There are "<<returns.size()<<" returns for index"<<endl;}
//    else{cout<<"60 days for index";
//
//    }
    return returns;
}

vector<double> Stock::GetReturnVec(){
    
    vector<double> returns;
    map<string,double> ::iterator itr;
    for(itr=returnMap.begin();itr!=returnMap.end();itr++){
        returns.push_back(itr->second);
    }
    if(returns.size()!=60){cout<<"There are"<<returns.size()<<" returns"<<endl;}
    return returns;
}

size_t write_data(void *ptr, int size, int nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

struct MemoryStruct {
    char *memory;
    size_t size;
};

void *myrealloc(void *ptr, size_t size)
{
    /* There might be a realloc() out there that doesn't like reallocing
    NULL pointers, so we take care of it here */
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}


size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;

    mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

string getTimeinSeconds(string Time)
{
    std::tm t = {0};
    std::istringstream ssTime(Time);
    char time[100];
    memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT"))
    {
//        cout << std::put_time(&t, "%c %Z") << "\n"   << std::mktime(&t) << "\n";
               
        sprintf (time, "%ld", mktime(&t));
        return string(time);
    }
    else
    {
        cout << "Parse failed\n";
        return "";
    }
}

void Stock::SearchPrice(){
    try{
        string startTime = getTimeinSeconds(date_minus_30);
    
        string endTime = getTimeinSeconds(date_30);
        vector<string> symbolList;
        symbolList.push_back(string(getTicker()));
        vector<string>::iterator itr = symbolList.begin();
    
        struct MemoryStruct data;
        data.memory = NULL;
        data.size = 0;
    
        //file pointer to create file that store the data
        FILE *fp;
    
        //name of files
        const char outfilename[FILENAME_MAX] = "Output.txt";
//        const char resultfilename[FILENAME_MAX] = "Results.txt";
    
        /* declaration of an object CURL */
        CURL *handle;
    
        /* result of the whole process */
        CURLcode result;
    
        /* the first functions */
        /* set up the program environment that libcurl needs */
        curl_global_init(CURL_GLOBAL_ALL);
        
        /* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
        handle = curl_easy_init();
    
        /* if everything's all right with the easy handle... */
        if (handle)
        {
            while (true)
            {  string sCookies,sCrumb;
//                string sCookies = "18h3qj5eut97p&b=3&s=b7"
//                string sCrumb = "Y29P3C2VYL";
                if (sCookies.length() == 0 || sCrumb.length() == 0)
                {
                    fp = fopen(outfilename, "w");
                    //curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
                    curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
                    
                    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
                    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
                    curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
                    curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
    
                    curl_easy_setopt(handle, CURLOPT_ENCODING, "");
                    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
                    curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
                    result = curl_easy_perform(handle);
                    fclose(fp);
    
                    /* Check for errors */
                    if (result != CURLE_OK)
                    {
                        /* if errors have occurred, tell us what is wrong with 'result'*/
                        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                 
                    }
    
                    
                    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
                    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
    
                    /* perform, then store the expected code in 'result'*/
                    result = curl_easy_perform(handle);
    
                    /* Check for errors */
                    if (result != CURLE_OK)
                    {
                        /* if errors have occured, tell us what is wrong with 'result'*/
                        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                     
                    }
    
                    char cKey[] = "CrumbStore\":{\"crumb\":\"";
                    char *ptr1 = strstr(data.memory, cKey);
                    char *ptr2 = ptr1 + strlen(cKey);
                    char *ptr3 = strstr(ptr2, "\"}");
                    if ( ptr3 != NULL )
                        *ptr3 = NULL;
    
                    sCrumb = ptr2;
    
                    fp = fopen("cookies.txt", "r");
                    char cCookies[100];
                    if (fp) {
                        while (fscanf(fp, "%s", cCookies) != EOF);
                        fclose(fp);
                    }
                    else
                        cerr << "cookies.txt does not exists" << endl;
    
                    sCookies = cCookies;
                    free(data.memory);
                    data.memory = NULL;
                    data.size= 0;
                }
                free(data.memory);
                data.memory = NULL;
                data.size= 0;
                if (itr == symbolList.end())
                    break;
                string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
                string symbol = *itr;
                string urlB = "?period1=";
                string urlC = "&period2=";
                string urlD = "&interval=1d&events=history&crumb=";
                string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
                const char * cURL = url.c_str();
                const char * cookies = sCookies.c_str();
                curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
                curl_easy_setopt(handle, CURLOPT_URL, cURL);
//                fp = fopen(resultfilename, "ab");
                /*curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
                
                fclose(fp);重复了 应该写成write——data2
                */
                /* Check for errors */
                result = curl_easy_perform(handle);
                if (result != CURLE_OK)
                {
                    /* if errors have occurred, tell us what is wrong with 'result'*/
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
              
                }
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
                result = curl_easy_perform(handle);
    
                /* Check for errors */
                if (result != CURLE_OK)
                {
                    /* if errors have occurred, tell us what is wrong with 'result'*/
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
              
                }
                stringstream sData;
                sData.str(data.memory);
                string line;
                getline(sData, line);
//                cout << line << endl;
                string date,tempstring,adjClosestr;
                double  adjClose;
                while ( getline(sData, line) ){
//                    cout << line << endl;
                //Date,Open,High,Low,Close,Adj Close,Volume
                stringstream ss(line);
                getline(ss,date,',');
                getline(ss,tempstring,','); //open
                getline(ss,tempstring,',');//high
                getline(ss,tempstring,',');//low
                getline(ss,tempstring,',');//close
                getline(ss,adjClosestr,',');//adjclose
                adjClose = stold(adjClosestr); //string to long double
                getline(ss,tempstring,',');//volume
                
                priceMap[date] = adjClose;
//                    cout<<priceMap[date];
                    
                
                    
                
                }
                itr++;
            }
            free(data.memory);
            data.size= 0;
        }
        else
        {
                fprintf(stderr, "Curl init failed!\n");
        
        }
    
        /* cleanup since you've used curl_easy_init */
        curl_easy_cleanup(handle);
    
        /* this function releases resources acquired by curl_global_init() */
        curl_global_cleanup();
        
    }
    catch(...){}
    
}

void Index::SearchPrice(){
    string startTime = getTimeinSeconds(startdate);
        string endTime = getTimeinSeconds(enddate);
        vector<string> symbolList;
        symbolList.push_back(string(getTicker()));
        vector<string>::iterator itr = symbolList.begin();
    
        struct MemoryStruct data;
        data.memory = NULL;
        data.size = 0;
    
        //file pointer to create file that store the data
        FILE *fp;
    
        //name of files
        const char outfilename[FILENAME_MAX] = "Output.txt";
//        const char resultfilename[FILENAME_MAX] = "Results.txt";
    
        /* declaration of an object CURL */
        CURL *handle;
    
        /* result of the whole process */
        CURLcode result;
    
        /* the first functions */
        /* set up the program environment that libcurl needs */
        curl_global_init(CURL_GLOBAL_ALL);
        
        /* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
        handle = curl_easy_init();
    
        /* if everything's all right with the easy handle... */
        if (handle)
        {
            while (true)
            {
                string sCookies, sCrumb;
                if (sCookies.length() == 0 || sCrumb.length() == 0)
                {
                    fp = fopen(outfilename, "w");
                    //curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
                    curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
                    
                    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
                    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
                    curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
                    curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
    
                    curl_easy_setopt(handle, CURLOPT_ENCODING, "");
                    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
                    curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
                    result = curl_easy_perform(handle);
                    fclose(fp);
    
                    /* Check for errors */
                       if (result != CURLE_OK)
                       {
                           /* if errors have occurred, tell us what is wrong with 'result'*/
                           fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                    
                       }
    
                    
                    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
                    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
    
                    /* perform, then store the expected code in 'result'*/
                    result = curl_easy_perform(handle);
    
                    /* Check for errors */
                    if (result != CURLE_OK)
                    {
                        /* if errors have occured, tell us what is wrong with 'result'*/
                        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                     
                    }
    
                    char cKey[] = "CrumbStore\":{\"crumb\":\"";
                    char *ptr1 = strstr(data.memory, cKey);
                    char *ptr2 = ptr1 + strlen(cKey);
                    char *ptr3 = strstr(ptr2, "\"}");
                    if ( ptr3 != NULL )
                        *ptr3 = NULL;
    
                    sCrumb = ptr2;
    
                    fp = fopen("cookies.txt", "r");
                    char cCookies[100];
                    if (fp) {
                        while (fscanf(fp, "%s", cCookies) != EOF);
                        fclose(fp);
                    }
                    else
                        cerr << "cookies.txt does not exists" << endl;
    
                    sCookies = cCookies;
                    free(data.memory);
                    data.memory = NULL;
                    data.size= 0;
                }
                
                if (itr == symbolList.end())
                    break;
    
                string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
                string symbol = *itr;
                string urlB = "?period1=";
                string urlC = "&period2=";
                string urlD = "&interval=1d&events=history&crumb=";
                string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
                const char * cURL = url.c_str();
                const char * cookies = sCookies.c_str();
                curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
                curl_easy_setopt(handle, CURLOPT_URL, cURL);
                
                /*curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
                result = curl_easy_perform(handle);
                fclose(fp);重复了 应该写成write——data2
                */
                /* Check for errors */
//                if (result != CURLE_OK)
//                {
//                    /* if errors have occurred, tell us what is wrong with 'result'*/
//                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
//
//                }
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
                result = curl_easy_perform(handle);
    if (result != CURLE_OK)
      {
          /* if errors have occurred, tell us what is wrong with 'result'*/
          fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    
      }
                stringstream sData;
                sData.str(data.memory);
                string line;
                getline(sData, line);
//                cout << line << endl;
                string date,tempstring,adjClosestr;
                double  adjClose;
                while ( getline(sData, line) ){
//                    cout << line << endl;//
                //Date,Open,High,Low,Close,Adj Close,Volume
                stringstream ss(line);
                getline(ss,date,',');
                getline(ss,tempstring,','); //open
                getline(ss,tempstring,',');//high
                getline(ss,tempstring,',');//low
                getline(ss,tempstring,',');//close
                getline(ss,adjClosestr,',');//adjclose
                adjClose = stold(adjClosestr); //string to long double
                getline(ss,tempstring,',');//volume
                
                priceMap[date] = adjClose;
                }
                itr++;
            }
            free(data.memory);
            data.size= 0;
        }
        else
        {
                fprintf(stderr, "Curl init failed!\n");
        
        }
    
        /* cleanup since you've used curl_easy_init */
        curl_easy_cleanup(handle);
    
        /* this function releases resources acquired by curl_global_init() */
        curl_global_cleanup();
        
    
}



void  searchStocks(map<string,Stock*> pool)
{
//    string startTime = getTimeinSeconds("2019-04-01T16:00:00");
//    string endTime = getTimeinSeconds("2019-04-21T16:00:00");
//    vector<string> symbolList;
//    symbolList.push_back(string("AMZN"));
//    symbolList.push_back(string("MSFT"));
//    symbolList.push_back(string("TWTR"));
    map<string,Stock*>::iterator itr = pool.begin();

    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;

    //file pointer to create file that store the data
    FILE *fp;

    //name of files
    const char outfilename[FILENAME_MAX] = "Output.txt";
//    const char resultfilename[FILENAME_MAX] = "Results.txt";

    /* declaration of an object CURL */
    CURL *handle;

    /* result of the whole process */
    CURLcode result;

    /* the first functions */
    /* set up the program environment that libcurl needs */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
    handle = curl_easy_init();

    /* if everything's all right with the easy handle... */
    
    if (handle)
    {string sCookies, sCrumb;
        
        while (true)
        {
            
            if (sCookies.length() == 0 || sCrumb.length() == 0)
            {
                fp = fopen(outfilename, "w");
                //curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
                curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
                
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
                curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
                curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

                curl_easy_setopt(handle, CURLOPT_ENCODING, "");
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
                result = curl_easy_perform(handle);
                fclose(fp);

//                /* Check for errors */
//                if (result != CURLE_OK)
//                {
//                    /* if errors have occurred, tell us what is wrong with 'result'*/
//                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
////                    pool.erase(itr->first);
////                    return 1;
//                }

                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);

                /* perform, then store the expected code in 'result'*/
                result = curl_easy_perform(handle);

                /* Check for errors */
//                if (result != CURLE_OK)
//                {
//                    /* if errors have occured, tell us what is wrong with 'result'*/
//                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
////                    pool.erase(itr->first);
//                }

                char cKey[] = "CrumbStore\":{\"crumb\":\"";
                char *ptr1 = strstr(data.memory, cKey);
                char *ptr2 = ptr1 + strlen(cKey);
                char *ptr3 = strstr(ptr2, "\"}");
                if ( ptr3 != NULL )
                    *ptr3 = NULL;

                sCrumb = ptr2;

                fp = fopen("cookies.txt", "r");
                char cCookies[100];
                if (fp) {
                    while (fscanf(fp, "%s", cCookies) != EOF);
                    fclose(fp);
                }
                else
                    cerr << "cookies.txt does not exists" << endl;

                sCookies = cCookies;
                free(data.memory);
                data.memory = NULL;
                data.size= 0;
            }
            free(data.memory);
            data.memory = NULL;
            data.size= 0;
            if (itr == pool.end())
                break;
            
            string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
            string symbol = itr->first;
//            cout<<itr->first<<endl;
            string startTime = getTimeinSeconds(itr->second->getDate_minus_30());
//            cout<<itr->second->getDate_minus_30()<<endl;
            string endTime = getTimeinSeconds(itr->second->getDetdate_30());
//            cout<<symbol<<" "<<"start time: "<<startTime<<" endtime "<<endTime<<endl;
            string urlB = "?period1=";
            string urlC = "&period2=";
            string urlD = "&interval=1d&events=history&crumb=";
            string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
            const char * cURL = url.c_str();
            const char * cookies = sCookies.c_str();
            curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
            curl_easy_setopt(handle, CURLOPT_URL, cURL);
//            fp = fopen(resultfilename, "ab");
//            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
//            curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
//            result = curl_easy_perform(handle);
//            fclose(fp);
//
//            /* Check for errors */
//            if (result != CURLE_OK)
//            {
//                /* if errors have occurred, tell us what is wrong with 'result'*/
//                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
//               pool.erase(itr->first);
//            }
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            result = curl_easy_perform(handle);

            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
//                pool.erase(itr->first);
            }
                stringstream sData;
                sData.str(data.memory);
                string line;
                getline(sData, line);
//                cout << line << endl;
                string date,tempstring,adjClosestr;
                double  adjClose;
            
               map<string,double> pricemap;
//            cout<<line<<endl;
            while ( getline(sData, line) ){
                
//                    cout << line << endl;
                //Date,Open,High,Low,Close,Adj Close,Volume
//                cout<<line<<endl;
                stringstream ss(line);
                getline(ss,date,',');
                getline(ss,tempstring,','); //open
                getline(ss,tempstring,',');//high
                getline(ss,tempstring,',');//low
                getline(ss,tempstring,',');//close
                getline(ss,adjClosestr,',');//adjclose
//                cout<<"adj str is "<<adjClosestr<<endl;
                adjClose = stold(adjClosestr); //string to long double
                getline(ss,tempstring,',');//volume
                
                pricemap[date]= adjClose;
//                    cout<<date<<" "<<adjClose;
            }
            
            
//            cout<<"size of pricemap: "<<pricemap.size()<<endl;
             itr->second->setPriceMap(pricemap);
            cout<<itr->first<<" searched\n";
//            cout<<"After set,size of price map of stock: "<<
//            itr->second->GetPriceMap().size();
            itr++;
//            cout<<"start new stock\n";
        }
        free(data.memory);
        data.size= 0;
    } //end of(if handle)
    else
    {
            fprintf(stderr, "Curl init failed!\n");
            
    }

    /* cleanup since you've used curl_easy_init */
    curl_easy_cleanup(handle);

    /* this function releases resources acquired by curl_global_init() */
    curl_global_cleanup();
    cout<<"Search End\n";
}

