// This program plays an array of beeps for an array of specific durations
// to play a simple tune.
// This program uses beep driver with its exact device name using native API.

#pragma comment(lib, "ntdll")

// Include C or C++ standard headers first
#include<stdio.h>

// Then include basic windows headers
#include <Windows.h>
#include <winternl.h>

// Lastly, include other windows headers
#include <ntddbeep.h>

int main(/*int argc, char* argv[]*/) {
	/*ANSI_STRING tmp;
	UNICODE_STRING filePath;
	if (argc > 1) {
		RtlInitAnsiString(&tmp, argv[1]);
		NTSTATUS status = RtlAnsiStringToUnicodeString(&filePath, &tmp, TRUE);
		if (!NT_SUCCESS(status)) {
			printf_s("ERROR!! Failed to convert to unicode string - %d (0x%08X)", status, status);
			return status;
		}
	}*/
	 
	// Twinkle twinkle little star
	const int ArraySize = 14;
	int frequencies[ArraySize]{ 262, 262, 392, 392, 440, 440, 392, 349, 349, 330, 330, 294, 294, 262 };
	int durations[ArraySize]{ 500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 500, 500, 1000 };

	/*
	// You are my sunshine
	const int ArraySize = 33;
	int frequencies[ArraySize] =
	{ 261, 329, 392, 392, 392, 392, 329, 392, 261, 261,		// You are my sunshine, my only sunshine
			261, 329, 392, 440, 440, 392, 329, 261,			// You make me happy when skies are gray
			261, 329, 392, 440, 440, 392, 329, 261, 261,	// You'll never know, dear, how much I love you
			329, 349, 392, 329, 261, 261					// Please don't take my sunshine away
	};
	int durations[ArraySize] =
	{ 300, 300, 600, 300, 300, 600, 300, 300, 600, 600,
	300, 300, 600, 600, 300, 300, 600, 600,
	300, 300, 600, 600, 300, 300, 600, 300, 300,
	300, 300, 600, 600, 1200, 600
	};
	*/
	/*
	// Tujhe dekha toh
	const int ArraySize = 18;
	int frequencies[ArraySize] =
	{ 659, 659, 659, 988, 880, 784, 880, 1047, 988,
	659, 659, 659, 988, 880, 784, 880, 784, 740 };
	int durations[ArraySize] =
	{ 300, 300, 500, 500, 300, 300, 300, 400, 800,
	300, 300, 500, 500, 300, 300, 300, 300, 800 };
	*/

	UNICODE_STRING deviceName;
	RtlInitUnicodeString(&deviceName, DD_BEEP_DEVICE_NAME_U);
	OBJECT_ATTRIBUTES attr;
	InitializeObjectAttributes(&attr, &deviceName, OBJ_CASE_INSENSITIVE, nullptr, nullptr);
	HANDLE hFile;
	IO_STATUS_BLOCK ioStatus;
	NTSTATUS status = ::NtOpenFile(&hFile, GENERIC_WRITE, &attr, &ioStatus, 0, 0);
	if (!NT_SUCCESS(status)) {
		printf_s("ERROR!! Failed to open device file - %d (0x%08X)\n", status, status);
		return status;
	}

	BEEP_SET_PARAMETERS params;
	for (int i{ 0 }; i < ArraySize; ++i)
	{
		//printf_s("Playing frequency %d for %d ms\n", frequencies[i], durations[i]);

		params.Frequency = frequencies[i];
		params.Duration = durations[i];

		DWORD bytes;
		::DeviceIoControl(hFile, IOCTL_BEEP_SET, &params, sizeof(params), nullptr, 0, &bytes, nullptr);
	}

	::Sleep(1000);
	::CloseHandle(hFile);
	//RtlFreeUnicodeString(&filePath);

	return 0;
}