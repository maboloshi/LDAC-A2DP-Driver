// WPP tracing configuration for the LDAC driver.
// Referenced from LDACDriver.vcxproj via WppScanConfigurationData.

#define WPP_CONTROL_GUIDS                                                     \
    WPP_DEFINE_CONTROL_GUID(                                                  \
        LDACDriverTraceGuid,                                                  \
        (3f2a1b4c, 5d6e, 4f70, 8a91, b2c3d4e5f607),                           \
        WPP_DEFINE_BIT(TRACE_DRIVER)                                          \
        WPP_DEFINE_BIT(TRACE_DEVICE)                                          \
        WPP_DEFINE_BIT(TRACE_QUEUE)                                           \
        )

// begin_wpp config
// end_wpp
