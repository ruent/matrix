#include<vector>
using namespace std;

template <class T>
class matrix
{
    size_t numOfRows;
    size_t numOfCols;

    vector<T> auxVector;

public:
    using value_type = T;
    
    //two constructers
    matrix(): numOfRows(0), numOfCols(0) {}

    matrix (const size_t rows, const size_t cols):
        numOfRows(rows), numOfCols(cols), auxVector(rows*cols) {}


    //accessors
    size_t numOfRowsGet() const {return numOfRows;}
    size_t numOfColsGet() const {return numOfCols;}
    //as in Gottschling, two versions: const and non-const
    const T* operator[] (const size_t rowNum) const
    {
        return &auxVector[rowNum*numOfCols];
    }

    T* operator[] (const size_t rowNum)
    {
        return &auxVector[rowNum*numOfCols];
    }

};

inline void productOfMatrices(const matrix<double>&a, const matrix<double>&b, matrix<double>&c)
{
    const size_t n = a.numOfRowsGet();
    const size_t n_ = a.numOfColsGet();
    const size_t m = b.numOfColsGet();


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
    const size_t n = a.numOfRowsGet();
    const size_t n_ = a.numOfColsGet();
    const size_t m = b.numOfColsGet();

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
    const size_t n = a.numOfRowsGet();
    const size_t m = a.numOfColsGet();
     

    double aux(0.0);
    for (size_t i = 0; i <n; ++i)
    {      
        const auto ai = a[i];
        aux += ai[i]; 
        //cout<<i <<" "<<ai[i]<<"\n";
    }
    return aux;
}
