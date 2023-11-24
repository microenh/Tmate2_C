#include <stdio.h>
#include <unistd.h>
#include <hidapi/hidapi.h>

#include "display_bitfield.h"
#include "layout.h"

#define VENDOR_ID 0x1721
#define PRODUCT_ID 0x0614

DISPLAY_BITFIELD data;

int main(int argc, char* argv[]) {
    const int index = 17;
    data.b.smeter_1l = smeter_layout[index][0];
    data.b.smeter_1r = smeter_layout[index][1];
    data.b.smeter_2l = smeter_layout[index+1][0];
    data.b.smeter_2r = smeter_layout[index+1][1];
    data.b.smeter_3l = smeter_layout[index+2][0];
    data.b.smeter_3r = smeter_layout[index+2][1];

    data.b.main_1l = main_layout[index][0];
    data.b.main_1r = main_layout[index][1];
    data.b.main_2l = main_layout[index+1][0];
    data.b.main_2r = main_layout[index+1][1];
    data.b.main_3l = main_layout[index+2][0];
    data.b.main_3r = main_layout[index+2][1];
    data.b.main_4l = main_layout[index+3][0];
    data.b.main_4r = main_layout[index+3][1];
    data.b.main_5l = main_layout[index+4][0];
    data.b.main_5r = main_layout[index+4][1];
    data.b.main_6l = main_layout[index+5][0];
    data.b.main_6r = main_layout[index+5][1];
    data.b.main_7l = main_layout[index+6][0];
    data.b.main_7r = main_layout[index+6][1];
    data.b.main_8l = main_layout[index+7][0];
    data.b.main_8r = main_layout[index+7][1];
    data.b.main_9l = main_layout[index+8][0];
    data.b.main_9r = main_layout[index+8][1];

    data.b.rgb_red = 0xff;
    data.b.rgb_green = 0xff;
    data.b.rgb_blue = 0xff;

    hid_device *handle;
    handle = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    if (handle) {
        for (int i=0; i<50; i++) {
            hid_write(handle, data.d, 44);
            usleep(100000);
        }
        hid_close(handle);
    }
}