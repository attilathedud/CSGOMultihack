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

/*!
*	Helper method that uses CreateToolhelp32Snapshot to get a listing of all loaded
*	libraries present in CSGO and then loops until it finds client.dll. 
*
*	Sets clientdllBase to the found address.
*
*	Returns: DWORD holding the address of client.dll.
*/
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

/*!
*	Helper method that uses CreateToolhelp32Snapshot to iterate through processes to find 
*	csgo. Once it finds it, it opens a handle with all access to the process.
*
*	Sets goProcessId to the process id of csgo.
*	Sets goProcessHandle to an open handle of csgo. When the class is deconstructed the handle
*	is closed.
*
*	Returns the handle to csgo.
*/
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