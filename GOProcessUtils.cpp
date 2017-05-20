#include "GOProcessUtils.h"
#include <tlhelp32.h>

GOProcessUtils::GOProcessUtils(void)
{
	clientdllBase = 0;
	goProcessHandle = NULL;
	goProcessId = 0;
}


GOProcessUtils::~GOProcessUtils(void)
{
	if( goProcessHandle != NULL )
		CloseHandle( goProcessHandle );
}


DWORD GOProcessUtils::getClientBaseAddress(void)
{
	return clientdllBase;
}


HANDLE GOProcessUtils::getGoProcessHandle(void)
{
	return goProcessHandle;
}

DWORD GOProcessUtils::findClientModule(void)
{
	HANDLE snapshot;
	MODULEENTRY32 me32;

	me32.dwSize = sizeof( MODULEENTRY32 );

	if( goProcessId > 0 )
	{
		snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, goProcessId );
		if( snapshot == INVALID_HANDLE_VALUE )
		{
			return NULL;
		}
	}

	if( !Module32First( snapshot, &me32 ) )
	{
		CloseHandle( snapshot );          
		return NULL;
	}

	do
	{
		if( wcscmp( me32.szModule, L"client.dll" ) == 0 )
		{
			clientdllBase = (DWORD) me32.modBaseAddr;
		}
	} while( Module32Next( snapshot, &me32 ) );

	CloseHandle( snapshot );

	return clientdllBase;
}

HANDLE GOProcessUtils::findCSGoProcess(void)
{
	HANDLE snapshot;
	PROCESSENTRY32 pe32;

	pe32.dwSize = sizeof( PROCESSENTRY32 );

	snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( snapshot == NULL )
	{
		return NULL;
	}

	if( !Process32First( snapshot, &pe32 ) )
	{
		CloseHandle( snapshot );          
		return NULL;
	}

	do
	{
		if( wcscmp( pe32.szExeFile, L"csgo.exe" ) == 0 )
		{
			goProcessId = pe32.th32ProcessID;
			goProcessHandle = OpenProcess( PROCESS_ALL_ACCESS, false, goProcessId );
		}
	} while( Process32Next( snapshot, &pe32 ) );

	CloseHandle( snapshot );

	return goProcessHandle;
}