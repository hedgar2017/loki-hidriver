#include "queue_default.h"
#include "queue_manual.h"
#include "device.h"
#include "memory.h"

_Use_decl_annotations_
NTSTATUS
QueueDefaultCreate(
    _In_ WDFDEVICE device,
    _Out_ WDFQUEUE *queueOut)
{
    NTSTATUS                  status       = STATUS_SUCCESS;
    WDFQUEUE                  queue        = NULL;
    PQUEUE_DEFAULT_CONTEXT    context      = NULL;
    WDF_IO_QUEUE_CONFIG       config;
    WDF_OBJECT_ATTRIBUTES     attributes;

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&config, WdfIoQueueDispatchSequential);
    config.EvtIoInternalDeviceControl = QueueDefaultEvtIoDeviceControl;

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, QUEUE_DEFAULT_CONTEXT);

    status = WdfIoQueueCreate(device, &config, &attributes, &queue);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    context = QueueDefaultGetContext(queue);
    context->device      = device;
    context->queue       = queue;
    *queueOut            = queue;

    return status;
}

_Use_decl_annotations_
VOID
QueueDefaultEvtIoDeviceControl(
    _In_ WDFQUEUE   queue,
    _In_ WDFREQUEST request,
    _In_ size_t     outputBufferLength,
    _In_ size_t     inputBufferLength,
    _In_ ULONG      ioControlCode)
{
    UNREFERENCED_PARAMETER(outputBufferLength);
    UNREFERENCED_PARAMETER(inputBufferLength);

    NTSTATUS                status               = STATUS_SUCCESS;
    PDEVICE_CONTEXT         deviceContext        = DeviceGetContext(QueueDefaultGetContext(queue)->device);

    switch (ioControlCode) {
    case IOCTL_HID_GET_DEVICE_DESCRIPTOR:
        status = CopyToRequestBuffer(
            request,
            deviceContext->hidDescriptor,
            deviceContext->hidDescriptor->bLength
        );
        WdfRequestComplete(request, status);
        break;
    case IOCTL_HID_GET_DEVICE_ATTRIBUTES:
        status = CopyToRequestBuffer(
            request,
            &deviceContext->hidDeviceAttributes,
            sizeof(HID_DEVICE_ATTRIBUTES)
        );
        WdfRequestComplete(request, status);
        break;
    case IOCTL_HID_GET_REPORT_DESCRIPTOR:
        status = CopyToRequestBuffer(
            request,
            deviceContext->hidReportDescriptor,
            deviceContext->hidDescriptor->DescriptorList[0].wReportLength
        );
        WdfRequestComplete(request, status);
        break;
    case IOCTL_HID_READ_REPORT:
    case IOCTL_HID_GET_INPUT_REPORT:
        status = WdfRequestForwardToIoQueue(
            request,
            deviceContext->queueManual
        );
        if (!NT_SUCCESS(status)) {
            WdfRequestComplete(request, status);
        }
        break;
    case IOCTL_HID_WRITE_REPORT:
    case IOCTL_HID_SET_OUTPUT_REPORT:
        status = QueueManualSendReport(
            request,
            deviceContext
        );
        WdfRequestComplete(request, status);
        break;
    default:
        status = STATUS_NOT_IMPLEMENTED;
        WdfRequestComplete(request, status);
        break;
    }
}