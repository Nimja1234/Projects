
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

class parser {
    string file;
    vector<double> column;
    vector<double> row;
    vector<vector<double>> grid;

    bool appearonce(int& num) {           
        //Checks to see if number appears more than once on board
        int count = 0;
        for (size_t x = 0; x < row.size(); x++) {
            for (size_t y = 0; y < column.size(); y++) {
                if (grid[x][y] == num) {
                    count++;
                }
                if (count == 2) {      //If number appears twice, returns false
                    return false;
                }
            }
        }
        return true;       //Default return true
    }

    vector<string> splitLine(string line, string delim) {           //Reads a line and splits it based on a delimiter
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
        
    ifstream openfile(const string& file) {                     //Opens a file, also borrowed from the Stock Data STL assignment
        ifstream in(file); 
        if (!in.is_open()) {
            throw invalid_argument("File could not be opened");
        }
        return in;
    }

    public:

        void readinput(const string& file) {        
            //Reads input file, collects information such as columns header, rows header, and board
            string line;
            ifstream inputfile = openfile(file);        //open file
            
            while (getline(inputfile, line)) {           //Reads first line containing information of column sums
                if (line.substr(0,13) != "column sums: ") {          //If it is not in this format
                    throw invalid_argument("Check formatting of input file");
                }
                vector<string> columnstring = splitLine(line, ":");         //Splits line based on ":"
                vector<string> columndata = splitLine(columnstring[1], ",");      //Splits line based on comma
                if (columndata.size() != 4 || columnstring.size() != 2) {        //If there are not 4 sums or formatting of line is wrong
                    throw invalid_argument("Check formatting of input file");
                }
                try {
                    for (size_t i = 0; i < columndata.size(); ++i) {
                        double tempval = stod(columndata[i]);
                        if (tempval < 10 || tempval > 58) {          //Column sums should be between 10-58
                            throw invalid_argument("Check formatting of input file");                        }
                        column.push_back(tempval);
                    }
                }
                catch (const invalid_argument& e) {                            
                    throw invalid_argument("Check formatting of input file");
                }
                break;
            }

            while (getline(inputfile, line)) {                          //Reads first line containing information of row sums
                if (line.substr(0,10) != "row sums: ") {                   //If it is not in this format
                    throw invalid_argument("Check formatting of input file");
                } 
                vector<string> rowstring = splitLine(line, ":");        //Splits line based on ":"
                vector<string> rowdata = splitLine(rowstring[1], ",");   //Splits line based on comma
                if (rowdata.size() != 4 || rowstring.size() != 2) {      //If there are not 4 row sums or formatting of line is wrong
                    throw invalid_argument("Check formatting of input file");
                }
                try {
                    for (size_t i = 0; i < rowdata.size(); ++i) {
                        double tempval = stod(rowdata[i]);
                        if (tempval < 10 || tempval > 58) {          //Row sums should be between 10-58
                            throw invalid_argument("Check formatting of input file"); 
                        }
                        row.push_back(tempval);
                    }
                }
                catch (const invalid_argument& e) {                            
                    throw invalid_argument("Check formatting of input file");
                }
                break;
            }

            while (getline(inputfile, line)) {          //Reads line after row sums
                if (line != "grid:") {                 //If it doesnt follow correct formatting
                    throw invalid_argument("Check formatting of input file");
                }
                break;
            }

            while (getline(inputfile, line)) {         //Reads information about board
                vector<string> data = splitLine(line, ",");
                if (data.size() != 4) {               //If there are not 4 cells
                    throw invalid_argument("Check formatting of input file");
                }
                try {
                    vector<double> row_double;
                    for (size_t i = 0; i < data.size(); i++) {
                        if ((stod(data[i]) < 1 || stod(data[i]) > 16) && stod(data[i]) != 0) {      //If number is not between 1-16 or not empty
                            throw invalid_argument("Check formatting of input file");
                        }
                        row_double.push_back(stod(data[i]));
                    }  
                    grid.push_back(row_double);
                }
                catch (const invalid_argument& e) {                            
                    throw invalid_argument("Check formatting of input file");
                }
            }

            if (grid.size() != 4) {               //If board does not have 4 rows
                throw invalid_argument("Check formatting of input file");
            }

            for (int num = 1; num < 17; num++) {      //Checks to make sure no number appears twice, save zeros
                if (!appearonce(num)) {
                    throw invalid_argument("Check formatting of input file");
                }
            }
        }    

        vector<double> getcolumn() {   //Getter for column
            return column;
        }

        vector<double> getrow() {       //Getter for row  
            return row;
        }

        vector<vector<double>> getgrid() {    //Getter for grid  
            return grid;
        }
};



