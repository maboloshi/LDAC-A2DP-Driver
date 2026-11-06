// LDAC A2DP Windows Driver Header
// Copyright (c) 2024 LDAC Driver Team
// Licensed under MIT License

#pragma once

#include <ntddk.h>
#include <wdf.h>

// LDAC codec definitions
#define LDAC_CODEC_ID           0x2D
#define LDAC_MAX_BITRATE        990000
#define LDAC_MID_BITRATE        660000
#define LDAC_MIN_BITRATE        330000

// Bitrates used by the negotiation/configuration code
#define LDAC_BITRATE_990        990000
#define LDAC_BITRATE_660        660000
#define LDAC_BITRATE_330        330000

// Registry keys for codec management
#define LDAC_REGISTRY_KEY       L"SOFTWARE\\LDACDriver"
#define LDAC_ENABLED_VALUE      L"LDACEnabled"
#define LDAC_BITRATE_VALUE      L"PreferredBitrate"
#define LDAC_ADAPTIVE_VALUE     L"AdaptiveBitrate"

// IOCTL interface
#define IOCTL_LDAC_SET_BITRATE  CTL_CODE(FILE_DEVICE_BLUETOOTH, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_LDAC_GET_STATUS   CTL_CODE(FILE_DEVICE_BLUETOOTH, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Codec identifiers reported back through IOCTL_LDAC_GET_STATUS
typedef enum _LDAC_CODEC_TYPE {
    CodecTypeUnknown = 0,
    CodecTypeLDAC    = 1
} LDAC_CODEC_TYPE;

// LDAC codec configuration
typedef struct _LDAC_CODEC_CONFIG {
    unsigned int  Type;
    unsigned int  Bitrate;
    unsigned int  SamplingFreq;
    unsigned char ChannelMode;
    unsigned char BitDepth;
} LDAC_CODEC_CONFIG, *PLDAC_CODEC_CONFIG;

// Per-device context
typedef struct _LDAC_DEVICE_EXTENSION {
    WDFDEVICE   Device;
    WDFQUEUE    DefaultQueue;
    WDFQUEUE    ManualQueue;
    WDFSPINLOCK QueueLock;
    WDFTIMER    CodecTimer;
    ULONG       CurrentBitrate;
    BOOLEAN     IsLDACEnabled;
    BOOLEAN     IsExclusiveMode;
} LDAC_DEVICE_EXTENSION, *PLDAC_DEVICE_EXTENSION;

WDF_DECLARE_CONTEXT_TYPE(LDAC_DEVICE_EXTENSION);

// Callbacks wired up in EvtDeviceAdd (defined in LDACDriver.c)
NTSTATUS EvtDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

VOID EvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    );

VOID EvtIoRead(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length
    );

VOID EvtIoWrite(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length
    );

// Helper routines defined in LDACDriver.c
PLDAC_DEVICE_EXTENSION LDACGetDeviceExtension(
    _In_ WDFDEVICE Device
    );

NTSTATUS LDACReadConfiguration(
    _In_ PLDAC_DEVICE_EXTENSION DeviceExtension
    );

VOID LDACLogEvent(
    _In_ WDFDEVICE Device,
    _In_ NTSTATUS Status,
    _In_ PCWSTR Message
    );

BOOLEAN LDACIsDeviceSupported(
    _In_ WDFDEVICE Device
    );

NTSTATUS LDACConfigureBitrate(
    _In_ PLDAC_DEVICE_EXTENSION DeviceExtension,
    _In_ ULONG TargetBitrate
    );

NTSTATUS LDACNegotiateCodec(
    _In_ PLDAC_DEVICE_EXTENSION DeviceExtension,
    _In_ PVOID CodecCapabilities,
    _In_ ULONG CapabilitiesLength
    );
