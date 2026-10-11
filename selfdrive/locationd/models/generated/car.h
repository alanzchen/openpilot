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
void car_err_fun(double *nom_x, double *delta_x, double *out_4008055473606692387);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6014015634177356234);
void car_H_mod_fun(double *state, double *out_2369935343221964708);
void car_f_fun(double *state, double dt, double *out_1926748051812530846);
void car_F_fun(double *state, double dt, double *out_6644089939752104464);
void car_h_25(double *state, double *unused, double *out_258381263417220356);
void car_H_25(double *state, double *unused, double *out_2533756481962700132);
void car_h_24(double *state, double *unused, double *out_6738890471142770327);
void car_H_24(double *state, double *unused, double *out_7807250764875712726);
void car_h_30(double *state, double *unused, double *out_3206107253772705317);
void car_H_30(double *state, double *unused, double *out_5052089440469948759);
void car_h_26(double *state, double *unused, double *out_4149617106674367752);
void car_H_26(double *state, double *unused, double *out_1207746836911356092);
void car_h_27(double *state, double *unused, double *out_2778404788027112329);
void car_H_27(double *state, double *unused, double *out_229654223019035151);
void car_h_29(double *state, double *unused, double *out_8207780521433550962);
void car_H_29(double *state, double *unused, double *out_1483708503850515882);
void car_h_28(double *state, double *unused, double *out_2223268479633733316);
void car_H_28(double *state, double *unused, double *out_479921767714810369);
void car_h_31(double *state, double *unused, double *out_16812798867285533);
void car_H_31(double *state, double *unused, double *out_2564402443839660560);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}