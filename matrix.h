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
        numOFRows(rows), numOfCols(cols), auxVector(rows*cols) {}


    T* operator[] (const size_t rowNum)
    {
        return &auxVector[rowNum*numOfCols];
    }

};
