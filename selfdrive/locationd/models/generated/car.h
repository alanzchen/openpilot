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
void car_err_fun(double *nom_x, double *delta_x, double *out_7910267928171215570);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7159291503430419295);
void car_H_mod_fun(double *state, double *out_5496957526712014953);
void car_f_fun(double *state, double dt, double *out_1029461127856364564);
void car_F_fun(double *state, double dt, double *out_5756828007788109456);
void car_h_25(double *state, double *unused, double *out_4771532071622313326);
void car_H_25(double *state, double *unused, double *out_5028448514023676858);
void car_h_24(double *state, double *unused, double *out_3583680814182296994);
void car_H_24(double *state, double *unused, double *out_1470522915221353722);
void car_h_30(double *state, double *unused, double *out_4811100577194217104);
void car_H_30(double *state, double *unused, double *out_8890599229558266560);
void car_h_26(double *state, double *unused, double *out_3985208887071780036);
void car_H_26(double *state, double *unused, double *out_8769951832897733082);
void car_h_27(double *state, double *unused, double *out_6240068567658537477);
void car_H_27(double *state, double *unused, double *out_6715835917757841649);
void car_h_29(double *state, double *unused, double *out_72954527687403174);
void car_H_29(double *state, double *unused, double *out_9045913499836892872);
void car_h_28(double *state, double *unused, double *out_7205002238390168494);
void car_H_28(double *state, double *unused, double *out_7082283228271566621);
void car_h_31(double *state, double *unused, double *out_2689026692626574999);
void car_H_31(double *state, double *unused, double *out_9050584138578467058);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}