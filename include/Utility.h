/**
 * @file    Utility.h
 * @brief   
 * @version 1.0
 * @date    2016.3.3
 */
#ifndef __Utility_H__
#define __Utility_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Utility
{
    /**
    * ����ָ��·��������ָ����׺���ļ�����ͨ���ص�����֪ͨ���ⲿ
    * ʾ������
    * 1.����ص��ӿ� bool onLoop(const TCHAR* szPath, const TCHAR* szFileName)
    * 2.ִ�б��� Utility::TravesAllFiles(�ļ�·��, ɨ���׺, onLoop);
    * ���лص��ӿڷ���true��ʾ��ֹѭ�����ļ�·��������'\\'��β
    */
    template<typename _tFunc>
    bool TravesAllFiles(const TCHAR* pszPath, const TCHAR* pszSuffix, _tFunc func)
    {
        bool bFinish = false;
        HANDLE hFind = INVALID_HANDLE_VALUE;
        do
        {
            TCHAR szFile[MAX_PATH];
            WIN32_FIND_DATA wfData;
            _tcscpy(szFile, pszPath);
            _tcscat(szFile, pszSuffix);

            hFind = ::FindFirstFile(szFile, &wfData);
            if (INVALID_HANDLE_VALUE != hFind)
            {
                for (;;)
                {
                    if (wfData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                    {// directory
                        if (TEXT('.') != wfData.cFileName[0])
                        {
                            _tcscpy(szFile, pszPath);
                            _tcscat(szFile, wfData.cFileName);
                            _tcscat(szFile, TEXT("\\"));
                            if (TravesAllFiles(szFile, pszSuffix, func))
                            {
                                bFinish = true;
                                break;
                            }
                        }
                    }
                    else
                    {// file
                        if (func(pszPath, wfData.cFileName))
                        {
                            bFinish = true;
                            break;
                        }
                    }

                    if (!FindNextFile(hFind, &wfData))
                    {
                        break;
                    }
                }
            }
        } while (false);
        if (INVALID_HANDLE_VALUE != hFind)
        {
            FindClose(hFind);
        }
        return bFinish;
    }
}

#endif /*__Utility_H__*/