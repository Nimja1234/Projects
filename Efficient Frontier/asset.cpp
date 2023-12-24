#include <string>
#include <ostream>
#include <fstream>
#include <iomanip>
#include "asset.hpp"
using namespace std;

asset::asset() : name(""),rate_of_return(0), standard_deviation(0) {}   //Default Constructor

//Constructor
asset::asset(string& somename, double& some_rate, double& some_dev) : name(somename), rate_of_return(some_rate), standard_deviation(some_dev) {}

void asset::changerate(double& some_rate) {             //Sets rate of return
    rate_of_return = some_rate;
}

void asset::changedev(double& some_dev) {               //Sets standard deviation  
    standard_deviation = some_dev;
}

void asset::setname(string& somename) {                 //Sets name of asset
    name = somename;
}

double asset::getdev() {                                //Getter for standard deviation
    return standard_deviation;
}

double asset::getrate() {                               //Getter for rate of return
    return rate_of_return;
}
ostream& operator<<(ostream& os, const asset& someasset) {                     //ostream operator 
    os << "This is asset: " << someasset.name << " Rate of Return: " << someasset.rate_of_return << 
    " Standard Deviation: " << someasset.standard_deviation << endl;
    return os;
}