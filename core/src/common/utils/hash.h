#ifndef COMMON_UTILS_HASH_H
#define COMMON_UTILS_HASH_H

#include <functional>

template <typename T>
void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

#endif //COMMON_UTILS_HASH_H
