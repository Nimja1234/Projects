+#include <Eigen/Dense>
#include <vector>
#include "asset.hpp"

class solver {                                          //Solver base class, contains common functions used by other subclasses
    protected: 
        Eigen::MatrixXd covariancematrix;               //Covariance matrix
        Eigen::MatrixXd A;                              //A matrix, which is 2 X N in form
        Eigen::MatrixXd A_T;                            //Tranpose of A
        Eigen::MatrixXd leftside;                       //Leftside matrix of equation
        Eigen::MatrixXd rightside;                      //Rightside matrix of equation
        Eigen::VectorXd b;                              //column vector as mentioned in instructions
        Eigen::VectorXd weights;                        //Weights vector, contains weights of each asset in portfolio
        vector<asset> assetsvec;                        //Vector of type assets
        double volatility;                              //Volatility of portfolio
        double r_p;                                     //Used to set portfolio return which will be used to calculate other values

    public:
        virtual void compute(Eigen::MatrixXd& correlationmatrix, vector<asset>& assetvector, double portfolioreturn) = 0;   

        virtual void computercovariancematrix(Eigen::MatrixXd& correlationmatrix, vector<asset>& assetvector) {    //Computes covariance matrix
                                                                        //Takes correlationmatrix and assetvector information collected from parser.cpp
            assetsvec = assetvector;                                    
            int rows = correlationmatrix.rows();
            int cols = correlationmatrix.cols();
            covariancematrix.resize(rows,cols);                          //Resizing covariance matrix so it has the same size as the correlation matrix
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {   
                    covariancematrix(i,j) = correlationmatrix(i,j)*assetsvec[i].getdev()*assetsvec[j].getdev();    //Calculating covariance matrix
                }   
            }
        }

        virtual void computeA() {                           //Computes A matrix
            A.resize(2,assetsvec.size());                       //Resizing A so it has the correct size
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < assetsvec.size(); j++) {
                    if (i == 0) {
                        A(i,j) = 1;
                    }
                    else {
                        A(i,j) = assetsvec[j].getrate();
                    }
                }
            }
            A_T = A.transpose();     //Also creates transposed A matrix
        }

        virtual void computeb() {               //Computes b vector
            b.resize(2);
            b(0) = 1.0;
            b(1) = r_p;
        }

        virtual void computeleftside() {                                    //Computes leftside matrix of equation
            int rows = covariancematrix.rows() + A.rows();                  //leftside has rows the same number of rows as covariancematrix plus A
            int cols = covariancematrix.cols() + A_T.cols();                //leftside has columns the same number of cols as covariancematrix plus A
            leftside.resize(rows,cols);                                     //Resizes leftside so it has correct dimensioning
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {                            //Below logic is for creating leftside matrix
                    if (j < covariancematrix.cols() && i < covariancematrix.rows()) {
                        leftside(i,j) = covariancematrix(i,j);
                    } 
                    else if (j >= covariancematrix.cols() && i < covariancematrix.rows()) {
                        leftside(i,j) = A_T(i,j-covariancematrix.rows());
                    }
                    else if (j < covariancematrix.cols() && i >= covariancematrix.rows()) {
                        leftside(i,j) = A(i-covariancematrix.rows(),j);
                    }
                    else {
                        leftside(i,j) = 0.0;
                    }
                }
            }
        }

        virtual void computerightside() {           //Computes rightside matrix of equation
            size_t rightsiderows = assetsvec.size() + 2;            //amount of rows in rightside equals amount of assets plus 2
            rightside.resize(rightsiderows,1);                      //resizes rightside matrix to correct dimension
            for (size_t i = 0; i < rightsiderows; i++) {           //Logic for creating rightside matrix below
                if (i < rightsiderows - 2) {
                    rightside(i,0) = 0;
                }
                else if (i == rightsiderows - 2) {
                    rightside(i,0) = 1.0;
                }
                else {
                    rightside(i,0) = r_p;
                }
            }
        }

        virtual void computeweights() {                        //Computes weight vector
            Eigen::VectorXd weightstemp = leftside.colPivHouseholderQr().solve(rightside);       //Uses linear algebra to calculate weights vector
            weights = weightstemp.head(covariancematrix.rows());         //weightstemp holds langrange multipliers as well, this makes sure weights vector
                                                                         //holds only weights of assets
        }

        virtual void computevolatility() {                              //Computes volatility of portfolio
            double sum = 0;
            for (int i = 0; i < weights.size(); i++) {                      //Logic below for calculating summation of weights 
                                                                            //and covariancematrix elements underneath square root
                for (int j = 0; j < weights.size(); j++) {
                    sum = sum + (weights(i)*weights(j)*covariancematrix(i,j));
                }  
            }
            volatility = sqrt(sum);                                 //Calculates volatility
        }

        virtual double getvolatility() {                                //Getter for volaility of portfolio
            return volatility;
        }

        virtual void showweights() {
            for (size_t i = 0; i < assetsvec.size(); i++) {
                cout << weights(i) << ", ";
            }
            cout << endl;
        }
};

class unrestricted : public solver {                 //Sub class use for unrestricted optimization method
    public:

    void compute(Eigen::MatrixXd& correlationmatrix, vector<asset>& assetvector, double portfolioreturn) override {
        //This sets the order needed to complete a unrestricted optimization method
        r_p = portfolioreturn;
        computercovariancematrix(correlationmatrix,assetvector);
        computeA();
        computeb();
        computeleftside();
        computerightside();
        computeweights();
        computevolatility();
    }
};

class restricted : public solver {                  //Sub class used for restricted optimization method
    public:

    void compute(Eigen::MatrixXd& correlationmatrix, vector<asset>& assetvector, double portfolioreturn) override {
        //This sets the order needed to complete a restricted optimization method
        r_p = portfolioreturn;
        computercovariancematrix(correlationmatrix,assetvector);
        computeA();
        computeb();
        computeleftside();
        computerightside();
        computeweights();
        if (checkweights()) {
            computevolatility();
        }
        else {                                     //If negative weights exist, continues to modify 
                                                   //left and right side of equation till no negative weights exist
            while(!checkweights()) {
                modifyleftandright();
                computeweights();
            }
            computevolatility();
        }
    }

    bool checkweights() {                         //Checks to see if weights are negative
        for (int i = 0; i < weights.size(); i++) {
            if (weights(i) < 0) {
                return false;
            }
        }
        return true;
    }

    void modifyleftandright() {                          //Modifies left and right side of equation accordingly based on if weights are negative
        for (int i = 0; i < weights.size(); i++) {
            if (weights(i) < 0) {
                modifyleftside(i);
                modifyrightside();
            }
        }
    }

    void modifyleftside(int index) {                     //Modifies left side
        leftside.conservativeResize(leftside.rows() + 1, leftside.cols() + 1);          //Adds a row and column
        int rows = leftside.rows();
        int cols = leftside.cols();
        for (int i = 0; i < rows; i++) {                        //Logic for correctly modifying leftside matrix correctly
            for (int j = 0; j < cols; j++) {
                if (i == leftside.rows() - 1) {
                    if (j == index) {
                        leftside(i, j) = 1.0;
                    } else {
                        leftside(i, j) = 0;
                    }
                } else if (j == leftside.cols() - 1) {
                    if (i == index) {
                        leftside(i, j) = 1.0;
                    } else {
                        leftside(i, j) = 0;
                    }
                }
            }
        }
    }

    void modifyrightside() {           //Modifies right side of equation 
        rightside.conservativeResize(rightside.rows() + 1,1);                 //Adds a row
        rightside(rightside.rows() - 1) = 0;                            //Sets added row to zero
    }
};