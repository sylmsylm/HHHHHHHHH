#pragma once

template<class _Ty>
struct hasher {
    size_t operator()(const _Ty& v);
};
