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

    int c = 0;
    for (KHashMap<int, int, std::hash<int>, std::equal_to<int> >::iterator it = mp[0].begin(); it != mp[0].end(); ++it)
    {
        assert(it->first == it->second);
        c++;
    }
    assert(c == x - y);

    c = 0;
    for (KHashMap<int, int, std::hash<int>, std::equal_to<int> >::iterator it = mp[1].begin(); it != mp[1].end(); it++)
    {
        assert(((it->first) ^ z) == it->second);
        c++;
    }
    assert(c == x);
    mp[1] = KHashMap<int, int, std::hash<int>, std::equal_to<int> >();
    assert(!(mp[1].begin() != mp[1].end()));

    for (int i = 0; i < x; ++i)
    {
        assert(mp[1].Insert(i, (i - z) * y) == TRUE);
    }
    c = 0;
    for (KHashMap<int, int, std::hash<int>, std::equal_to<int> >::iterator it = mp[1].begin(); it != mp[1].end(); ++it)
    {
        assert(((it->first) - z) * y == it->second);
        c++;
    }
    assert(c == x);

    Iter<int, int, std::hash<int>, std::equal_to<int>> it = mp[1].begin();

    return 0;
}
