//
//  gnuplot.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/7/19.
//  Copyright © 2019 NYU. All rights reserved.
//
#include <iostream>
#include "gnuplot.hpp"


void PlotVectors(vector<double> v1,vector<double> v2,vector<double> v3,
                 int nIntervals ,double stepSize,double x0,
                 const char * FileName1,const char * FileName2 ,const char * FileName3 ){
    FILE *gnuplotPipe,*tempDataFile;
    double x,y, x2, y2,x3,y3;
    int i;
    gnuplotPipe = popen("/opt/local/bin/gnuplot","w");

    int dataSize = (int)v1.size();
    if (gnuplotPipe) {
         fprintf(gnuplotPipe," set term wxt;set title \"Average CAAR\";set xlabel \"day\"; ");
         fprintf(gnuplotPipe,"plot \"%s\" with lines lw 6,\"%s\" with lines lw 6, \"%s\" with lines lw 6 \n",
                 FileName1, FileName2,FileName3);
         
         fflush(gnuplotPipe);
         tempDataFile = fopen(FileName1,"w");
         for (i=0; i < dataSize; i++) {
             x = x0+i*(stepSize);
             y = v1[i];
             fprintf(tempDataFile,"%lf %lf\n",x,y);
         }
         fclose(tempDataFile);
         
         tempDataFile = fopen(FileName2,"w");
         for (i=0; i < dataSize; i++) {
             x2 = x0+i*(stepSize);
             y2 = v2[i];
             fprintf(tempDataFile,"%lf %lf\n",x2,y2);
         }
         fclose(tempDataFile);
         
         tempDataFile = fopen(FileName3,"w");
         for (i=0; i < dataSize; i++) {
             x3 = x0+i*(stepSize);
             y3 = v3[i];
             fprintf(tempDataFile,"%lf %lf\n",x3,y3);
         }
         fclose(tempDataFile);
         
         printf("press enter to continue...");
         getchar();
        fprintf(gnuplotPipe,"exit \n");
        
     } else {
         printf("gnuplot not found...");
     }
    
}
