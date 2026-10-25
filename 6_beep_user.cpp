// This program shows how to open a driver's DEVICE_OBJECT directly
// using it's name and without using any symbolic link to the device name.
// This is achieved using native APIs (defined in NtDll.dll)

#pragma comment(lib, "ntdll")

#include <stdio.h>

// Core Windows headers must always come first
#include <Windows.h>
#include <winternl.h>

// WDK shared headers come after Windows.h
#include <ntddbeep.h>

int main(int argc, char* argv[]) {
	ULONG frequency = 800;	// any frequency
	ULONG duration = 1000;	// 1 second
	if (argc > 2) {
		frequency = atoi(argv[1]);
		duration = atoi(argv[2]);
	}
	else {
		printf_s("Error!! Please provide two values while executing program - \
			frequency & duration in millisecond\n");
		SetLastError(E_INVALIDARG);
		return E_INVALIDARG;
	}

	HANDLE hFile;
	OBJECT_ATTRIBUTES attr;
	UNICODE_STRING name;
	RtlInitUnicodeString(&name, DD_BEEP_DEVICE_NAME_U);
	InitializeObjectAttributes(&attr, &name, OBJ_CASE_INSENSITIVE, nullptr, nullptr);
	
	IO_STATUS_BLOCK ioStatus;
	NTSTATUS status = ::NtOpenFile(&hFile, GENERIC_WRITE, &attr, &ioStatus, 0, 0);
	if (!NT_SUCCESS(status)) {
		DWORD err = GetLastError();
		printf_s("ERROR!! Failed to open device file - %d (0x%08X)\n", err, err);
		return err;
	}

	BEEP_SET_PARAMETERS params;
	params.Frequency = frequency;
	params.Duration = duration;

	DWORD bytes;
	printf_s("Playing frequency %d for %d ms\n", frequency, duration);
	::DeviceIoControl(hFile, IOCTL_BEEP_SET, &params, sizeof(params), nullptr, 0, &bytes, nullptr);
	::Sleep(duration);
	
	::CloseHandle(hFile);

	return 0;
}
