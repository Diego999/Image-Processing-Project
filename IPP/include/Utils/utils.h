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

namespace ipp_utils
{
    template<typename T>
    void mergeVectors(std::vector<T>& v1, const std::vector<std::vector<T>>& v2)
    {
        for(auto& v:v2)
            v1.insert(v1.end(), v.begin(), v.end());
    }

    template<typename T>
    std::vector<std::vector<T>> createSubSamples(const std::vector<T>& sets, const int k)
    {
        std::vector<std::vector<T>> samples;
        int sizeSample = sets.size()/k;
        for(int i = 0; i < k-1; ++i)
        {
            samples.push_back(std::vector<T>());
            samples.back().insert(samples.back().end(), sets.begin()+i*sizeSample, sets.begin()+(i+1)*sizeSample);
        }
        samples.back().insert(samples.back().end(), sets.begin()+(k-1)*sizeSample, sets.end());
        return samples;
    }
}

#endif // UTILS_H
