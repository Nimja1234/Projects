#include "parser.cpp"
#include "asset.hpp"
#include "solver.cpp"
#include <iomanip>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    try {
        if (argc == 3) {                                //Initiated when user wants unrestricted optimization
            vector<asset> assetvec;                  //Vector of assets, each object in vector contains information about the asset
            Eigen::MatrixXd correlationmatrix;              //Initializes a correlation matrix
            universeparser parsinguniversal(argv[1]);                //parses universe.csv file
            correlationparser parsingcorrelation(argv[2]);            //parses correlation matrix file

            assetvec = parsinguniversal.getassetdata();                     //gets vector of assets from parsing script
            correlationmatrix = parsingcorrelation.getcorrelationmatrix();          //gets correlation matrix from parsing script

            if (static_cast<int>(assetvec.size()) != static_cast<int>(correlationmatrix.rows())) {  //Checks if either file has more information than the other
                throw invalid_argument("Check file formats: Input files should hold the same amount of information");
            }

            unrestricted a;                                            //Initializes an object of instance unrestricted
            cout << "ROR,volatility" << "\n";                        //Begin printing RoR and volatility information
            for (double i = .01; i < .27; i += 0.01) {                  //Each iteration represents 1%-26%
                a.compute(correlationmatrix, assetvec, i);              //Uses solver script to compute volatility using unrestricted optimization
                cout << fixed << setprecision(1) << i*100 << "%,"
                << fixed << setprecision(2) << a.getvolatility()*100
                << "%" << "\n";
            }
        }
        
        else if (argc == 4) {                           //Initiated when user wants restricted optimization
            string arg1(argv[1]);

            if (arg1 == "-r") {                           //Checks for -r tag
                vector<asset> assetvec;                         //Everything below same as the block before in terms of style and intention
                Eigen::MatrixXd correlationmatrix;
                universeparser parsinguniversal(argv[2]);
                correlationparser parsingcorrelation(argv[3]);

                assetvec = parsinguniversal.getassetdata();
                correlationmatrix = parsingcorrelation.getcorrelationmatrix();

                if (static_cast<int>(assetvec.size()) != static_cast<int>(correlationmatrix.rows())) {
                    throw invalid_argument("Check file formats: Too much information in either input files");
                }

                restricted b;
                cout << "ROR,volatility" << "\n";
                for (double i = .01; i <.27; i += .01) {
                    b.compute(correlationmatrix, assetvec, i);
                    cout << fixed << setprecision(1) << i*100 << "%,"
                    << fixed << setprecision(2) << b.getvolatility()*100
                    << "%" << "\n";
                }
            }
            else {                                                //If usage of program is wrong, throws error
                throw invalid_argument("Incorrect usage");
            }

        }

        else {                                                  //If usage of program is wrong, throws error
            throw invalid_argument("Incorrect usage");
        }

        return EXIT_SUCCESS;
    }

    catch (invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}
