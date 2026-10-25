// This is a user mode program.
// It opens the DEVICE_OBJECT for Process Explorer driver
// This program demonstrates that you can open the any Device for its corresponding Driver by
// having its symbolic name (which is found using WinObj sysinternals tool).

#include <iostream>
#include <Windows.h>

int main(void) {
	// The WinObj tool shows that the symbolic name for the Device of Process Explorer Driver
	// is PROCEXP152 and it points to /Device/PROCEXP152
	HANDLE hDevice = CreateFile(LR"(\\.\PROCEXP152)", GENERIC_READ,
		0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (INVALID_HANDLE_VALUE != hDevice) {
		printf_s("The Process Explorer Device opened successfully!");
		CloseHandle(hDevice);
	}
	else {
		DWORD err = GetLastError();
		printf_s("Encountered error %d while opening Process Explorer Device.", err);
		return err;
	}

	return 0;
}

// This program haven't worked always returned error 5 (ERROR_ACCESS_DENIED).