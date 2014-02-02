#include <iostream>
#include <Windows.h>


int main(int argc, char **argv)
{
	//simple sanity check
	if (argc != 3) {
		printf("Usage: Injector.exe pid path_do_dll \n");
		printf("pid - target process ID to inject \n");
		printf("path_do_dll - full path to .dll file\n");
		return 0;
	}

	//debug info to verify parameters
	printf("Injecting \"%s\" to process ID: %s. \n", argv[2], argv[1]);

	// retrieve a handle to target process
	DWORD processId = atoi(argv[1]);
	HANDLE hProcessHandle =  ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (!hProcessHandle) {
		printf("ERROR: Couldn't obtain process handle.");
		return -1;
	}

	//allocate some memory in target process
	LPVOID libraryPath =  ::VirtualAllocEx( hProcessHandle, NULL, sizeof(argv[2]), MEM_COMMIT, PAGE_READWRITE ); 
	
	//write path to the .dll file to the target process
	::WriteProcessMemory( hProcessHandle, libraryPath, (void*)argv[2], sizeof(argv[2]), NULL );

	HMODULE kernelModule = ::GetModuleHandle("Kernel32");
	HANDLE targetThread = ::CreateRemoteThread( hProcessHandle, NULL, 0, 
		(LPTHREAD_START_ROUTINE)::GetProcAddress( kernelModule, "LoadLibraryA" ), libraryPath, 0, NULL );

	//wait for a target thread to finish it's execution
	::WaitForSingleObject( targetThread, INFINITE );


	// Get handle of the loaded module
	DWORD targetThreadResult = 0; 
	::GetExitCodeThread( targetThread, &targetThreadResult );

	// Clean up
	::CloseHandle( hProcessHandle );
	::VirtualFreeEx( hProcessHandle, libraryPath, sizeof(sizeof(argv[2])), MEM_RELEASE );

}