#include<vector>
using namespace std;

template <class T>
class tree
{
    size_t numOfPeriods;
    size_t numOfInitialNodes;
    //size_t endPeriod;
    vector<T> auxVector;

public:
    //using value_type = T;
    
    //two constructors
    matrix(): numOfPeriods(0), numOfInitialNodes(0) {}

    //trinomial tree with 1 initial node and k periods will have k**2 entries
    //suppose ageneral setup where tree can start at other periods
    //e.g. starts at 2 and ends at 3: then 3 nodes at initial time, and 5 nodes at final
    // i.e. 8 nodes. Verify that this is 3*3 - 1*1
    matrix (const size_t periods, const size_t initialnodes):
        numOfPeriods(periods), numOfInitialNodes(initialnodes), 
        auxVector( numOfPeriods*(numOfPeriods-1) + numOfPeriods*numOfInitialNodes) {}

    //above formula is from k, k+2, ..., k+2(n-1)
    //for k = numofinitial nodes, n = numofperiods
    //accessors
    size_t numOfPeriodsIs() const {return numOfPeriods;}
    size_t numOfIniNodesIs() const {return numOfInitialNodes;}
    
    //as in Gottschling, two versions: const and non-const
    const T* operator[] (const size_t rowNum) const
    {
        return &auxVector[rowNum*numOfCols];
    }

    T* operator[] (const size_t rowNum)
    {
        return &auxVector[rowNum*numOfCols];
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
