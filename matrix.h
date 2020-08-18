#pragma once
#include<vector>
using namespace std;

template <class T>
class matrix
{
    size_t numOfRows;
    size_t numOfCols;
    vector<T> auxVector;

public:
    //two constructers
    matrix(): numOfRows(0), numOfCols(0) {}

    matrix (const size_t rows, const size_t cols):
        numOfRows(rows), numOfCols(cols), auxVector(rows*cols) {}


    //accessors
    size_t numOfRowsIs() const {return numOfRows;}
    size_t numOfColsIs() const {return numOfCols;}
    //as in Gottschling, two versions: const and non-const
    const T* operator[] (const size_t rowNum) const
    {
        return &auxVector[rowNum*numOfCols];
    }

    T* operator[] (const size_t rowNum)
    {
        return &auxVector[rowNum*numOfCols];
    }

    //iterators, I need these in solvers 
    //see if anyone else (Gottshcling etc.) need to define these as I
    //AUTO can replace typename vector<T>::iterator!
    //but learn first
    //typename is a C++ requirement: see:
    //https://stackoverflow.com/questions/610245/
    //where-and-why-do-i-have-to-put-the-template-and-typename-keywords/17579889#17579889
    typename vector<T>::iterator beginiteruent(const size_t rowNum)
    {
        return auxVector.begin()+(rowNum*numOfCols);
    }
    typename vector<T>::iterator enditeruent(const size_t rowNum)
    {
        return auxVector.begin()+(rowNum*numOfCols+numOfCols);
    }
    typename vector<T>::const_iterator beginiteruent(const size_t rowNum) const
    {
        return auxVector.begin()+(rowNum*numOfCols);
    }
    typename vector<T>::const_iterator enditeruent(const size_t rowNum) const
    {
        return auxVector.begin()+(rowNum*numOfCols+numOfCols);
    }

    //swappers
    void swapper(matrix& rhs)
    {
        auxVector.swap(rhs.auxVector);
        std::swap(numOfRows, rhs.numOfRows);
        std::swap(numOfCols, rhs.numOfCols);
    }

    //copy ctor and copy =
    matrix (const matrix& rhs): 
        numOfRows(rhs.numOfRows), numOfCols(rhs.numOfCols), 
        auxVector(rhs.auxVector){}
    
    matrix& operator= (const matrix& rhs) 
    {
        if (this == &rhs) return *this;
        matrix<T> thecopyof(rhs);
        swapper(thecopyof); //this does not feel natural! why create a temp?
        return *this;
    }

    //move ctor and =
    matrix(matrix&& rhs):
        numOfRows(rhs.numOfRows),
        numOfCols(rhs.numOfCols),
        auxVector(move(rhs.auxVector)){}

    matrix operator=(matrix&& rhs)
    {
        if (this = &rhs) return *this;
        matrix<T> temp(move(rhs)); //do we have such a move?
        swapper(temp);
        return *this;
    }

    //extra functions
    //transpose
    //isempty
    //ones, zeros, diagonal
};

//above Matrix returns A[i] as a row, and then


inline void productOfMatrices(const matrix<double>&a, const matrix<double>&b, matrix<double>&c)
{
    const size_t n = a.numOfRowsIs();
    const size_t n_ = a.numOfColsIs();
    const size_t m = b.numOfColsIs();


    for (size_t i = 0; i <n; ++i)
    {
        const auto ai = a[i];
        auto ci = c[i];

        for (size_t j=0; j <m; ++j)
        {
            double aux = 0.0;
            for (size_t k=0; k <n_; ++k)
            {
                aux += ai[k] * b[k][j];            
            }
            c[i][j] = aux;
        }
    }
}

inline void productOfMatricesFast(const matrix<double>&a, const matrix<double>&b, matrix<double>&c)
{
    const size_t n = a.numOfRowsIs();
    const size_t n_ = a.numOfColsIs();
    const size_t m = b.numOfColsIs();

    for (size_t i = 0; i <n; ++i)
    {
        auto ci = c[i];
        for (size_t j=0; j <m; ++j)
        {
            ci[j] = 0;
        }
    }

    #pragma omp parallel for 
    for (size_t i = 0; i <n; ++i)
    {
        const auto ai = a[i];
        auto ci = c[i];
        for (size_t k=0; k <m; ++k)
        {
            const auto bk = b[k];
            const auto aik = ai[k];
            //#pragma loop(no_vector)
            for (size_t j=0; j <n_; ++j)
            {
                ci[j] += aik * bk[j];            
            }
            
        }
    }
}

inline double trace(const matrix<double>& a)
{   //should be square but still ...
    const size_t n = a.numOfRowsIs();
    const size_t m = a.numOfColsIs();
     

    double aux(0.0);
    for (size_t i = 0; i <n; ++i)
    {      
        const auto ai = a[i];
        aux += ai[i]; 
        //cout<<i <<" "<<ai[i]<<"\n";
    }
    return aux;
}
