#include <random>
//ref: Gottschling, Chapter 4

std::default_random_engine& global_urng()
{
    static std::default_random_engine u{};
    return u;
}

//if you don't want a fixed sequence of numbers
//run this func before pick(...)
//i.e. 
// randomize();
// cout<< pick (1,2);
void randomize()
{
    static std::random_device rd{};
    global_urng().seed(rd());
}

//gives an int in [from, thru]
int pick(int from, int thru)
{
    static std::uniform_int_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d(global_urng(),parm_t{from, thru});
}

//gives a double in [from, upto]
double pick(double from, double upto)
{
    static std::uniform_real_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d(global_urng(),parm_t{from, upto});
}