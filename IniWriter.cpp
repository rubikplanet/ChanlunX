#include "IniWriter.h"
#include <iostream>
#include <windows.h>

// http://blog.csdn.net/sdcxyz/article/details/10946111
// http://www.codeproject.com/Tips/376146/A-Small-Class-to-Read-INI-Files

CIniWriter::CIniWriter(LPCTSTR szFileName)
{
    memset(m_szFileName, 0x00, sizeof(m_szFileName));
    memcpy(m_szFileName, szFileName, _tcslen(szFileName) * sizeof(TCHAR));
}

void CIniWriter::WriteInteger(LPCTSTR szSection, LPCTSTR szKey, int iValue)
{
    TCHAR szValue[255];
    _sntprintf_s(szValue, sizeof(szValue) - 1, TEXT("%d"), iValue);
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}

void CIniWriter::WriteFloat(LPCTSTR szSection, LPCTSTR szKey, float fltValue)
{
    TCHAR szValue[255];
    _sntprintf_s(szValue, sizeof(szValue) - 1, TEXT("%f"), fltValue);
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}

void CIniWriter::WriteBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolValue)
{
    TCHAR szValue[255];
    _sntprintf_s(szValue, sizeof(szValue) - 1, TEXT("%s"), bolValue ? TEXT("True") : TEXT("False"));
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}

void CIniWriter::WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
