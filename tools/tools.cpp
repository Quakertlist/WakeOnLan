#include "stdafx.h"
#include <direct.h>
#include <windows.h>
#include "tools.h"


namespace Utility
{
    int smkdir(const TCHAR * szPath)
    {
        if (szPath == NULL || szPath[0] == TEXT('\0'))
        {
            return -1;
        }
        TCHAR* dir;
        TCHAR fpath[_MAX_PATH];
        int oldDrive;
        int drive;
        TCHAR oldPath[_MAX_PATH];

        _tfullpath(fpath, szPath, _MAX_PATH);
        TCHAR* c = _tcsrchr(fpath, TEXT('\\'));
        if (c != NULL)
        {
            *c = TEXT('\0');
        }
        if (_taccess(fpath, 0) == 0)
        {
            return 0;
        }
        dir = _tcstok(fpath, _T("\\"));
        dir[0] = _totupper(dir[0]);
        drive = dir[0] - 'A' + 1;
        oldDrive = _getdrive();
        _tgetcwd(oldPath, _MAX_PATH);

        _chdrive(drive);
        _tchdir(_T("\\"));
        dir = _tcstok(NULL, _T("\\"));
        while (dir)
        {
            if (_tchdir(dir))
            {
                _tmkdir(dir);
                _tchdir(dir);
            }
            dir = _tcstok(NULL, _T("\\"));
        }
        _chdrive(oldDrive);
        _tchdir(oldPath);
        return 0;
    }
}