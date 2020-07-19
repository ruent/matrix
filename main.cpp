#include <iostream>
#include "randomness.h"
#include "matrix.h"
#include <chrono>

using namespace std;
int main()
{
    cout<<"hey whats up?"<<"\n"; 
    //randomize();
    size_t n = 1000;
    size_t m = 1000;
    size_t k = 1000;
    matrix<double> A(n,m);
    
    for (size_t i = 0; i<n; ++i)
    {
        
        for (size_t j = 0; j<m; ++j)
            {
                //cout<< A[i][j]<<"\n";
                double x= pick(0.0,1.0);
                A[i][j] = x;
            }
    }

    matrix<double> B(m,k);
    
    for (size_t i = 0; i<n; ++i)
    {       
        for (size_t j = 0; j<m; ++j)
            {
                double x= pick(0.0,1.0);
                B[i][j] = x;
            }
    }
    matrix<double> C(n,k);
    std::clock_t startcputime = std::clock();
    productOfMatricesFast(A,B,C);
    //cout<< trace(C)<<"\n"; 

    double cpu_duration = (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;
    std::cout << "Finished in " << cpu_duration << " seconds [CPU Clock] " << std::endl;

}