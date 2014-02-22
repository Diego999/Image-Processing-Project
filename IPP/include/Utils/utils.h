#ifndef UTILS_H
#define UTILS_H

#include <vector>

namespace ipp_random
{
    int randInt(int a, int b); // return a <= x <= b
    double randDouble(double a, double b);// return a <= x < b

    std::vector<int> randIntList(int a, int b, int n); // return a <= x <= b, n times
    std::vector<double> randDoubleList(double a, double b, int n);// return a <= x < b, n times
}

#endif // UTILS_H
