/**
 * @file    WTLHelpFunction.h
 * @brief   ¸¨ÖúWTL¿âµÄº¯Êý¿â
 * @version 1.0
 * @date    2016.3.3
 */
#ifndef __WTLHelpFunction_H__
#define __WTLHelpFunction_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<typename _tClass, typename _tParentClass>
void AttachControl(_tClass& obj, unsigned int nID, const _tParentClass& parent)
{
    obj.Attach(parent->GetDlgItem(nID));
}

#endif /*__WTLHelpFunction_H__*/