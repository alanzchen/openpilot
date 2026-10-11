#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_5884898945529161916);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2695974985561036334);
void pose_H_mod_fun(double *state, double *out_3237876647370796076);
void pose_f_fun(double *state, double dt, double *out_5341964247148914054);
void pose_F_fun(double *state, double dt, double *out_6962674312365040456);
void pose_h_4(double *state, double *unused, double *out_802636545138050071);
void pose_H_4(double *state, double *unused, double *out_8390395032409876811);
void pose_h_10(double *state, double *unused, double *out_3869789647935931974);
void pose_H_10(double *state, double *unused, double *out_5760458429482568539);
void pose_h_13(double *state, double *unused, double *out_9172956192586326054);
void pose_H_13(double *state, double *unused, double *out_4556639569107352787);
void pose_h_14(double *state, double *unused, double *out_6465394495488652181);
void pose_H_14(double *state, double *unused, double *out_5307606600114504515);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}