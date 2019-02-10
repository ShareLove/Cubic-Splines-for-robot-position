#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include "spline.h"

#define stepSize 0.2

struct position {
  float x;
  float y;
} ;
float setLimits(float, float, float);
position* calculateSmoothedPath(position*, int);
position* calculateCubicSplines(position*, position*, int);


float setLimits(float value, float min, float max)
{
    if(value < min)
        value = min;
    if(value > max)
        value = max;
    return value;
}

position* calculateSmoothedPath(position path[], int pathSize){
    int i = 1;
    position splittedPath[100];
    position smoothedPath[1000];
    position remainingPath[1000];
    position wholePath[10000];

    // split path


    // x-value increasing (going to right of map)
    if((path + 1)->x > (path + 0)->x)
    {
        printf("\nIncreasing x-values\n");

        // take elements of array until x-value decreases
        (splittedPath + 0)->x = (path + 0)->x;
        (splittedPath + 0)->y = (path + 0)->y;

        while((path + i)->x > (path + i - 1)->x && i<= pathSize)
        {
            (splittedPath + i)->x = (path + i)->x;
            (splittedPath + i)->y = (path + i)->y;

            i++;
        }

        // calculate Cubic Splines with the splitted path
        calculateCubicSplines(splittedPath, smoothedPath, i);   

        printf("i = %d\n", i);
        // remove elements of path that has been already calculated
        if(i < pathSize)
        {
            printf("pathSize = %d\n", pathSize);
            //printf("<\n");
            for(int j = 0; j < pathSize - i + 1; j++)   // do not remove the last value put it in first value for next one!!!!!!!!!!!!!!!!!!!!!!!
            {
                //printf("<<\n");
                (remainingPath + j)->x = (path + j + i - 1)->x;
                (remainingPath + j)->y = (path + j + i - 1)->y;
                //printf("/%d + %d\n", j, i);
                printf("%f, %f\n", (remainingPath + j)->x, (remainingPath + j)->y);
            }
            //printf("<<<\n");
            calculateSmoothedPath(remainingPath, pathSize - i + 1);
        }
    }

    // x-value decreasing (going to left of map)
    else if((path + 1)->x < (path + 0)->x)
    {
        printf("\nDecreasing x-values\n");

        // take elements of array until x-value increases
        (splittedPath + 0)->x = (path + 0)->x;
        (splittedPath + 0)->y = (path + 0)->y;
        printf("\n%f, %f\n", (splittedPath + 0)->x, (splittedPath + 0)->y);

        while((path + i)->x < (path + i - 1)->x && i< pathSize)
        {
            (splittedPath + i)->x = (path + i)->x;
            (splittedPath + i)->y = (path + i)->y;

            printf("-->%f, %f\n", (splittedPath + i)->x, (splittedPath + i)->y);
            printf("i = %d, pathSize = %d\n", i, pathSize);

            i ++;
        }

        // inverse order
        position tmp;
        for(int j = 0; j < (int)(i / 2); j++)
        {
            printf("inside for loop\n");
            tmp.x = (splittedPath + j)->x;
            (splittedPath + j)->x = (splittedPath + i - 1 - j)->x;
            (splittedPath + i - 1 - j)->x = tmp.x;

            tmp.y = (splittedPath + j)->y;
            (splittedPath + j)->y = (splittedPath + i - 1 - j)->y;
            (splittedPath + i - 1 - j)->y = tmp.y;
        }

        // calculate Cubic Splines with the splitted path
        calculateCubicSplines(splittedPath, smoothedPath, i);   

        printf("i = %d\n", i);
        printf("pathSize = %d\n", pathSize);
        // remove elements of path that has been already calculated
        if(i < pathSize)
        {
            printf("pathSize = %d\n", pathSize);
            //printf("<\n");
            for(int j = 0; j < pathSize - i + 1; j++)   // do not remove the last value put it in first value for next one!!!!!!!!!!!!!!!!!!!!!!!
            {
                //printf("<<\n");
                (remainingPath + j)->x = (path + j + i - 1)->x;
                (remainingPath + j)->y = (path + j + i - 1)->y;
                //printf("/%d + %d\n", j, i);
                printf("%f, %f\n", (remainingPath + j)->x, (remainingPath + j)->y);
            }
            //printf("<<<\n");
            calculateSmoothedPath(remainingPath, pathSize - i + 1);
        }
    }

    else{
        return 0;
    }


    return 0;
}

position* calculateCubicSplines(position path[], position smoothedPath[], int pathSize){
    // spline function necessarily needs x that increase
    // --> the path needs to be splitted
    int ctr = 0;

    std::vector<double> X(pathSize), Y(pathSize);
    for(int i = 0; i < pathSize; i++)
    {
        X[i] = (path + i)->x; 
        Y[i] = (path + i)->y;
       printf("%f, %f\n", X[i], Y[i]);
    }

    tk::spline s;
    s.set_points(X,Y); 

    (smoothedPath + 0)->x = (path + 0)->x;
    (smoothedPath + 0)->y = s((smoothedPath + 0)->x);
    printf("\n%f, %f\n", (smoothedPath + 0)->x, (smoothedPath + 0)->y);

    //for(int i = 0; i <= (15-8)/0.5; i++)//(int)((path[pathSize-1] - path[0]) / stepSize); i++)
    for(float f = (path + 0)->x; f <= (path + pathSize - 1)->x - stepSize; f += stepSize) //f = 8.0 + stepSize; f <= 15.0; f+= stepSize)
    {
        ctr++;
        (smoothedPath + ctr)->x = (smoothedPath + ctr - 1)->x + stepSize;

        (smoothedPath + ctr)->y = setLimits(s((smoothedPath + ctr)->x), 0, 15);


        printf("%f, %f\n", (smoothedPath + ctr)->x, (smoothedPath + ctr)->y);
    }
/*
    printf("\n\n%f, %f\n", 8.0, s(8.0));
    printf("%f, %f\n", 8.2, s(8.2));
    printf("%f, %f\n", 8.4, s(8.4));
    printf("%f, %f\n", 8.6, s(8.6));
    printf("%f, %f\n", 8.8, s(8.8));
    printf("%f, %f\n", 9.0, s(9.0));
    printf("%f, %f\n", 9.2, s(9.2));
    printf("%f, %f\n", 9.4, s(9.4));
    printf("%f, %f\n", 9.6, s(9.6));
    printf("%f, %f\n", 9.8, s(9.8));
    printf("%f, %f\n", 10.0, s(10.0));
    printf("%f, %f\n", 10.2, s(10.2));
    printf("%f, %f\n", 10.4, s(10.4));
    printf("%f, %f\n", 10.6, s(10.6));
    printf("%f, %f\n", 10.8, s(10.8));
    printf("%f, %f\n", 11.0, s(11.0));
    printf("%f, %f\n", 11.2, s(11.2));
    printf("%f, %f\n", 11.4, s(11.4));
    printf("%f, %f\n", 11.6, s(11.6));
    printf("%f, %f\n", 11.8, s(11.8));
    printf("%f, %f\n", 12.0, s(12.0));
    printf("%f, %f\n", 12.2, s(12.2));
    printf("%f, %f\n", 12.4, s(12.4));
    printf("%f, %f\n", 12.6, s(12.6));
    printf("%f, %f\n", 12.8, s(12.8));
    printf("%f, %f\n", 13.0, s(13.0));
    printf("%f, %f\n", 13.2, s(13.2));
    printf("%f, %f\n", 13.4, s(13.4));
    printf("%f, %f\n", 13.6, s(13.6));
    printf("%f, %f\n", 13.8, s(13.8));
    printf("%f, %f\n", 14.0, s(14.0));
    printf("%f, %f\n", 14.2, s(14.2));
    printf("%f, %f\n", 14.4, s(14.4));
    printf("%f, %f\n", 14.6, s(14.6));
    printf("%f, %f\n", 14.8, s(14.8));
    printf("%f, %f\n", 15.0, s(15.0));
*/

   return smoothedPath;
}

int main(int argc, char** argv) {
    position path[10];

    path[0].x = 8.0;
    path[0].y = 8.0;
    path[1].x = 8.4;
    path[1].y = 6.0;
    path[2].x = 9.9;
    path[2].y = 3.0;
    path[3].x = 12.3;
    path[3].y = 5.5;
    path[4].x = 12.4;
    path[4].y = 8.7;
    path[5].x = 15.0;
    path[5].y = 14.0;
    path[6].x = 10.0;
    path[6].y = 9.7;
    path[7].x = 9.2;
    path[7].y = 10.0;

    path[8].x = 10.0;
    path[8].y = 11.0;
    path[9].x = 11.2;
    path[9].y = 12.0;

    /*path[0].x = 15.0;
    path[0].y = 14.0;
    path[1].x = 10.0;
    path[1].y = 9.7;
    path[2].x = 9.2;
    path[2].y = 10.0;
    path[3].x = 8.6;
    path[3].y = 7.0;*/
/*
    path[0].x = 0.0;
    path[0].y = 5.0;
    path[1].x = 1.0;
    path[1].y = 0.0;
    path[2].x = 2.0;
    path[2].y = 5.0;
    path[3].x = 1.5;
    path[3].y = 1.0;
    path[4].x = 0.3;
    path[4].y = 0.0;*/

    int pathSize = 10;

    calculateSmoothedPath(path, pathSize);

   return 0;
}