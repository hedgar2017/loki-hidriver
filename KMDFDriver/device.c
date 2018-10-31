#include "device.h"
#include "queue_default.h"
#include "queue_manual.h"

HID_REPORT_DESCRIPTOR g_reportDescriptor[] = {
    0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,        // USAGE (Mouse)
    0xA1, 0x01,        // COLLECTION (Application)
    0x85,                  REPORT_ID_MOUSE_INPUT,
    0x09, 0x01,            // USAGE_PAGE (Pointer)
    0xA1, 0x00,            // COLLECTION (Physical)
    0x05, 0x09,                // USAGE_PAGE (Buttons)
    0x19, 0x01,                // USAGE_MINIMUM (1)
    0x29, 0x03,                // USAGE_MAXIMUM (3)
    0x15, 0x00,                // LOGICAL_MINIMUM (0)
    0x25, 0x01,                // LOGICAL_MAXIMUM (1)
    0x95, 0x03,                // REPORT_COUNT (3)
    0x75, 0x01,                // REPORT_SIZE (1)
    0x81, 0x02,                // INPUT (Data, Variable, Absolute)
    0x95, 0x01,                // REPORT_COUNT (1)
    0x75, 0x05,                // REPORT_SIZE (5)
    0x81, 0x01,                // INPUT (Constant)
    0x05, 0x01,                // USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                // USAGE (X)
    0x09, 0x31,                // USAGE (Y)
    0x15, 0x81,                // LOGICAL_MINIMUM (-127)
    0x25, 0x7F,                // LOGICAL_MAXIMUM (127)
    0x75, 0x08,                // REPORT_SIZE (8)
    0x95, 0x02,                // REPORT_COUNT (2)
    0x81, 0x06,                // Input (Data, Variable, Relative)
    0xC0,                  // END_COLLECTION
    0xC0,              // END_COLLECTION

    0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
    0x09, 0x00,        // USAGE (Undefined)
    0xa1, 0x01,        // COLLECTION (Application)
    0x85,                  REPORT_ID_MOUSE_OUTPUT,
    0x09, 0x00,            // USAGE (Undefined)
    0x15, 0x00,            // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,      // LOGICAL_MAXIMUM (255)
    0x95, 0x03,            // REPORT_COUNT (3)
    0x75, 0x08,            // REPORT_SIZE (8)
    0x91, 0x02,            // OUTPUT (Data, Variable, Absolute)
    0xc0,              // END_COLLECTION

    0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,        // USAGE (Keyboard)
    0xA1, 0x01,        // COLLECTION (Application)
    0x85,                  REPORT_ID_KEYBOARD_INPUT,
    0x05, 0x07,            // USAGE_PAGE (Keyboard Key Codes)
    0x19, 0xE0,            // USAGE_MINIMUM (224)
    0x29, 0xE7,            // USAGE_MAXIMUM (231)
    0x15, 0x00,            // LOGICAL_MINIMUM (0)
    0x25, 0x01,            // LOGICAL_MAXIMUM (1)
    0x75, 0x01,            // REPORT_SIZE (1)
    0x95, 0x08,            // REPORT_COUNT (8)
    0x81, 0x02,            // INPUT (Data, Variable, Absolute)
    0x95, 0x01,            // REPORT_COUNT (1)
    0x75, 0x08,            // REPORT_SIZE (8)
    0x81, 0x01,            // INPUT (Constant)
    0x19, 0x00,            // USAGE_MINIMUM (0)
    0x29, 0x65,            // USAGE_MAXIMUM (101)
    0x15, 0x00,            // LOGICAL_MINIMUM (0)
    0x25, 0x65,            // LOGICAL_MAXIMUM (101)
    0x95, 0x06,            // REPORT_COUNT (6)
    0x75, 0x08,            // REPORT_SIZE (8)
    0x81, 0x00,            // INPUT (Data, Array, Absolute)
    0x05, 0x08,            // USAGE_PAGE (LEDs)
    0x19, 0x01,            // USAGE_MINIMUM (Num Lock)
    0x29, 0x05,            // USAGE_MAXIMUM (Kana)
    0x95, 0x05,            // REPORT_COUNT (5)
    0x75, 0x01,            // REPORT_SIZE (1)
    0x91, 0x02,            // OUTPUT (Data, Variable, Absolute)
    0x95, 0x01,            // REPORT_COUNT (1)
    0x75, 0x03,            // REPORT_SIZE (3)
    0x91, 0x01,            // OUTPUT (Constant)
    0xC0,              // END_COLLECTION

    0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
    0x09, 0x00,        // USAGE (Undefined)
    0xa1, 0x01,        // COLLECTION (Application)
    0x85,                  REPORT_ID_KEYBOARD_OUTPUT,
    0x09, 0x00,            // USAGE (Undefined)
    0x15, 0x00,            // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,      // LOGICAL_MAXIMUM (255)
    0x95, 0x08,            // REPORT_COUNT (8)
    0x75, 0x08,            // REPORT_SIZE (8)
    0x91, 0x02,            // OUTPUT (Data, Variable, Absolute)
    0xc0               // END_COLLECTION
};

HID_DESCRIPTOR g_hidDescriptor = {
    0x09,        // length of HID descriptor
    0x21,        // descriptor type == HID 0x21
    0x0100,      // hid spec release
    0x00,        // country code == Not Specified
    0x01,        // number of HID class descriptors
    {            // DescriptorList[0]
        0x22,                             // report descriptor type 0x22
        sizeof(g_reportDescriptor)        // total length of report descriptor
    }
};

_Use_decl_annotations_
NTSTATUS
DeviceCreate(
    _Inout_ PWDFDEVICE_INIT deviceInit)
{
    NTSTATUS                status               = STATUS_SUCCESS;
    WDFDEVICE               device               = NULL;
    PDEVICE_CONTEXT         deviceContext        = NULL;
    WDF_OBJECT_ATTRIBUTES   deviceAttributes;

    WdfFdoInitSetFilter(deviceInit);

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&deviceInit, &deviceAttributes, &device);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    deviceContext = DeviceGetContext(device);
    deviceContext->device                               = device;
    deviceContext->queueDefault                         = NULL;
    deviceContext->queueManual                          = NULL;
    deviceContext->hidDescriptor                        = &g_hidDescriptor;
    deviceContext->hidReportDescriptor                  = g_reportDescriptor;

    RtlZeroMemory(&deviceContext->hidDeviceAttributes, sizeof(HID_DEVICE_ATTRIBUTES));
    deviceContext->hidDeviceAttributes.Size             = sizeof(HID_DEVICE_ATTRIBUTES);
    deviceContext->hidDeviceAttributes.VendorID         = VENDOR_ID;
    deviceContext->hidDeviceAttributes.ProductID        = PRODUCT_ID;
    deviceContext->hidDeviceAttributes.VersionNumber    = VERSION_NUMBER;

    status = QueueDefaultCreate(device, &deviceContext->queueDefault);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = QueueManualCreate(device, &deviceContext->queueManual);

    return status;
}