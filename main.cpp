#include <windows.h>
#include <stdio.h>
#include "GOProcessUtils.h"

int main( int argc, char** argv )
{
	GOProcessUtils gpu;

	HANDLE CSHandle = gpu.findCSGoProcess();
	if( CSHandle == NULL )
	{
		printf( "Could not find GO");
		return 1;
	}

	DWORD clientHandle = gpu.findClientModule();
	if( clientHandle == 0 )
	{
		printf( "Could not find client base");
		return 1;
	}

	DWORD curTargetAddress, curTarget;
	
	ReadProcessMemory( CSHandle, (void*)(clientHandle + 0x00A6C434), &curTargetAddress, 4, NULL );
	curTargetAddress += 0x23AC;

	while( 1 )
	{
		ReadProcessMemory( CSHandle, (void*)curTargetAddress, &curTarget, 4, NULL );
		if( curTarget > 0 )
		{
			mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
			Sleep( 10 );
			mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
			Sleep( 10 );
		}

		Sleep( 100 );
	}

	return 0;
}