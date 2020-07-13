#include <iostream>
#include "randomness.h"
using namespace std;
int main()
{
    cout<<"hey whats up?"; 
    randomize();
    for (int i = 0; i<10; ++i)
        cout<< pick(1,6);
}