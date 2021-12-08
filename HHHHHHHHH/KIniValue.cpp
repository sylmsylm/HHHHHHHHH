#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "KIniValue.h"

KIniValue::KIniValue()
{
    m_nLength       = 0;
}

KIniValue::~KIniValue() {}

BOOL KIniValue::SetString(const char* cpszValue, int nValueLen)
{
    BOOL    bResult     = FALSE;

    KG_PROCESS_ERROR(cpszValue);
    KG_PROCESS_ERROR(nValueLen < INI_VALUE_BUFFER_SIZE);

    strncpy(m_szData, cpszValue, nValueLen);
    m_szData[nValueLen] = '\0';

    m_nLength = nValueLen;

    bResult = TRUE;
Exit0:
    return bResult;
}

BOOL KIniValue::SetString(const char* cpszValue)
{
    BOOL    bResult     = FALSE;
    int     nValueLen   = 0;

    if (!cpszValue)
    {
        return FALSE;
    }

    nValueLen = (int)strlen(cpszValue);
    bResult = SetString(cpszValue, nValueLen);

    return bResult;
}

BOOL KIniValue::SetInteger(const int nValue)
{
    snprintf(m_szData, INI_VALUE_BUFFER_SIZE, "%d", nValue);
    m_szData[INI_VALUE_BUFFER_SIZE - 1] = '\0';
    return TRUE;
}

int KIniValue::GetString(char* pszValue, size_t uSZValue) const
{
    BOOL            bResult         = FALSE;
    int             nRLength        = -1;

    KG_PROCESS_ERROR(pszValue);
    KG_PROCESS_ERROR(m_nLength < uSZValue);

    strncpy(pszValue, m_szData, m_nLength);
    pszValue[m_nLength] = '\0';
    nRLength = m_nLength;

    bResult = TRUE;
Exit0:
    return nRLength;
}

BOOL KIniValue::GetInteger(int& nValue) const
{
    BOOL            bResult         = FALSE;
    int             nRValue         = 0;

    nRValue = strtol(m_szData, NULL, 10);
    KG_PROCESS_ERROR(!(errno == ERANGE && (nRValue == LONG_MAX || nRValue == LONG_MIN)));
    KG_PROCESS_ERROR(!(errno != 0 && nRValue == 0));

    nValue = nRValue;

    bResult = TRUE;
Exit0:
    return bResult;
}

BOOL KIniValue::operator==(const KIniValue& cOtherKey) const
{
    return strcmp(m_szData, cOtherKey.m_szData) == 0;
}

KIniValue& KIniValue::operator=(const KIniValue& cOther)
{
    strncpy(m_szData, cOther.m_szData, INI_VALUE_BUFFER_SIZE);
    m_szData[INI_VALUE_BUFFER_SIZE - 1] = '\0';
    m_nLength = cOther.m_nLength;
    return *this;
}

size_t KIniValue::HashSelf() const
{
    size_t uHash = INI_VALUE_HASH_BASE;

    for (int i = 0; i < m_nLength; ++i)
    {
        uHash ^= (size_t)m_szData[i];
        uHash *= INI_VALUE_HASH_PRIME;
    }

    return uHash;
}