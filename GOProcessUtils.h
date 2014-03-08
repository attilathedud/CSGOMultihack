#pragma once

#include <Windows.h>

class GOProcessUtils
{
public:
	GOProcessUtils(void);
	~GOProcessUtils(void);
	DWORD getClientBaseAddress(void);
	HANDLE getGoProcessHandle(void);
	HANDLE findCSGoProcess(void);
	DWORD findClientModule(void);
private:
	int clientdllBase;
	DWORD goProcessId;
	HANDLE goProcessHandle;
};

