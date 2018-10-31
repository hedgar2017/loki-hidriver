#include "driver.h"
#include "device.h"

#include <hidport.h>

_Use_decl_annotations_
NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT     driverObject,
    _In_ PUNICODE_STRING    registryPath)
{
    NTSTATUS                status = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG       config;

    WDF_DRIVER_CONFIG_INIT(&config, DriverEvtDeviceAdd);

    status = WdfDriverCreate(driverObject, registryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);

    return status;
}

_Use_decl_annotations_
NTSTATUS
DriverEvtDeviceAdd(
    _In_ WDFDRIVER            driver,
    _Inout_ PWDFDEVICE_INIT   deviceInit)
{
    UNREFERENCED_PARAMETER(driver);
    return DeviceCreate(deviceInit);
}