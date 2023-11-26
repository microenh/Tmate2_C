#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <hidapi/hidapi.h>

#include "tmate2.h"
#include "bitfields.h"
#include "layout.h"

#define VENDOR_ID 0x1721
#define PRODUCT_ID 0x0614
#define PRESS_CT 80;

void (*main_move)(int delta);
void (*e1_move)(int delta);
void (*e2_move)(int delta);
void (*main_tap)(void);
void (*e1_tap)(void);
void (*e2_tap)(void);
void (*main_press)(void);
void (*e1_press)(void);
void (*e2_press)(void);
void (*f1_tap)(void);
void (*f2_tap)(void);
void (*f3_tap)(void);
void (*f4_tap)(void);
void (*f5_tap)(void);
void (*f6_tap)(void);
void (*f1_press)(void);
void (*f2_press)(void);
void (*f3_press)(void);
void (*f4_press)(void);
void (*f5_press)(void);
void (*f6_press)(void);

static union write_buffer write_data;
static union read_buffer read_data;
static int main_encoder, e1_encoder, e2_encoder;
static bool btn_down[9];
static bool btn_press[9];
static int btn_ctr[9];
static hid_device *handle;

static int encoder_delta(int which, unsigned int current) {
    static unsigned int old_encoder_value[3];
    int o = old_encoder_value[which];
    if (current == o)
        return 0;
    old_encoder_value[which] = current;
    int delta = current - o;
    if (delta > 32768)
        delta -= 65536;
    else if (delta < -32768)
        delta += 65536;
    return delta;
}

bool init_tmate2(void) {
    handle = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    if (handle) {
        set_rgb(0xff, 0xff, 0xff);
        display_main_number(main_encoder);
        main_encoder_setup(1, 100, 1000, 32, 64, 10);
        write_data.w.hz = 1;

        // get initial encoder values
        hid_read(handle, read_data.d, 9);
        encoder_delta(0, read_data.r.main_encoder);
        encoder_delta(1, read_data.r.e1_encoder);
        encoder_delta(2, read_data.r.e2_encoder);
        return true;
    }
    return false;
}

void close_tmate2(void) {
    hid_close(handle);
    handle = NULL;
}

void update_main_digit(int which, char value) {
    int left_bits = main_layout[value - 32][0];
    int right_bits = main_layout[value - 32][1];
    switch (which) {
        case 0:
            write_data.w.main_1l = left_bits;
            write_data.w.main_1r = right_bits;
            break;
        case 1:
            write_data.w.main_2l = left_bits;
            write_data.w.main_2r = right_bits;
            break;
        case 2:
            write_data.w.main_3l = left_bits;
            write_data.w.main_3r = right_bits;
            break;
        case 3:
            write_data.w.main_4l = left_bits;
            write_data.w.main_4r = right_bits;
            break;
        case 4:
            write_data.w.main_5l = left_bits;
            write_data.w.main_5r = right_bits;
            break;
        case 5:
            write_data.w.main_6l = left_bits;
            write_data.w.main_6r = right_bits;
            break;
        case 6:
            write_data.w.main_7l = left_bits;
            write_data.w.main_7r = right_bits;
            break;
        case 7:
            write_data.w.main_8l = left_bits;
            write_data.w.main_8r = right_bits;
            break;
        case 8:
            write_data.w.main_9l = left_bits;
            write_data.w.main_9r = right_bits;
            break;
    }
}

void update_smeter_digit(int which, char value) {
    int left_bits = smeter_layout[value - 32][0];
    int right_bits = smeter_layout[value - 32][1];
    switch (which) {
        case 0:
            write_data.w.smeter_1l = left_bits;
            write_data.w.smeter_1r = right_bits;
            break;
        case 1:
            write_data.w.smeter_2l = left_bits;
            write_data.w.smeter_2r = right_bits;
            break;
        case 2:
            write_data.w.smeter_3l = left_bits;
            write_data.w.smeter_3r = right_bits;
            break;
    }
}

void display_main_number(int value) {
    bool blank = value > 999999999;
    update_main_digit(8, value < 0 && !blank ? '-' : ' ');
    value = abs(value);
    write_data.w.dot1 = value > 1000000 && !blank;
    write_data.w.dot2 = value > 1000 && !blank;
    for (int i=0; i<8; i++) {
        int number = value % 10;
        value /= 10;
        update_main_digit(i, number == 0 && value == 0 && i > 0 || blank ? ' ': number + '0');
    }
}

void display_smeter_number(int value) {
    // use 1000 to blank display
    bool blank = value > 999;
    write_data.w.smeter_db_minus = value < 0;
    value = abs(value);
    for (int i=0; i<8; i++) {
        int number = value % 10;
        value /= 10;
        update_smeter_digit(i, (number == 0 && value == 0 && i > 0) || blank ? ' ': number + '0');
    }
}

void set_rgb(int r, int g, int b) {
    write_data.w.rgb_red = r;
    write_data.w.rgb_green = g;
    write_data.w.rgb_blue = b;
}

void set_contrast(int c) {
    write_data.w.contrast = c;
}

void set_smeter_graph(bool on) {
    write_data.w.smeter_line = on;
    write_data.w.smeter_1 = on;
    write_data.w.smeter_3 = on;
    write_data.w.smeter_5 = on;
    write_data.w.smeter_7 = on;
    write_data.w.smeter_9 = on;
    write_data.w.smeter_10 = on;
    write_data.w.smeter_20 = on;
    write_data.w.smeter_40 = on;
    write_data.w.smeter_60 = on;
}

void set_smeter_s(bool on) {
    write_data.w.w = !on;
    write_data.w.s = on;
    write_data.w.smeter_plus20 = on;
    write_data.w.smeter_plus40 = on;
    write_data.w.smeter_plus60 = on;
}

void main_encoder_setup(int speed1, int speed2, int speed3, int trans12, int trans23, int trans_time) {
    write_data.w.encoder_speed1 = speed1;
    write_data.w.encoder_speed2 = speed2;
    write_data.w.encoder_speed3 = speed3;
    write_data.w.encoder_trans12 = trans12;
    write_data.w.encoder_trans23 = trans23;
    write_data.w.trans_time = trans_time;
}


void click(void) {
    write_data.w.click = 1;
    hid_write(handle, write_data.d, 44);
}

static int check_button(int which, bool state) {
    if (state) {
        if (btn_down[which]) {
            btn_ctr[which] = 0;
            btn_down[which] = false;
            if (btn_press[which]) {
                btn_press[which] = false;
                return 2;
            } else {
                return 1;
            }
        }
    } else {
        if (btn_down[which]) {
            if (!--btn_ctr[which]) {
                click();
                btn_press[which] = true;
            }
        } else {
            btn_down[which] = true;
            btn_ctr[which] = PRESS_CT;
        }
    }
    return 0;
}

void tmate2_tick(void) {
    hid_read(handle, read_data.d, 9);
    int delta = encoder_delta(0, read_data.r.main_encoder);
    if (delta && main_move)
        main_move(delta);

    delta = encoder_delta(1, read_data.r.e1_encoder);
    if (delta && e1_move)
        e1_move(delta);

    delta = encoder_delta(2, read_data.r.e2_encoder);
    if (delta && e2_move)
        e2_move(delta);

    int b_action = check_button(0, read_data.r.main_button);
    if (b_action == 1) {
        if (main_tap)
            main_tap();
    } else if (b_action == 2) {
        if (main_press)
            main_press();
    }

    b_action = check_button(1, read_data.r.e1_button);
    if (b_action == 1) {
        if (e1_tap)
            e1_tap();
    } else if (b_action == 2) {
        if (e1_press)
            e1_press();
    }

    b_action = check_button(2, read_data.r.e2_button);
    if (b_action == 1) {
        if (e2_tap)
            e2_tap();
    } else if (b_action == 2) {
        if (e2_press)
            e2_press();
    }

    b_action = check_button(3, read_data.r.f1_button);
    if (b_action == 1) {
        if (f1_tap)
            f1_tap();
    } else if (b_action == 2) {
        if (f1_press)
            f1_press();
    }

    b_action = check_button(4, read_data.r.f2_button);
    if (b_action == 1) {
        if (f2_tap)
            f2_tap();
    } else if (b_action == 2) {
        if (f2_press)
            f2_press();
    }

    b_action = check_button(5, read_data.r.f3_button);
    if (b_action == 1) {
        if (f3_tap)
            f3_tap();
    } else if (b_action == 2) {
        if (f3_press)
            f3_press();
    }

    b_action = check_button(6, read_data.r.f4_button);
    if (b_action == 1) {
        if (f4_tap)
            f4_tap();
    } else if (b_action == 2) {
        if (f4_press)
            f4_press();
    }

    b_action = check_button(7, read_data.r.f5_button);
    if (b_action == 1) {
        if (f5_tap)
            f5_tap();
    } else if (b_action == 2) {
        if (f5_press)
            f5_press();
    }

    b_action = check_button(8, read_data.r.f6_button);
    if (b_action == 1) {
        if (f6_tap)
            f6_tap();
    } else if (b_action == 2) {
        if (f6_press)
            f6_press();
    }

    hid_write(handle, write_data.d, 44);
}
