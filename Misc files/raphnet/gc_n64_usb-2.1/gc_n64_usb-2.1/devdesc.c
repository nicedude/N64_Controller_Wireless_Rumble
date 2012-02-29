#include "devdesc.h"
#include "usbconfig.h"

#define USBDESCR_DEVICE         1

const char usbDescrDevice[] PROGMEM = {    /* USB device descriptor */
    18,         /* sizeof(usbDescrDevice): length of descriptor in bytes */
    USBDESCR_DEVICE,    /* descriptor type */
    0x01, 0x01, /* USB version supported */
    USB_CFG_DEVICE_CLASS,
    USB_CFG_DEVICE_SUBCLASS,
    0,          /* protocol */
    8,          /* max packet size */
    USB_CFG_VENDOR_ID,  /* 2 bytes */
    USB_CFG_DEVICE_ID,  /* 2 bytes */
    USB_CFG_DEVICE_VERSION, /* 2 bytes */
#if USB_CFG_VENDOR_NAME_LEN
    1,          /* manufacturer string index */
#else
    0,          /* manufacturer string index */
#endif
#if USB_CFG_DEVICE_NAME_LEN
    2,          /* product string index */
#else
    0,          /* product string index */
#endif
#if USB_CFG_SERIAL_NUMBER_LENGTH
    3,          /* serial number string index */
#else
    0,          /* serial number string index */
#endif
    1,          /* number of configurations */
};

int getUsbDescrDevice_size(void) { return sizeof(usbDescrDevice); }

