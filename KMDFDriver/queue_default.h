#pragma once

#include <ntddk.h>
#include <wdf.h>

#include "device.h"

typedef struct _QUEUE_DEFAULT_CONTEXT {
    WDFDEVICE            device;
    WDFQUEUE             queue;
} QUEUE_DEFAULT_CONTEXT, *PQUEUE_DEFAULT_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_DEFAULT_CONTEXT, QueueDefaultGetContext);

NTSTATUS
QueueDefaultCreate(
    _In_ WDFDEVICE device,
    _Out_ WDFQUEUE *queue
);

EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL QueueDefaultEvtIoDeviceControl;