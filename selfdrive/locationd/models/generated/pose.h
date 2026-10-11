#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_1637031099889953199);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3175856492030467000);
void pose_H_mod_fun(double *state, double *out_3254015575193517177);
void pose_f_fun(double *state, double dt, double *out_5452727561293164163);
void pose_F_fun(double *state, double dt, double *out_1596973880245959678);
void pose_h_4(double *state, double *unused, double *out_622781828802881011);
void pose_H_4(double *state, double *unused, double *out_1261848416734986970);
void pose_h_10(double *state, double *unused, double *out_5306685819502841937);
void pose_H_10(double *state, double *unused, double *out_8350343691043762535);
void pose_h_13(double *state, double *unused, double *out_9048838141074958611);
void pose_H_13(double *state, double *unused, double *out_1950425408597345831);
void pose_h_14(double *state, double *unused, double *out_2303399324759113969);
void pose_H_14(double *state, double *unused, double *out_2701392439604497559);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}