/**
 * @file    tools.h
 * @brief   ��������ʵ��
 * @version 1.0
 * @date    2016.3.3
 */
#ifndef __tools_H__
#define __tools_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Utility
{
    /**
     * @brief ���ݸ�����·���������ļ��У�֧�ֶ༶�ļ���·������
     * @param[in] szPath    ��Ҫ������·��
     * @return int �����ɹ�����0��ʧ�ܷ��ظ���
     */
    int smkdir(const TCHAR* szPath);
}


#endif /*__tools_H__*/