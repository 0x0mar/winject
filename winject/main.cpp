#include <iostream>

#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#include <TlHelp32.h>

typedef HINSTANCE(*fpLoadLibrary)(char*);

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 3)
	{
		return 1;
	}

	_TCHAR* library = argv[1];
	_TCHAR* process = argv[2];

	if (PathIsRelative(library)) {
		DWORD length = GetCurrentDirectory(0, NULL);
		library = new _TCHAR[length + _tcslen(argv[1], )];
		GetCurrentDirectory(length, library);
		PathAppend(library, argv[1]);
	}

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	DWORD pid = NULL;

	if (!Process32First(snapshot, &entry))
	{
		return 1;
	}

	do
	{
		if (_tcscmp(entry.szExeFile, process))
		{
			pid = entry.th32ProcessID;
			break;
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);

	HANDLE hProcess;

	HINSTANCE hDll = LoadLibraryA("KERNEL32");

	fpLoadLibrary LoadLibraryAddr = (fpLoadLibrary)GetProcAddress(hDll, "LoadLibraryA");

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

	LPVOID paramAddr = VirtualAllocEx(hProcess, 0, _tcslen(library) + 1, MEM_COMMIT, PAGE_READWRITE);
	BOOL memoryWritten = WriteProcessMemory(hProcess, paramAddr, library, _tcslen(library) + 1, NULL);

	CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryAddr, paramAddr, 0, 0);

	CloseHandle(hProcess);
	delete[] library;
	return 0;
}
