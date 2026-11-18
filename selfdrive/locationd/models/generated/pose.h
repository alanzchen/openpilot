#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_5580352609619235680);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_977978120678855025);
void pose_H_mod_fun(double *state, double *out_6079805720435821411);
void pose_f_fun(double *state, double dt, double *out_600860468097978335);
void pose_F_fun(double *state, double dt, double *out_1300580827509148386);
void pose_h_4(double *state, double *unused, double *out_1401553991619916962);
void pose_H_4(double *state, double *unused, double *out_6518856546521771484);
void pose_h_10(double *state, double *unused, double *out_2150734223135501944);
void pose_H_10(double *state, double *unused, double *out_5900386540209642427);
void pose_h_13(double *state, double *unused, double *out_5542331391789455258);
void pose_H_13(double *state, double *unused, double *out_8715613701855447331);
void pose_h_14(double *state, double *unused, double *out_1432390697257379482);
void pose_H_14(double *state, double *unused, double *out_7964646670848295603);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}