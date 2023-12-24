#include <iomanip>
#include <iostream>
#include "parser.cpp"
#include "kuboksolver.cpp"
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {                    //Incorrect usage
        throw invalid_argument("Incorrect usage: kubok inputfile");
    }
    try {  
        parser parsing;               
        parsing.readinput(argv[1]);                //Reading inputfile, collecting information to create board
        vector<double> row = parsing.getrow();
        vector<double> column = parsing.getcolumn();
        vector<vector<double>> grid = parsing.getgrid();
        kubok kuboksolver(row,column,grid);         //Constructor for kuboksolver, initializing values
        kuboksolver.solver();                       //Solving board
        kuboksolver.printgrid();                    //Printing solved board
    }

    catch (invalid_argument& e) {                   //Catching any errors in program
        cerr << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}