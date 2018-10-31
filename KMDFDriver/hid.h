#pragma once

#include <ntddk.h>
#include <wdf.h>

#define REPORT_ID_MOUSE_INPUT       0x01
#define REPORT_ID_MOUSE_OUTPUT      0x02
#define REPORT_ID_KEYBOARD_INPUT    0x03
#define REPORT_ID_KEYBOARD_OUTPUT   0x04

#pragma pack(1)
typedef struct _HID_MOUSE_INPUT_REPORT {
    BYTE                reportId;
    BYTE                buttons;
    CHAR                x;
    CHAR                y;
} HID_MOUSE_INPUT_REPORT, *PHID_MOUSE_INPUT_REPORT;

typedef struct _HID_MOUSE_OUTPUT_REPORT {
    BYTE                reportId;
    BYTE                buttons;
    CHAR                x;
    CHAR                y;
} HID_MOUSE_OUTPUT_REPORT, *PHID_MOUSE_OUTPUT_REPORT;

typedef struct _HID_KEYBOARD_INPUT_REPORT {
    BYTE                reportId;
    BYTE                modifiers;
    BYTE                _reserved;
    BYTE                keyCodes[6];
} HID_KEYBOARD_INPUT_REPORT, *PHID_KEYBOARD_INPUT_REPORT;

typedef struct _HID_KEYBOARD_OUTPUT_REPORT {
    BYTE                reportId;
    BYTE                modifiers;
    BYTE                _reserved;
    BYTE                keyCodes[6];
} HID_KEYBOARD_OUTPUT_REPORT, *PHID_KEYBOARD_OUTPUT_REPORT;
#pragma pack()