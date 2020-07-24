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
    const T* operator[] (const size_t periodNum) const
    {
        return &auxVector[periodNum*(periodNum+1) + (periodNum+1)*numOfInitialNodes];
    }

    T* operator[] (const size_t rowNum)
    {
        return &auxVector[periodNum*(periodNum+1) + (periodNum+1)*numOfInitialNodes];
    }

    //swappers
    void swapper(matrix& rhs)
    {
        auxVector.swap(rhs.auxVector);
        std::swap(numOfRows, rhs.numOfPeriods);
        std::swap(numOfCols, rhs.numOfInitialNodes);
    }

    //copy ctor and copy =
    tree (const tree& rhs): 
        numOfPeriods(rhs.numOfRows), numOfInitialNodes(rhs.numOfInitialNodes), 
        auxVector(rhs.auxVector){}
    
    tree& operator= (const tree& rhs) 
    {
        if (this == &rhs) return *this;
        tree<T> thecopyof(rhs);
        swapper(thecopyof); //this does not feel natural! why create a temp?
        return *this;
    }

    //move ctor and =
    tree(tree&& rhs):
        numOfRows(rhs.numOfPeriods),
        numOfCols(rhs.numOfInitialNodes),
        auxVector(move(rhs.auxVector)){}

    tree operator=(tree&& rhs)
    {
        if (this = &rhs) return *this;
        tree<T> temp(move(rhs)); //do we have such a move?
        swapper(temp);
        return *this;
    }

    //extra functions
    //transpose
    //isempty
    //ones, zeros, diagonal



};

 

 
 