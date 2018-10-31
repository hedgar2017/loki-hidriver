#include "memory.h"

_Use_decl_annotations_
NTSTATUS
CopyToRequestBuffer(
    _In_ WDFREQUEST request,
    _In_ PVOID      sourceBuffer,
    _In_ size_t     numBytesToCopyFrom)
{
    NTSTATUS    status                = STATUS_SUCCESS;
    WDFMEMORY   memory                = NULL;
    size_t      outputBufferLength    = 0;

    if (numBytesToCopyFrom <= 0) {
        status = STATUS_INVALID_BUFFER_SIZE;
        return status;
    }

    status = WdfRequestRetrieveOutputMemory(request, &memory);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    WdfMemoryGetBuffer(memory, &outputBufferLength);
    if (outputBufferLength < numBytesToCopyFrom) {
        status = STATUS_INVALID_BUFFER_SIZE;
        return status;
    }

    status = WdfMemoryCopyFromBuffer(memory, 0, sourceBuffer, numBytesToCopyFrom);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    WdfRequestSetInformation(request, numBytesToCopyFrom);
    return status;
}