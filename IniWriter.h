#ifndef INIWRITER_H
#define INIWRITER_H

#include <windows.h>
#include <tchar.h>

class CIniWriter
{
public:
    CIniWriter(LPCTSTR szFileName);
    void WriteInteger(LPCTSTR szSection, LPCTSTR szKey, int iValue);
    void WriteFloat(LPCTSTR szSection, LPCTSTR szKey, float fltValue);
    void WriteBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolValue);
    void WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);
private:
    TCHAR m_szFileName[255];
};
#endif //INIWRITER_H
