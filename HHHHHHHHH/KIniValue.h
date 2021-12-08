#ifndef _KVALUE_H_
#define _KVALUE_H_

#include "JX3.h"

#define INI_VALUE_BUFFER_SIZE       (64)
#define INI_VALUE_HASH_BASE         (14695981039346656037ULL)
#define INI_VALUE_HASH_PRIME        (1099511628211ULL)

class KIniValue
{
public:
    KIniValue();
    ~KIniValue();

    BOOL SetString(const char* cpszValue, int nValueLen);
    BOOL SetString(const char* cpszValue);
    BOOL SetInteger(const int cnValue);
    
    int  GetString(char* pszValue, size_t uSZValue) const;
    BOOL GetInteger(int& nValue) const;

    BOOL operator==(const KIniValue& cOtherKey) const;

    KIniValue& operator=(const KIniValue& cOtherKey);

    size_t HashSelf() const;

private:
    int             m_nLength;
    char            m_szData[INI_VALUE_BUFFER_SIZE];
};

struct INI_VALUE_HASH 
{
    size_t operator()(const KIniValue& Value) 
    {
        return Value.HashSelf();
    }
};

struct INI_VALUE_EQUAL 
{
    bool operator()(const KIniValue& Value1, const KIniValue& Value2) 
    {
        return Value1 == Value2;
    }
};

#endif // !_KVALUE_H_