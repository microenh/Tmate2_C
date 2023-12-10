#include <unistd.h>

#include "tmate2/tmate2.h"

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

void cb_f6_tap(void) {
    loop = false;
}

void cb_f6_press(void) {
    static bool state;
    state = !state;
    if (state)
        set_rgb(0, 0xff, 0);
    else
        set_rgb(0xff, 0xff, 0xff);
}

int main(int argc, char* argv[]) {
    set_main_move(cb_main_move);
    set_f6_tap(cb_f6_tap);
    set_f6_press(&cb_f6_press);

    if (init_tmate2()) {
        while (loop) {
            tmate2_tick();
            sleep(0.125);
        }
        close_tmate2();
    }
}