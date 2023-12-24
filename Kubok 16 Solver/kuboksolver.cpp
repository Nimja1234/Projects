#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

class kubok {
    vector<double> rows;        //Holds row totals
    vector<double> columns;     //Holds column totals
    vector<vector<double>> grid;        //Represents board

    public:       
        kubok(vector<double>& somerows, vector<double>& somecolumn, vector<vector<double>>& somegrid)   //Constructor
            : rows{somerows}, columns{somecolumn}, grid{somegrid} {
            }
        
        bool isvalid(const vector<vector<double>>& grid,int& row, int& col, int& number) {      
            //Checks to see if number being tried in empty cell could provide a valid solution for the board, gives 
            //bool true if so, else false.
            int sumrow = 0;
            int sumcol = 0;

            for (int x = 0; x < 4; x++) {       
                for (int y = 0; y < 4; y++) {
                    if (grid[x][y] == number) {     //If number already exists on board
                        return false;
                    }
                }
            }

            for (int x = 0; x < 4; x++) {
                sumrow += grid[row][x];
                if ((sumrow + number) > rows[row]) {        //If sum of row with the number being tried would be more than the actual sum of that row 
                    return false;
                }
            } 

            for (int y = 0; y < 4; y++) {
                sumcol += grid[y][col];
                if ((sumcol + number) > columns[col]) {    //if sum of column with the number being tried would be more than the actual sum of that column
                    return false;
                }
            }

            return true;   //Returns true if number is a valid solution
        }

        bool solver() {
            //Base case - no empty cells remaining
            int row = 0;
            int col = 0;
            bool empty = false;
            for (row = 0; row < 4; row++) {
                for (col = 0; col < 4; col++) {
                    if (grid[row][col] == 0) {  //If cell is empty
                        empty = true;
                        break;
                    }
                }
                if (empty) { //Break out of loop is cell is empty
                    break;
                }
            }  

            if (!empty) {      //If cell is not empty
                return true;
            }

            for (int num = 1; num < 17; num++) {         //Goes through number 1-16
                if (isvalid(grid, row, col, num)) {      //If number is a valid solution for that cell
                    grid[row][col] = num;                //Apply number
                    if (solver()) {                      //If true, applies number to cell
                        return true;
                    }  
                    grid[row][col] = 0;                  //Else, reverse cell back to 0 to try another number
                }
            }
            
            return false;                               //Default return 
        }

        void printgrid() {          //Prints grid
            cout << "    ";
            for (size_t i = 0; i < rows.size(); i++) {
                cout << setw(3) << fixed << setprecision(0) << columns[i];
            }
            cout << endl;
            for (size_t i = 0; i < rows.size(); i++) {
                cout << setw(4) << rows[i] << " ";
                for (size_t j = 0; j < columns.size(); j++) {
                    cout << setw(2) << fixed << setprecision(0) << grid[i][j] << " ";
                }
                cout << endl;
            }
        }

};