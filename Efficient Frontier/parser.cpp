#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "asset.hpp"
#include <Eigen/Dense>

using namespace std;
//Parses file, collects information from them to be processed later

class fileparser {                         //Base class for parsing, establishes common functions used by subclasses

    public:
        virtual vector<asset> readdata(const string& file) = 0;            

        virtual vector<string> splitLine(string line, string delim) {           //Reads a line and splits it based on a delimiter
            vector<string> result;                                              //puts information in a vector
                                                                                //This is borrowed from the Stock Data STL assignment
            auto start = 0U;
            auto end = line.find(delim);
            while (end != string::npos) {
                result.push_back(line.substr(start, end - start));
                start = end + delim.length();
                end = line.find(delim, start);
            }
            string last = line.substr(start, end);
            if (last.length()) {
                result.push_back(last);
            }
            return result;
            }
            
        virtual ifstream openfile(const string& file) {                     //Opens a file, also borrowed from the Stock Data STL assignment
            ifstream in(file); 
            if (!in.is_open()) {
                throw invalid_argument("File could not be opened");
            }
            return in;
        }
};

class universeparser : public fileparser {          //Sub class used to parse the universe file 
    string file;   
    vector<asset> assets;                               //Initialize a vector of type assets, will be used to store asset information collected

    public:
        vector<asset> readdata(const string& file) override {                   //Reads data of universe file, collects information from it
            string line;
            ifstream inputfile = openfile(file);

            while (getline(inputfile, line)) {
                vector<string> data = splitLine(line, ","); 
                if (data.size() != 3) {                                     //If format of file is wrong, should only have 3 columns per row. 
                                                                            //Also if user put order of input files wrong
                    throw invalid_argument("Check file formats: Universe File\n Also check order of files inputted");
                }

                asset newasset;                                        //Initialize an object of an instance of the asset class
                newasset.setname(data[0]);                              //Sets the name of the asset information from that line to the object newasset
                try {
                    double parsedrate = stod(data[1]);                  //Collects rate of return
                    double parseddev = stod(data[2]);                   //Collects standard deviation
                    newasset.changerate(parsedrate);                    //Sets rate of return for that asset
                    newasset.changedev(parseddev);                      //Sets standard deviation for that asset
                    assets.push_back(newasset);                         //Pushes asset information into assets vector
                }
                catch (const std::invalid_argument& e) {                //If information about assets is not numeric or blank
                    throw invalid_argument("Check file format: Universe File");
                }     
            }
            return assets;
        }   

        vector<asset> getassetdata() {                          //Getter for assets vector
            return assets;
        }

        universeparser(const string& file) {                        //Constructor
            readdata(file);
        }
};

class correlationparser : public fileparser {                       //Sub class used to parse the correlation file
    Eigen::MatrixXd m;                                              //Initialize a matrix that will hold the correlation matrix

    public:
        vector<asset> readdata(const string& file) override {      //Reads data of correlation file, collects information from it
            string line;
            ifstream inputfile = openfile(file);
            vector<vector<double>> matrixtemp;                  //Initialize a temp matrix used to collect information from file
            size_t numrowtrack = 0;                              //Will be used to track if matrix is N X N, and not N X M

            while (getline(inputfile, line)) {
                vector<double> row;
                vector<string> data = splitLine(line, ",");

                if (numrowtrack != 0 && data.size() != numrowtrack) {               //If number of columns in row does not match 
                                                                                    //the number in the first row, matrix is not N X N
                    throw invalid_argument("Check file format: Correlation File - Rows have different numbers of elements");
                }

                for (size_t i = 0; i < data.size(); i++) {
                    try {
                        row.push_back(stod(data[i]));
                    }
                    catch (const std::invalid_argument& e) {                            //If information in matrix is not numeric or blank
                        throw invalid_argument("Check file format: Correlation File");
                    }
                }
                if (numrowtrack == 0) {
                    numrowtrack = row.size();
                }
                matrixtemp.push_back(row);                          //Push back row into matrixtemp vector
            }   

            size_t numrow = matrixtemp.size();
            size_t numcol = matrixtemp[0].size();
            if (numrow != numcol) {                                 //Checks to make sure matrix is not N X M
                                                                                    //mostly another safeguard in case before check fails
                throw invalid_argument("Check file format: Correlation File - Should be N X N, not N X M");
            }

            m.resize(numrow,numcol);                                    //Change size of matrix m to fit size of correlation matrix processed  
            for (size_t i = 0; i < numrow; ++i) {               //Makes m matrix the correlation matrix based on information collected in temp matrix
                for (size_t j = 0; j < numcol; ++j) {
                    m(i,j) = matrixtemp[i][j];
                }
            }

            return {};
        }  

        Eigen::MatrixXd getcorrelationmatrix() {                         //Getter for correlation matrix
            return m;
        }

        correlationparser(const string& file) {                         //Constructor
            readdata(file);
        }
};

#endif // FILEPARSER_HPP
