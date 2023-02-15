#include "ConcealDLL.h"
#include <stdio.h>

int main()
{
	LoadLibrary(TEXT("TestDLL.dll"));
	
	HMODULE hBase = GetModuleHandle(TEXT("TestDLL.dll"));
	DWORD dwPID = GetCurrentProcessId();
	Option opt = ALL;

	ListProcessModules(dwPID);
	ConcealDLL(hBase, opt);
	puts("----------------------------------------------------");
	ListProcessModules(dwPID);

	LPVOID             lpSystemInfo = NULL;
	DWORD				dwNumberBytes = 0;
	DWORD				dwReturnLength = 0;
	
	NtQuerySystemInformation(SystemPathInformation, lpSystemInfo, dwNumberBytes, &dwReturnLength);
	return 0;
}