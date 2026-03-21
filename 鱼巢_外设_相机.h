//
// 鱼巢_外设_相机.h
//

#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "鱼巢_外设_相机_resource.h"

class C鱼巢外设相机App : public CWinApp
{
public:
    C鱼巢外设相机App();

public:
    virtual BOOL InitInstance() override;
    virtual int ExitInstance() override;

    DECLARE_MESSAGE_MAP()
};

extern C鱼巢外设相机App theApp;

