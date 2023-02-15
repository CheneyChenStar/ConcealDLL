#pragma once

#ifndef __CONCEAL_DLL_H_
#define __CONCEAL_DLL_H_

/************************************************************************************************************************
*	头文件包含
*************************************************************************************************************************/
#include "ntdll/ntdll.h"


#include <tchar.h>
#include <windows.h>
#include <TlHelp32.h>
#include <intrin.h>
/************************************************************************************************************************
*	end
*************************************************************************************************************************/


/************************************************************************************************************************
*	预编译配置
*************************************************************************************************************************/
#ifdef _WIN64
	#pragma comment(lib,"ntdll/ntdll_x64.lib")
#else
	#pragma comment(lib,"ntdll/ntdll_x86.lib")
#endif // !_WIN64

#ifdef __cplusplus
	#define EXPORT extern "C"
#else
	#define EXPORT
#endif //__cplusplus
/************************************************************************************************************************
*	end
*************************************************************************************************************************/

extern PPEB gPeb;

enum Option{
	LoadOrderLinks = 1 << 0,
	MemoryOrderLinks = 1 << 1,
	InitializationOrderLinks = 1 << 2,
	ALL = LoadOrderLinks | MemoryOrderLinks | InitializationOrderLinks,
};


/// <summary>
/// 打印错误信息
/// </summary>
/// <param name="msg">error msg</param>
VOID PrintError(CONST TCHAR* msg);


/// <summary>
/// 打印当前进程所有模块
/// </summary>
BOOL ListProcessModules(DWORD dwPid);


/// <summary>
/// 隐藏模块
/// </summary>
/// <param name="hBase">待隐藏的模块基地址</param>
/// <param name="option">模块隐藏配置</param>
/// <returns></returns>
BOOL ConcealDLL(HMODULE hBase, Option option);

/// <summary>
/// 隐藏模块之断LoadOrderLinks链表
/// </summary>
/// <param name="hBase">待隐藏的模块基地址</param>
/// <returns></returns>
BOOL ConcealOnLoadOrderLinks(HMODULE hBase);

/// <summary>
/// 隐藏模块之断MemoryOrderLinks链表
/// </summary>
/// <param name="hBase">待隐藏的模块基地址</param>
/// <returns></returns>
BOOL ConcealOnMemoryOrderLinks(HMODULE hBase);

/// <summary>
/// 隐藏模块之断InitializationOrderLinks链表
/// </summary>
/// <param name="hBase"></param>
/// <returns></returns>
BOOL ConcealOnInitializationOrderLinks(HMODULE hBase);

#endif // !__CONCEAL_DLL_H_
