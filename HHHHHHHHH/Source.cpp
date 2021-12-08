#include <iostream>
#include <assert.h>

#include "JX3.h"
#include "KHashMap.h"

int main() {
    int x = 123456, y = 12345, z = 102938089;
    KHashMap<int, int, std::hash<int>, std::equal_to<int> >mp[2];
    
    for (int i = 0; i < x; ++i)
    {
        assert(mp[0].Insert(i, i) == TRUE);
    }
    for (int i = 0; i < x * 2; ++i)
    {
        int* p = mp[0].GetValue(i);
        assert(i < x && p && *p == i || i >= x && !p);
    }
    mp[1] = mp[0];
    for (int i = 0; i < y; ++i)
    {
        assert(mp[0].Delete(i) == TRUE);
    }
    for (int i = 0; i < x * 2; ++i)
    {
        int* p = mp[1].GetValue(i);
        assert(i < x && p && *p == i || i >= x && !p);
    }
    for (int i = 0; i < x * 2; ++i)
    {
        int* p = mp[0].GetValue(i);
        assert((i >= y && i < x && p && *p == i) || (i >= x && !p) || (i < y && !p));
    }
    for (int i = 0; i < x; ++i)
    {
        assert(mp[1].Modify(i, i ^ z) == TRUE);
        int* p = mp[1].GetValue(i);
        assert(p && (*p == (i ^ z)));
    }
    for (int i = 0; i < x * 2; ++i)
    {
        int* p = mp[0].GetValue(i);
        assert((i >= y && i < x && p && *p == i) || (i >= x && !p) || (i < y && !p));
    }
    std::cout << "...";
    return 0;
}
