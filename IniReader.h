#ifndef INIREADER_H
#define INIREADER_H

#include <windows.h>
#include <tchar.h>

class CIniReader
{
public:
    CIniReader(LPCTSTR szFileName);
    int ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue);
    float ReadFloat(LPCTSTR szSection, LPCTSTR szKey, float fltDefaultValue);
    bool ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue);
    LPTSTR ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue);

private:
    TCHAR m_szFileName[255];
};
#endif //INIREADER_H
