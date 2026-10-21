#include <ntddk.h>

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);

	KdPrint(("Inside SampleUnload\n"));
}

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);

	DriverObject->DriverUnload = SampleUnload;
	KdPrint(("Inside DriverEntry\n"));

	OSVERSIONINFOW OsVersion;
	NTSTATUS ret = RtlGetVersion(&OsVersion);
	if (NT_SUCCESS(ret)) {
		KdPrint(("OS version info\n"));
		KdPrint(("You are on Windows OS version %lu.%lu.%lu\n", 
			OsVersion.dwMajorVersion, OsVersion.dwMinorVersion, 
			OsVersion.dwBuildNumber));
	}
	else {
		KdPrint(("Failed to get OS version due to error 0x%08X\n", ret));
	}

	return STATUS_SUCCESS;
}