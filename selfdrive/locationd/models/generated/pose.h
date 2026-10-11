#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_2238114088573503330);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3877213446685125163);
void pose_H_mod_fun(double *state, double *out_7869929370112138903);
void pose_f_fun(double *state, double dt, double *out_5672712038044060314);
void pose_F_fun(double *state, double dt, double *out_4762702412345730966);
void pose_h_4(double *state, double *unused, double *out_6450886787262042442);
void pose_H_4(double *state, double *unused, double *out_6805007493753505141);
void pose_h_10(double *state, double *unused, double *out_7174068847243736276);
void pose_H_10(double *state, double *unused, double *out_5919592439627785975);
void pose_h_13(double *state, double *unused, double *out_4032282206185544656);
void pose_H_13(double *state, double *unused, double *out_805623714563195788);
void pose_h_14(double *state, double *unused, double *out_6526222386602091277);
void pose_H_14(double *state, double *unused, double *out_8558948147660674179);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}