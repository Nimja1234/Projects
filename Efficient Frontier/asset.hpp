#ifndef ASSET_H
#define ASSET_H

#include <iostream>
#include <string>
using namespace std;

class asset {
    string name;                                                          //Name of asset
    double rate_of_return;                                                //Rate of return of asset
    double standard_deviation;                                            //Standard deviation of asset

public:
    asset();                                                              //Default constructor
    asset(string& somename, double& some_rate, double& some_dev);         //Constructor
    void changerate(double& some_rate);                                   //Sets rate of return
    void changedev(double& some_dev);                                     //Sets standard deviaton
    void setname(string& somename);                                       //Sets name of asset
    double getdev();                                                      //Getter for standard deviation
    double getrate();                                                     //Getter for rate of return
    friend ostream& operator<<(ostream& os, const asset& someasset);      //Ostream operator
};

#endif // ASSET_H
