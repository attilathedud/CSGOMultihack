#include <windows.h>
#include <stdio.h>

int main( int argc, char** argv )
{
	DWORD pID = 0;
	HANDLE process = 0;
	DWORD curTarget = 0;

	GetWindowThreadProcessId(FindWindow( NULL, L"Counter-Strike: Global Offensive" ), &pID );

	process = OpenProcess(PROCESS_VM_READ, true, pID );

	if( !process )
		printf( "Can't find go" );

	Sleep( 5000 );

	while( 1 )
	{
		ReadProcessMemory( process, (void*)0x127a8654, &curTarget, 4, NULL );
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