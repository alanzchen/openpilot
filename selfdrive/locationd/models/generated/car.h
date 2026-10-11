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
void car_err_fun(double *nom_x, double *delta_x, double *out_6653433619134560972);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_4075777637853922991);
void car_H_mod_fun(double *state, double *out_650513881183439463);
void car_f_fun(double *state, double dt, double *out_3572410594373472278);
void car_F_fun(double *state, double dt, double *out_6071102774533750593);
void car_h_25(double *state, double *unused, double *out_6443152477224924979);
void car_H_25(double *state, double *unused, double *out_145179438983142751);
void car_h_24(double *state, double *unused, double *out_3730775051233589211);
void car_H_24(double *state, double *unused, double *out_4127502590380910649);
void car_h_30(double *state, double *unused, double *out_9055865606129141676);
void car_H_30(double *state, double *unused, double *out_6771510902508474004);
void car_h_26(double *state, double *unused, double *out_6243313502859288293);
void car_H_26(double *state, double *unused, double *out_3886682757857198975);
void car_h_27(double *state, double *unused, double *out_4585685138250245853);
void car_H_27(double *state, double *unused, double *out_4596747590708049093);
void car_h_29(double *state, double *unused, double *out_8452549721452664159);
void car_H_29(double *state, double *unused, double *out_7281742246822866188);
void car_h_28(double *state, double *unused, double *out_2462856572878929859);
void car_H_28(double *state, double *unused, double *out_2199343229753335614);
void car_h_31(double *state, double *unused, double *out_1438120173529124206);
void car_H_31(double *state, double *unused, double *out_114533477106182323);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}