#pragma once

#include <ntddk.h>
#include <wdf.h>

NTSTATUS
CopyToRequestBuffer(
    _In_ WDFREQUEST       request,
    _In_ PVOID            sourceBuffer,
    _In_ size_t           numBytesToCopyFrom
);