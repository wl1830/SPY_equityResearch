//
//  gnuplot.cpp
//  SPY_EquityResearch
//
//  Created by 刘唯婷婷 on 12/7/19.
//  Copyright © 2019 NYU. All rights reserved.
//

#include "gnuplot.hpp"
void plotResults(double* xData, double* yData, double* yData2, double* yData3, int dataSize)
{
    FILE *gnuplotPipe,*tempDataFile;
    const char *tempDataFileName1 = "Beat";
    const char *tempDataFileName2 = "Miss";
    const char *tempDataFileName3 = "Meet";
    double x,y, x2, y2,x3,y3;
    int i;
    gnuplotPipe = popen("/opt/local/bin/gnuplot","w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe,"set term wxt;plot \"%s\" with lines,\"%s\" with lines, \"%s\" with lines \n",tempDataFileName1, tempDataFileName2,tempDataFileName3);
        fflush(gnuplotPipe);
        
        tempDataFile = fopen(tempDataFileName1,"w");
        for (i=0; i <= dataSize; i++) {
            x = xData[i];
            y = yData[i];
            fprintf(tempDataFile,"%lf %lf\n",x,y);
        }
        fclose(tempDataFile);
        
        tempDataFile = fopen(tempDataFileName2,"w");
        for (i=0; i <= dataSize; i++) {
            x2 = xData[i];
            y2 = yData2[i];
            fprintf(tempDataFile,"%lf %lf\n",x2,y2);
        }
        fclose(tempDataFile);
        
        tempDataFile = fopen(tempDataFileName3,"w");
        for (i=0; i <= dataSize; i++) {
            x3 = xData[i];
            y3 = yData3[i];
            fprintf(tempDataFile,"%lf %lf\n",x3,y3);
        }
        fclose(tempDataFile);
        
        printf("press enter to continue...");
        getchar();
        remove(tempDataFileName1); //deletes the file
        remove(tempDataFileName2);
        remove(tempDataFileName3);
        fprintf(gnuplotPipe,"exit \n");
    } else {
        printf("gnuplot not found...");
    }
}

