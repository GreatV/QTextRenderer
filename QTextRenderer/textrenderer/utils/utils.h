#ifndef TR_UTILS_H
#define TR_UTILS_H

#include <vector>
#include <random>

template<typename T>
std::vector<T> random_choice(const std::vector<T> &raw_samples, size_t size)
{
    std::vector<T> ret_value;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, raw_samples.size() - 1);

    for (size_t i = 0; i < size; i++)
    {
        auto first = std::begin(raw_samples);
        auto offset = dis(gen);

        std::advance(first, offset);
        ret_value.push_back(*first);
    }

    return ret_value;
}

#endif
