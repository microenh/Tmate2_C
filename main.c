#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tmate2.h"

bool loop = true;

void cb_main_move(int delta) {
    static int main_encoder;

    main_encoder -= delta;
    if (main_encoder < 0)
        main_encoder = 0;
    else if (main_encoder > 30000000)
        main_encoder = 30000000;
    display_main_number(main_encoder);
}


void cb_e1_move(int delta) {}
void cb_e2_move(int delta) {}
void cb_main_tap(void) {}
void cb_main_press(void) {}
void cb_e1_tap(void) {}
void cb_e1_press(void) {}
void cb_e2_tap(void) {}
void cb_e2_press(void) {}
void cb_f1_tap(void) {}
void cb_f1_press(void) {}
void cb_f2_tap(void) {}
void cb_f2_press(void) {}
void cb_f3_tap(void) {}
void cb_f3_press(void) {}
void cb_f4_tap(void) {}
void cb_f4_press(void) {}
void cb_f5_tap(void) {}
void cb_f5_press(void) {}

void cb_f6_tap(void) {
    loop = false;
}

void cb_f6_press(void) {
    set_rgb(0, 0xff, 0);    
}


int main(int argc, char* argv[]) {
    main_move = &cb_main_move;
    f6_tap = &cb_f6_tap;
    f6_press = &cb_f6_press;

    if (init_tmate2()) {

        while (loop) {
            tmate2_tick();
            sleep(0.125);
        }
        close_tmate2();
    }
}