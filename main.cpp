/*!
*	A proof-of-concept that demonstrated using CSGO's internal nametags to trigger a shot.
*/

#include <windows.h>
#include <stdio.h>
#include "GOProcessUtils.h"

int main( int argc, char** argv )
{
	GOProcessUtils gpu;

	// Get a handle to csgo
	HANDLE CSHandle = gpu.findCSGoProcess();
	if( CSHandle == NULL )
	{
		printf( "Could not find GO");
		return 1;
	}

	// Find the base of client.dll
	DWORD clientHandle = gpu.findClientModule();
	if( clientHandle == 0 )
	{
		printf( "Could not find client base");
		return 1;
	}

	DWORD curTargetAddress, curTarget;
	
	// clientHandle + 0x00A6C434 holds the address of the player structure.
	ReadProcessMemory( CSHandle, (void*)(clientHandle + 0x00A6C434), &curTargetAddress, 4, NULL );
	// + 0x23ac points to the value that holds whether or not we are pointing at another player
	curTargetAddress += 0x23AC;

	// Constantly read the target address and if we are pointing at another player,
	// send a down then up mouse press.
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