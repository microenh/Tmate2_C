#pragma once

#include <stdbool.h>

bool init_tmate2(void);
void close_tmate2(void);
void tmate2_tick();

extern void (*main_move)(int delta);
extern void (*e1_move)(int delta);
extern void (*e2_move)(int delta);

extern void (*main_tap)(void);
extern void (*e1_tap)(void);
extern void (*e2_tap)(void);
extern void (*main_press)(void);
extern void (*e1_press)(void);
extern void (*e2_press)(void);
extern void (*f1_tap)(void);
extern void (*f2_tap)(void);
extern void (*f3_tap)(void);
extern void (*f4_tap)(void);
extern void (*f5_tap)(void);
extern void (*f6_tap)(void);
extern void (*f1_press)(void);
extern void (*f2_press)(void);
extern void (*f3_press)(void);
extern void (*f4_press)(void);
extern void (*f5_press)(void);
extern void (*f6_press)(void);

void update_main_digit(int which, char value);
void update_smeter_digit(int which, char value);
void display_main_number(int value);
void display_smeter_number(int value);
void set_rgb(int r, int g, int b);
void set_contrast(int c);
void set_smeter_graph(bool on);
void set_smeter_s(bool on);
void main_encoder_setup(int speed1, int speed2, int speed3, int trans12, int trans23, int trans_time);
void click(void);
