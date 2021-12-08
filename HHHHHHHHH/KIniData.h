#ifndef _KINIDATA_H_
#define _KINIDATA_H_

#include "JX3.h"
#include "KIniValue.h"
#include "KHashMap.h"

#define DATA_BUFFER_SIZE        (1 << 13)
#define LINE_BUFFER_SIZE        (128)
#define DEFAULT_KEYVALUE_SIZE   (15)

enum class LINE_STATU
{
    LINE_ERROR,
    LINE_EMPTY,
    LINE_COMMENT,
    LINE_SECTION,
    LINE_VALUE
};

class KIniData
{
public:
    KIniData();
    ~KIniData();

    BOOL Init(const char* cpszPath);
    void UnInit();

    BOOL SettingGroup(const char* cpszSection);
    void EndGroup();

    BOOL InsertValue(const char* cpszKey, int nKeyLen, const char* cpszValue, int nValueLen);
    BOOL InsertValue(const char* cpszKey, const char* cpszValue);

    int GetString(const char* cpszKey, const char* cpszDefault, char* cpszRString, unsigned int dwSize);
    BOOL GetInteger(const char* cpszKey, int nDefault, int& nVaule);

    template <class T>
    inline BOOL GetIntegerToType(const char* cpszKey, int nDefault, T& nValue)
    {
        int nRValue = 0;
        BOOL bRetCode = GetInteger(cpszKey, nDefault, nRValue);
        nValue = nRValue;
        return bRetCode;
    }

    BOOL ModifyString(const char* cpszKey, const char* cpszValue);
    BOOL ModifyInteger(const char* cpszKey, int nValue);

    BOOL InsertSection(const char* cpszSection, int nSectionLen);

private:
    typedef KHashMap<KIniValue, KIniValue, INI_VALUE_HASH, INI_VALUE_EQUAL>         KeyValueMap;
    typedef KHashMap<KIniValue, KeyValueMap, INI_VALUE_HASH, INI_VALUE_EQUAL>       IniDataMap;

    BOOL                    _StrModify(char* pszStr, int& nLength);
    LINE_STATU              _ParseLine(
        const char* pszLine, int nLineLen, char* pszSection, int& nSectionLen,
        char* pszKey, int& nKeyLen, char* pszValue, int& nValueLen
    );
    char*                   _IniDataStorge(const char* cpszPath, int& nDataLen);

    IniDataMap      m_IniDataMap;
    KeyValueMap*    m_pGroupMap;
};

#endif