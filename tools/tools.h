/**
 * @file    tools.h
 * @brief   辅助工具实现
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
     * @brief 根据给定的路径，创建文件夹，支持多级文件夹路径创建
     * @param[in] szPath    需要创建的路径
     * @return int 创建成功返回0，失败返回负数
     */
    int smkdir(const TCHAR* szPath);
}


#endif /*__tools_H__*/