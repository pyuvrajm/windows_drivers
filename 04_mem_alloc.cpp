#include <ntddk.h>
#include <dontuse.h>

#define DRIVER_TAG 'ivuY'

// uninitialized global var - zero initialized - buffer pointer is also zero i.e. NULL
UNICODE_STRING g_RegistryPath;

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);
	ExFreePool(g_RegistryPath.Buffer);

	KdPrint(("SampleUnload called\n"));
}

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	DriverObject->DriverUnload = SampleUnload;
	
	KdPrint(("Inside DriverEntry\n"));
	KdPrint(("Original string: %wZ, length: %d\n", RegistryPath, RegistryPath->Length));

	g_RegistryPath.Buffer = (PWCH)ExAllocatePool2(POOL_FLAG_PAGED, RegistryPath->Length, DRIVER_TAG);
	if (NULL == g_RegistryPath.Buffer) {
		KdPrint(("ERROR - Failed to allocate buffer for registry path\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	g_RegistryPath.Length = RegistryPath->Length;
	g_RegistryPath.MaximumLength = RegistryPath->Length;
	
	RtlCopyUnicodeString(&g_RegistryPath, (PCUNICODE_STRING)RegistryPath);

	KdPrint(("Copied string: %wZ\n", &g_RegistryPath));

	return STATUS_SUCCESS;
}