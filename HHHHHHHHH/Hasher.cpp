#include "Hasher.h"

size_t hasher<int>::operator()(const int& x) {
    return x;
}
