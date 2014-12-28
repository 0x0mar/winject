#include <iostream>

#include <Windows.h>
#include <TlHelp32.h>

int main(int argc, char* argv[])
{
        if (argc != 3)
        {
                return 1;
        }
        char* library = argv[1];
        char* process = argv[2];

	return 0;
}
