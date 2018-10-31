#include "queue_manual.h"
#include "memory.h"
#include "hid.h"

_Use_decl_annotations_
NTSTATUS
QueueManualCreate(
    _In_  WDFDEVICE device,
    _Out_ WDFQUEUE  *queueOut)
{
    NTSTATUS                status                  = STATUS_SUCCESS;
    WDFQUEUE                queue                   = NULL;
    PQUEUE_MANUAL_CONTEXT   queueContext            = NULL;

    WDF_IO_QUEUE_CONFIG     queueConfig;
    WDF_OBJECT_ATTRIBUTES   queueAttributes;

    WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&queueAttributes, QUEUE_MANUAL_CONTEXT);

    status = WdfIoQueueCreate(device, &queueConfig,    &queueAttributes, &queue);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    queueContext = QueueManualGetContext(queue);
    queueContext->device            = device;
    queueContext->queue             = queue;

    *queueOut                       = queue;
    return status;
}

_Use_decl_annotations_
NTSTATUS
QueueManualSendReport(
    _In_ WDFREQUEST         outputRequest,
    _In_ PDEVICE_CONTEXT    deviceContext)
{
    NTSTATUS                      status                       = STATUS_SUCCESS;
    WDFQUEUE                      queue                        = deviceContext->queueManual;
    size_t                        inputReportRequiredSize      = 0;

    WDFREQUEST                    inputRequest;
    WDF_REQUEST_PARAMETERS        outputRequestParams;
    HID_XFER_PACKET               hidXferPacket;
    
    WDF_REQUEST_PARAMETERS_INIT(&outputRequestParams);
    WdfRequestGetParameters(outputRequest, &outputRequestParams);
    if (outputRequestParams.Parameters.DeviceIoControl.InputBufferLength < sizeof(HID_XFER_PACKET)) {
        status = STATUS_BUFFER_TOO_SMALL;
        return status;
    }
    RtlCopyMemory(&hidXferPacket, WdfRequestWdmGetIrp(outputRequest)->UserBuffer, sizeof(HID_XFER_PACKET));

    switch (hidXferPacket.reportId) {
    case REPORT_ID_MOUSE_OUTPUT:
        hidXferPacket.reportId = REPORT_ID_MOUSE_INPUT;
        hidXferPacket.reportBuffer[0] = hidXferPacket.reportId;
        inputReportRequiredSize = sizeof(HID_MOUSE_INPUT_REPORT);
        break;
    case REPORT_ID_KEYBOARD_OUTPUT:
        hidXferPacket.reportId = REPORT_ID_KEYBOARD_INPUT;
        hidXferPacket.reportBuffer[0] = hidXferPacket.reportId;
        inputReportRequiredSize = sizeof(HID_KEYBOARD_INPUT_REPORT);
        break;
    default:
        status = STATUS_INVALID_PARAMETER;
        return status;
    }

    if (hidXferPacket.reportBufferLen < inputReportRequiredSize) {
        status = STATUS_INVALID_BUFFER_SIZE;
        return status;
    }

    status = WdfIoQueueRetrieveNextRequest(queue, &inputRequest);
    if (!NT_SUCCESS(status)) {
        return status;
    }
    status = CopyToRequestBuffer(inputRequest, hidXferPacket.reportBuffer, inputReportRequiredSize);

    WdfRequestComplete(inputRequest, status);
    return status;
}