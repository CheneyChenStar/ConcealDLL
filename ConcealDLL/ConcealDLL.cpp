#include "ConcealDLL.h"


#ifdef _WIN64
    PPEB gPeb = (PPEB)__readgsqword(0x60);	// lea gPeb, qword ptr gs:[0x60]
#else
    PPEB gPeb = (PPEB)__readfsdword(0x30);	// lea gPeb, qword ptr fs[0x30]
#endif // !_WIN64


void PrintError(CONST TCHAR* msg) {
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError();
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, eNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        sysMsg, 256, NULL);

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while ((*p > 31) || (*p == 9))
        ++p;
    do { *p-- = 0; } while ((p >= sysMsg) &&
        ((*p == '.') || (*p < 33)));

    // Display the message
    _tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
}

BOOL ListProcessModules(DWORD dwPID) {
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;

    //  Take a snapshot of all modules in the specified process. 
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
        PrintError(TEXT("CreateToolhelp32Snapshot (of modules)"));
        return(FALSE);
    }

    //  Set the size of the structure before using it. 
    me32.dwSize = sizeof(MODULEENTRY32);

    //  Retrieve information about the first module, 
    //  and exit if unsuccessful 
    if (!Module32First(hModuleSnap, &me32))
    {
        PrintError(TEXT("Module32First"));  // Show cause of failure 
        CloseHandle(hModuleSnap);     // Must clean up the snapshot object! 
        return(FALSE);
    }

    //  Now walk the module list of the process, 
    //  and display information about each module 
    do
    {
        _tprintf(TEXT("\n\n     MODULE NAME:     %s"), me32.szModule);
        _tprintf(TEXT("\n     executable     = %s"), me32.szExePath);
        _tprintf(TEXT("\n     process ID     = 0x%08X"), me32.th32ProcessID);
        _tprintf(TEXT("\n     ref count (g)  =     0x%04X"), me32.GlblcntUsage);
        _tprintf(TEXT("\n     ref count (p)  =     0x%04X"), me32.ProccntUsage);
        _tprintf(TEXT("\n     base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
        _tprintf(TEXT("\n     base size      = %d"), me32.modBaseSize);

    } while (Module32Next(hModuleSnap, &me32));

    _tprintf(TEXT("\n"));

    //  Do not forget to clean up the snapshot object. 
    CloseHandle(hModuleSnap);
    return(TRUE);
}

BOOL ConcealOnLoadOrderLinks(HMODULE hBase) {
    
    BOOL ret = FALSE;
    PLIST_ENTRY pHead = 0 , pTail = 0;
    PLDR_DATA_TABLE_ENTRY pLdrDataTableEntry = NULL;

    pHead = pTail = gPeb->Ldr->InLoadOrderModuleList.Blink;
    do
    {
        pLdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)CONTAINING_RECORD(pHead, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

        if (pLdrDataTableEntry->DllBase == NULL) {
            break;
        }

        if (pLdrDataTableEntry->DllBase == hBase) {
            RemoveEntryList(pHead);
            ret = TRUE;
        }

        pHead = pHead->Blink;
    } while (pHead != pTail);

    return  ret;

}

BOOL ConcealOnMemoryOrderLinks(HMODULE hBase) {
    BOOL ret = FALSE;
    PLIST_ENTRY pHead = 0, pTail = 0;
    PLDR_DATA_TABLE_ENTRY pLdrDataTableEntry = NULL;

    pHead = pTail = gPeb->Ldr->InMemoryOrderModuleList.Blink;
    do
    {
        pLdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)CONTAINING_RECORD(pHead, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        
        if (pLdrDataTableEntry->DllBase == NULL) {
            break;
        }

        if (pLdrDataTableEntry->DllBase == hBase) {
            RemoveEntryList(pHead);
            ret = TRUE;
        }

        pHead = pHead->Blink;
    } while (pHead != pTail);

    return  ret;
}

BOOL ConcealOnInitializationOrderLinks(HMODULE hBase) {
    BOOL ret = FALSE;
    PLIST_ENTRY pHead = 0, pTail = 0;
    PLDR_DATA_TABLE_ENTRY pLdrDataTableEntry = NULL;

    pHead = pTail = gPeb->Ldr->InInitializationOrderModuleList.Blink;
    do
    {
        pLdrDataTableEntry = (PLDR_DATA_TABLE_ENTRY)CONTAINING_RECORD(pHead, LDR_DATA_TABLE_ENTRY, InInitializationOrderLinks);

        if (pLdrDataTableEntry->DllBase == NULL) {
            break;
        }

        if (pLdrDataTableEntry->DllBase == hBase) {
            RemoveEntryList(pHead);
            ret = TRUE;
        }

        pHead = pHead->Blink;
    } while (pHead != pTail);

    return  ret;
}

BOOL ConcealDLL(HMODULE hBase, Option option) {
    BOOL ret = FALSE;
    switch (option)
    {
    case LoadOrderLinks:
        ret = ConcealOnLoadOrderLinks(hBase);
        break;
    case MemoryOrderLinks:
        ret = ConcealOnMemoryOrderLinks(hBase);
        break;
    case InitializationOrderLinks:
        ret = ConcealOnInitializationOrderLinks(hBase);
        break;
    case ALL:
        ret = ConcealOnLoadOrderLinks(hBase);
        ret = ConcealOnMemoryOrderLinks(hBase);
        ret = ConcealOnInitializationOrderLinks(hBase);
        break;
    default:
        break;
    }
    return ret;
}