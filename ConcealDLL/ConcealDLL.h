#pragma once

#ifndef __CONCEAL_DLL_H_
#define __CONCEAL_DLL_H_

/************************************************************************************************************************
*	ͷ�ļ�����
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
*	Ԥ��������
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
/// ��ӡ������Ϣ
/// </summary>
/// <param name="msg">error msg</param>
VOID PrintError(CONST TCHAR* msg);


/// <summary>
/// ��ӡ��ǰ��������ģ��
/// </summary>
BOOL ListProcessModules(DWORD dwPid);


/// <summary>
/// ����ģ��
/// </summary>
/// <param name="hBase">�����ص�ģ�����ַ</param>
/// <param name="option">ģ����������</param>
/// <returns></returns>
BOOL ConcealDLL(HMODULE hBase, Option option);

/// <summary>
/// ����ģ��֮��LoadOrderLinks����
/// </summary>
/// <param name="hBase">�����ص�ģ�����ַ</param>
/// <returns></returns>
BOOL ConcealOnLoadOrderLinks(HMODULE hBase);

/// <summary>
/// ����ģ��֮��MemoryOrderLinks����
/// </summary>
/// <param name="hBase">�����ص�ģ�����ַ</param>
/// <returns></returns>
BOOL ConcealOnMemoryOrderLinks(HMODULE hBase);

/// <summary>
/// ����ģ��֮��InitializationOrderLinks����
/// </summary>
/// <param name="hBase"></param>
/// <returns></returns>
BOOL ConcealOnInitializationOrderLinks(HMODULE hBase);

#endif // !__CONCEAL_DLL_H_
