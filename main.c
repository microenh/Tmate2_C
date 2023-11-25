#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <hidapi/hidapi.h>

#include "bitfields.h"
#include "layout.h"

#define VENDOR_ID 0x1721
#define PRODUCT_ID 0x0614

union write_buffer write_data;
union read_buffer read_data;

int main(int argc, char* argv[]) {
    const int index = 17;
    write_data.w.smeter_1l = smeter_layout[index][0];
    write_data.w.smeter_1r = smeter_layout[index][1];
    write_data.w.smeter_2l = smeter_layout[index+1][0];
    write_data.w.smeter_2r = smeter_layout[index+1][1];
    write_data.w.smeter_3l = smeter_layout[index+2][0];
    write_data.w.smeter_3r = smeter_layout[index+2][1];

    write_data.w.main_1l = main_layout[index][0];
    write_data.w.main_1r = main_layout[index][1];
    write_data.w.main_2l = main_layout[index+1][0];
    write_data.w.main_2r = main_layout[index+1][1];
    write_data.w.main_3l = main_layout[index+2][0];
    write_data.w.main_3r = main_layout[index+2][1];
    write_data.w.main_4l = main_layout[index+3][0];
    write_data.w.main_4r = main_layout[index+3][1];
    write_data.w.main_5l = main_layout[index+4][0];
    write_data.w.main_5r = main_layout[index+4][1];
    write_data.w.main_6l = main_layout[index+5][0];
    write_data.w.main_6r = main_layout[index+5][1];
    write_data.w.main_7l = main_layout[index+6][0];
    write_data.w.main_7r = main_layout[index+6][1];
    write_data.w.main_8l = main_layout[index+7][0];
    write_data.w.main_8r = main_layout[index+7][1];
    write_data.w.main_9l = main_layout[index+8][0];
    write_data.w.main_9r = main_layout[index+8][1];

    write_data.w.rgb_red = 0xff;
    write_data.w.rgb_green = 0xff;
    write_data.w.rgb_blue = 0xff;

    write_data.w.encoder_speed1 = 1;
    write_data.w.encoder_speed2 = 1;
    write_data.w.encoder_speed3 = 1;

    // write_data.w.click = 1;

    hid_device *handle = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    #define COUNTS 100
    int ctr = COUNTS;
    if (handle) {
        int main_value = 0, e1_value = 0, e2_value = 0;
        unsigned int old_main, old_e1, old_e2;
        unsigned int current;
        hid_read(handle, read_data.d, 9);
        old_main = read_data.r.main_encoder;
        old_e1 = read_data.r.e1_encoder;
        old_e2 = read_data.r.e2_encoder;
        while (1) {
            hid_read(handle, read_data.d, 9);
            current = read_data.r.main_encoder;
            if (current != old_main) {
                if (current > old_main)
                    main_value -= current - old_main;
                else
                    main_value += old_main - current;
                old_main = current;
                if (main_value > 30000000) 
                    main_value = 30000000;
                else if (main_value < 0)
                    main_value = 0;
                printf("%d\r\n", main_value);
            }
            current = read_data.r.e1_encoder;
            if (current != old_e1) {
                if (current > old_e1)
                    e1_value += current - old_e1;
                else
                    e1_value -= old_e1 - current;
                old_e1 = current;
            }
            current = read_data.r.e2_encoder;
            if (current != old_e2) {
                if (current > old_e2)
                    e2_value += current - old_e2;
                else
                    e2_value -= old_e2 - current;
                old_e2 = current;
            }


            if (!ctr--) {
                ctr = COUNTS;
                hid_write(handle, write_data.d, 44);
            }
            if (!read_data.r.main_button)
                break;
        }
        hid_close(handle);
        printf("Done\r\n");
    }
}