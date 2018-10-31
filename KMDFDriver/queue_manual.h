#pragma once

#include <ntddk.h>
#include <wdf.h>

#include "device.h"
#include "hid.h"

typedef struct _QUEUE_MANUAL_CONTEXT {
    WDFDEVICE           device;
    WDFQUEUE            queue;
} QUEUE_MANUAL_CONTEXT, *PQUEUE_MANUAL_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_MANUAL_CONTEXT, QueueManualGetContext);

NTSTATUS
QueueManualCreate(
    _In_ WDFDEVICE device,
    _Out_ WDFQUEUE *queue
);

NTSTATUS
QueueManualSendReport(
    _In_ WDFREQUEST         outputRequest,
    _In_ PDEVICE_CONTEXT    deviceContext
);