#pragma once

#include <ntddk.h>
#include <wdf.h>

DRIVER_INITIALIZE            DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD    DriverEvtDeviceAdd;