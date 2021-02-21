#pragma once
#include <vector>
#include <algorithm> //for max_element
#include <cassert>
#include "matrix.h"

//solve: Ax = y 
std::vector<double> solveEqn(matrix<double> &A, std::vector<double> &y)
{
	 
	size_t n = A.numOfRowsIs();
	size_t m = A.numOfColsIs();
	//this code is for square systems so
	//assert(n == m);	 

	//find maximum element of the absolute values in a row, for each row
    //rows are augmented rows: A+y
	std::vector<double> max_in_a_row_all;
	double val_max = 0.0;// *std::max_element(Jac[0].begin(), Jac[0].end());
	double val_min = 0.0;// *std::max_element(Jac[0].begin(), Jac[0].end());

	for (unsigned int i = 0; i < n; ++i)
	{
        val_max = *std::max_element(A.beginiteruent(i), A.enditeruent(i)); 
        val_max = max(val_max, y[i]);
        val_min = *std::min_element(A.beginiteruent(i), A.enditeruent(i));
        val_min = min(val_min, y[i]);
		max_in_a_row_all.push_back(std::max<double>(abs(val_max), abs(val_min)));
		assert(max_in_a_row_all[i] > 0); 
        //if assert fails then there is no unique soln.
	}
	
	//first iterate over columns: 
	for (size_t j = 0; j < m - 1; ++j)
	{
		//find the index of the maximum of abs(element) in a column, for each column
		double relative_col_max = 0;
		double relative_col_max_prev = 0;
		size_t index_of_max = j;
		for (unsigned int i = j; i < n; ++i)
		{
			if (abs(A[i][j]) / max_in_a_row_all[i] > relative_col_max)
			{
				relative_col_max = abs(A[i][j]);
				if (relative_col_max_prev < relative_col_max)
				{
					index_of_max = i;
				}
				relative_col_max_prev = relative_col_max;
			}
            
             
		}
         
        //swap rows i and index_of_max
        std::swap_ranges(A.beginiteruent(j), A.enditeruent(j), A.beginiteruent(index_of_max));
        std::swap(y[j], y[index_of_max]);
		//std::vector<double> temp(A[j]);
	    //Jac[j] = Jac[index_of_max];
		//Jac[index_of_max] = temp;
		double xxx = 0;
        //multiply and subtract step
        //this middle school algebra should be applied
        //both to A and y.
		for (size_t i = j + 1; i < n; ++i)
		{
			xxx = A[i][j] / A[j][j];
			for (size_t k = 0; k < m; ++k)
			{
				A[i][k] = A[i][k] - xxx * A[j][k];
			}
            y[i] = y[i] - xxx * y[j];
        }
	}
	/*
	py::print("augmented jacobian after row switch (turned off now) and Gaussian elimination");
	*/ 
	std::vector<double> solution(n);	 
	solution[n - 1] = y[n-1] / A[n - 1][n - 1];
	for (size_t i = 2; i <= n; ++i)
	{
		double aux = 0;
		for (size_t j = n - i + 1; j < n; ++j)
		{
			aux += A[n - i][j] * solution[j];
		}
		solution[n - i] = (y[n - i] - aux) / A[n - i][n - i];
	}
	  
	return solution;
}

//solve tridiagonal matrix
//here matrix is still a matrix, not a sparse matrix
//label the diagonal with d, upper diagonal with u, lower with l
//first row: d_0, u_0, or A[0,0], A[0,1]
//second row l_1 d_1 u_1 etc. A[1,0], A[1,1], A[1,2]
std::vector<double> solveEqnTridiagonal(matrix<double> &A, std::vector<double> &y)
{
	 
	size_t n = A.numOfRowsIs();
	size_t m = A.numOfColsIs();

	//matrix is row-contiguous
	//go from row 0 to n-1
	//for row i= 1 to n-1: 
	//    A[i,i] = A[i,i] (diago)- A[i-1,i](upper) 
	//                * A[i,i-1](lower) / A[i-1,i-1]
	//
	//    y[i] = y[i] - y[i-1]
	//                * A[i,i-1](lower) / A[i-1,i-1]
	//
	//the matrix became upper triangular
	//now go backwards to solve for x:
	// x_{n-1} = y_{n-1}/A[n-1,n-1]
	//for row j = n-2 to 0
	//    x[j] = (y[j] - A[j,j+1] * x[j+1])/ A[j,j] 
	for (size_t i = 1; i<n; ++i)
	{
		A[i][i] = A[i][i] - A[i-1][i]*A[i][i-1]/A[i-1][i-1];
		y[i] = y[i] - y[i-1] * A[i][i-1]/A[i-1][i-1];   
	}

	std::vector<double> solution(n); 
	solution[n - 1] = y[n-1] / A[n - 1][n - 1];
	for (size_t i = 2; i <= n; ++i)
	{		 
		solution[n - i] = (y[n - i] - A[i][i+1] * solution[n - i +1] ) / A[n - i][n - i];
	}

	return solution;
}
