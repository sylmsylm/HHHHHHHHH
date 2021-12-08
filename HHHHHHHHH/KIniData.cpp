#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include <iostream>

#include "KIniData.h"

KIniData::KIniData() 
{
    m_pGroupMap = NULL;
}

KIniData::~KIniData() {}

BOOL KIniData::Init(const char* cpszPath)
{
    BOOL                    bResult                     = FALSE;
    BOOL                    bRetCode                    = FALSE;
    int                     nDataLen                    = 0;
    int                     nLineLen                    = 0;
    int                     nSectionLen                 = 0;
    int                     nKeyLen                     = 0;
    int                     nValueLen                   = 0;
    LINE_STATU              eRetStatu                   = LINE_STATU::LINE_ERROR;
    char*                   pszIniData                  = NULL;
    char                    szLine[LINE_BUFFER_SIZE];
    char                    szKey[INI_VALUE_BUFFER_SIZE];
    char                    szValue[INI_VALUE_BUFFER_SIZE];
    char                    szSection[INI_VALUE_BUFFER_SIZE];

    KG_PROCESS_ERROR(cpszPath);

    pszIniData = _IniDataStorge(cpszPath, nDataLen);
    KG_PROCESS_ERROR(pszIniData);

    for (int nCurPosition = 0; nCurPosition < nDataLen; ++nCurPosition)
    {
        nLineLen = 0;
        while (nLineLen < LINE_BUFFER_SIZE - 1 && nCurPosition < nDataLen && pszIniData[nCurPosition] != '\n')
        {
            szLine[nLineLen++] = pszIniData[nCurPosition++];
        }
        KG_PROCESS_ERROR(nCurPosition == nDataLen || pszIniData[nCurPosition] == '\n');

        if (!nLineLen)
        {
            continue;
        }

        szLine[nLineLen] = '\0';

        bRetCode = _StrModify(szLine, nLineLen);
        KG_PROCESS_ERROR(bRetCode);

        if (!nLineLen)
        {
            continue;
        }

        eRetStatu = _ParseLine(szLine, nLineLen, szSection, nSectionLen, szKey, nKeyLen, szValue, nValueLen);
        switch (eRetStatu)
        {
        case LINE_STATU::LINE_COMMENT:
            break;

        case LINE_STATU::LINE_SECTION:
            bRetCode = InsertSection(szSection, nSectionLen);
            KG_PROCESS_ERROR(bRetCode);
            break;

        case LINE_STATU::LINE_VALUE:
            bRetCode = InsertValue(szKey, nKeyLen, szValue, nValueLen);
            KG_PROCESS_ERROR(bRetCode);
            break;

        case LINE_STATU::LINE_ERROR:
            goto Exit0;

        default:
            break;
        }
    }

    bResult = TRUE;
Exit0:
    if (!bResult)
    {
        m_IniDataMap.Clear();
    }
    return bResult;
}

void KIniData::UnInit()
{
    m_IniDataMap.Clear();
}

BOOL KIniData::SettingGroup(const char* cpszSection)
{
    BOOL            bResult         = FALSE;
    BOOL            bRetCode        = FALSE;
    KIniValue       Section;

    KG_PROCESS_ERROR(cpszSection);

    bRetCode = Section.SetString(cpszSection);
    KG_PROCESS_ERROR(bRetCode);

    m_pGroupMap = m_IniDataMap.GetValue(Section);
    KG_PROCESS_ERROR(m_pGroupMap);

    bResult = TRUE;
Exit0:
    return bResult;
}

void KIniData::EndGroup()
{
    m_pGroupMap = NULL;
}

BOOL KIniData::InsertValue(const char* cpszKey, int nKeyLen, const char* cpszValue, int nValueLen)
{
    BOOL                                    bResult     = FALSE;
    BOOL                                    bRetCode    = FALSE;
    KIniValue                               Key;
    KIniValue                               Value;

    KG_PROCESS_ERROR(m_pGroupMap);
    KG_PROCESS_ERROR(cpszKey);
    KG_PROCESS_ERROR(cpszValue);

    bRetCode = Key.SetString(cpszKey, nKeyLen);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = Value.SetString(cpszValue, nValueLen);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = m_pGroupMap->Insert(Key, Value);
    KG_PROCESS_ERROR(bRetCode);

    bResult = TRUE;
Exit0:
    return bResult;
}

BOOL KIniData::InsertValue(const char* cpszKey, const char* cpszValue)
{
    BOOL                                    bResult     = FALSE;
    BOOL                                    bRetCode    = FALSE;
    KIniValue                               Key;
    KIniValue                               Value;

    KG_PROCESS_ERROR(m_pGroupMap);
    KG_PROCESS_ERROR(cpszKey);
    KG_PROCESS_ERROR(cpszValue);

    bRetCode = Key.SetString(cpszKey);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = Value.SetString(cpszValue);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = m_pGroupMap->Insert(Key, Value);
    KG_PROCESS_ERROR(bRetCode);

    bResult = TRUE;
Exit0:
    return bResult;
}

BOOL KIniData::ModifyString(const char* cpszKey, const char* cpszValue)
{
    BOOL            bResult     = FALSE;
    BOOL            bRetCode    = FALSE;
    KIniValue       Key;
    KIniValue       Value;

    KG_PROCESS_ERROR(m_pGroupMap);
    KG_PROCESS_ERROR(cpszKey);
    KG_PROCESS_ERROR(cpszValue);

    bRetCode = Key.SetString(cpszKey);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = Value.SetString(cpszValue);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = m_pGroupMap->Modify(Key, Value);
    KG_PROCESS_ERROR(bRetCode);

    bResult = TRUE;
Exit0:
    return bResult;
}

BOOL KIniData::ModifyInteger(const char* cpszKey, int nValue)
{
    BOOL            bResult     = FALSE;
    BOOL            bRetCode    = FALSE;
    KIniValue       Key;
    KIniValue       Value;

    KG_PROCESS_ERROR(m_pGroupMap);
    KG_PROCESS_ERROR(cpszKey);

    bRetCode = Key.SetString(cpszKey);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = Value.SetInteger(nValue);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = m_pGroupMap->Modify(Key, Value);
    KG_PROCESS_ERROR(bRetCode);

    bResult = TRUE;
Exit0:
    return bResult;
}

int KIniData::GetString(const char* cpszKey, const char* cpszDefault, char* cpszRString, unsigned int dwSize)
{
    BOOL                                    bResult     = FALSE;
    int                                     nRLength    = -1;
    BOOL                                    bRetCode    = FALSE;
    KIniValue*                              pValue      = NULL;
    KIniValue                               Key;

    KG_PROCESS_ERROR(m_pGroupMap);
    KG_PROCESS_ERROR(cpszKey);
    KG_PROCESS_ERROR(cpszDefault);
    KG_PROCESS_ERROR(cpszRString);

    bRetCode = Key.SetString(cpszKey);
    KG_PROCESS_ERROR(bRetCode);

    pValue = m_pGroupMap->GetValue(Key);
    if (!pValue)
    {
        size_t nDefaultLen = strlen(cpszDefault);

        strncpy(cpszRString, cpszDefault, dwSize);
        cpszRString[dwSize - 1] = '\0';

        nRLength = (int)(nDefaultLen < dwSize ? nDefaultLen : dwSize); 
        goto Exit1;
    }

    nRLength = pValue->GetString(cpszRString, dwSize);
    KG_PROCESS_ERROR(nRLength != -1);

Exit1:
    bResult = TRUE;
Exit0:
    return nRLength;
}

BOOL KIniData::GetInteger(const char* cpszKey, int nDefault, int& nVaule)
{
    BOOL                                    bResult     = FALSE;
    BOOL                                    bRetCode    = FALSE;
    int                                     nRetCode    = 0;
    KIniValue*                              pValue      = NULL;
    KIniValue                               Key;

    KG_PROCESS_ERROR(m_pGroupMap);
    KG_PROCESS_ERROR(cpszKey);

    bRetCode = Key.SetString(cpszKey);
    KG_PROCESS_ERROR(bRetCode);

    pValue = m_pGroupMap->GetValue(Key);
    if (!pValue)
    {
        nVaule = nDefault;
        goto Exit1;
    }

    nRetCode = pValue->GetInteger(nVaule);
    KG_PROCESS_ERROR(nRetCode);

Exit1:
    bResult = TRUE;
Exit0:
    return bResult;
}

BOOL KIniData::InsertSection(const char* cpszSection, int nSectionLen)
{
    BOOL                                    bResult     = FALSE;
    BOOL                                    bRetCode    = FALSE;
    KIniValue                               Section;

    KG_PROCESS_ERROR(cpszSection);

    bRetCode = Section.SetString(cpszSection, nSectionLen);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = m_IniDataMap.Insert(Section, KeyValueMap());
    KG_PROCESS_ERROR(bRetCode);

    m_pGroupMap = m_IniDataMap.GetValue(Section);
    KG_PROCESS_ERROR(m_pGroupMap);

    bResult = TRUE;
Exit0:
    return bResult;
}

BOOL KIniData::_StrModify(char* pszStr, int& nLength)
{
    BOOL    bResult     = FALSE;
    char*   pszStart    = NULL;
    char*   pszEnd      = NULL;

    KG_PROCESS_ERROR(pszStr);

    pszStart = pszStr;
    while (*pszStart && isspace((int)*pszStart))
    {
        pszStart++;
    }

    pszEnd = pszStr + nLength;
    while (pszEnd > pszStart && isspace((int)*(pszEnd - 1)))
    {
        pszEnd--;
    }
    *pszEnd = (char)0;

    memmove(pszStr, pszStart, pszEnd - pszStart + 1);
    nLength = (int)(pszEnd - pszStart);

    bResult = TRUE;
Exit0:
    return bResult;
}

LINE_STATU KIniData::_ParseLine(
    const char* cpszLine, int nLineLen, char* pszSection, int& nSectionLen,
    char* pszKey, int& nKeyLen, char* pszValue, int& nValueLen
)
{
    BOOL        bResult     = FALSE;
    BOOL        bRetCode    = FALSE;
    LINE_STATU  eRetStatu   = LINE_STATU::LINE_ERROR;
    char        szBuffer[LINE_BUFFER_SIZE];

    KG_PROCESS_ERROR(cpszLine);
    KG_PROCESS_ERROR(pszSection);
    KG_PROCESS_ERROR(pszKey);
    KG_PROCESS_ERROR(pszValue);
    KG_PROCESS_ERROR(nLineLen < LINE_BUFFER_SIZE);

    if (cpszLine[0] == '#' || cpszLine[0] == ';')
    {
        eRetStatu = LINE_STATU::LINE_COMMENT;
    }
    else if (cpszLine[0] == '[' && cpszLine[nLineLen - 1] == ']')
    {
        nSectionLen = 0;
        for (int i = 1; i < nLineLen - 1; ++i, ++nSectionLen)
        {
            szBuffer[nSectionLen] = cpszLine[i];
        }
        szBuffer[nSectionLen] = '\0';

        bRetCode = _StrModify(szBuffer, nSectionLen);
        KG_PROCESS_ERROR(bRetCode);
        KG_PROCESS_ERROR(nSectionLen < INI_VALUE_BUFFER_SIZE);

        memcpy(pszSection, szBuffer, nSectionLen);
        pszSection[nSectionLen] = '\0';

        eRetStatu = LINE_STATU::LINE_SECTION;
    }
    else
    {
        int nPosition = 0;
        while (nPosition < nLineLen && cpszLine[nPosition] != '=')
        {
            nPosition++;
        }
        KG_PROCESS_ERROR(nPosition && nPosition != nLineLen);

        for (nKeyLen = 0; nKeyLen < nPosition; ++nKeyLen)
        {
            szBuffer[nKeyLen] = cpszLine[nKeyLen];
        }
        szBuffer[nKeyLen] = '\0';

        bRetCode = _StrModify(szBuffer, nKeyLen);
        KG_PROCESS_ERROR(bRetCode);
        KG_PROCESS_ERROR(nKeyLen < INI_VALUE_BUFFER_SIZE);

        memcpy(pszKey, szBuffer, nKeyLen);
        pszKey[nKeyLen] = '\0';

        for (nValueLen = 0, ++nPosition; nPosition < nLineLen; ++nPosition, ++nValueLen)
        {
            szBuffer[nValueLen] = cpszLine[nPosition];
        }
        szBuffer[nValueLen] = '\0';

        bRetCode = _StrModify(szBuffer, nValueLen);
        KG_PROCESS_ERROR(bRetCode);
        KG_PROCESS_ERROR(nValueLen < INI_VALUE_BUFFER_SIZE);

        memcpy(pszValue, szBuffer, nValueLen);
        pszValue[nValueLen] = '\0';

        eRetStatu = LINE_STATU::LINE_VALUE;
    }

    bResult = TRUE;
Exit0:
    if (!bResult)
    {
        eRetStatu = LINE_STATU::LINE_ERROR;
    }
    return eRetStatu;
}

char* KIniData::_IniDataStorge(const char* cpszPath, int& nDataLen)
{
    BOOL        bResult     = FALSE;
    BOOL        bRetCode    = FALSE;
    int         nRetCode    = 0;
    char*       pszData     = NULL;
    FILE*       fpFile      = NULL;

    KG_PROCESS_ERROR(cpszPath);

    fpFile = fopen(cpszPath, "rb");
    KG_PROCESS_ERROR(fpFile);

    bRetCode = fseek(fpFile, 0, SEEK_END);
    KG_PROCESS_ERROR(bRetCode == 0);

    nDataLen = (int)ftell(fpFile);
    pszData = (char*)malloc(sizeof(char) * nDataLen);
    KG_PROCESS_ERROR(pszData);

    bRetCode = fseek(fpFile, 0, SEEK_SET);
    KG_PROCESS_ERROR(bRetCode == 0);

    nRetCode = (int)fread(pszData, sizeof(char), nDataLen, fpFile);
    KG_PROCESS_ERROR(nRetCode == nDataLen);

    bResult = TRUE;
Exit0:
    if (!bResult)
    {
        if (pszData)
        {
            free(pszData);
            pszData = NULL;
        }
    }
    if (fpFile)
    {
        fclose(fpFile);
        fpFile = NULL;
    }
    return pszData;
}
