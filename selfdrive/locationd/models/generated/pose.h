#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_1794537510029647999);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_1818321109360324212);
void pose_H_mod_fun(double *state, double *out_4142439847419469755);
void pose_f_fun(double *state, double dt, double *out_274046506931821425);
void pose_F_fun(double *state, double dt, double *out_5107362200909236418);
void pose_h_4(double *state, double *unused, double *out_2986239765915263589);
void pose_H_4(double *state, double *unused, double *out_7914247057263377327);
void pose_h_10(double *state, double *unused, double *out_7953871030082380114);
void pose_H_10(double *state, double *unused, double *out_4746611735982339664);
void pose_h_13(double *state, double *unused, double *out_6031258319996835090);
void pose_H_13(double *state, double *unused, double *out_7320223191113841488);
void pose_h_14(double *state, double *unused, double *out_9013923068017280655);
void pose_H_14(double *state, double *unused, double *out_6569256160106689760);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}