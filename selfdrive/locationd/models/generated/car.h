#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_4964988638861534016);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1640007944371605601);
void car_H_mod_fun(double *state, double *out_728519293432710064);
void car_f_fun(double *state, double dt, double *out_2854727407156294866);
void car_F_fun(double *state, double dt, double *out_2914726948904805167);
void car_h_25(double *state, double *unused, double *out_6142951501910734792);
void car_H_25(double *state, double *unused, double *out_7192818739541149741);
void car_h_24(double *state, double *unused, double *out_8358013461846682543);
void car_H_24(double *state, double *unused, double *out_5020169140535650175);
void car_h_30(double *state, double *unused, double *out_3663915095080274940);
void car_H_30(double *state, double *unused, double *out_2665122409413541543);
void car_h_26(double *state, double *unused, double *out_1337758172580570705);
void car_H_26(double *state, double *unused, double *out_3451315420667093517);
void car_h_27(double *state, double *unused, double *out_2574885417957957534);
void car_H_27(double *state, double *unused, double *out_490359097613116632);
void car_h_29(double *state, double *unused, double *out_5368077147829101202);
void car_H_29(double *state, double *unused, double *out_3175353753727933727);
void car_h_28(double *state, double *unused, double *out_7576558685618803179);
void car_H_28(double *state, double *unused, double *out_5138984025293259978);
void car_h_31(double *state, double *unused, double *out_8363636550973010358);
void car_H_31(double *state, double *unused, double *out_2825107318433742041);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}