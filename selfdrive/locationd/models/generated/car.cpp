#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_7910267928171215570) {
   out_7910267928171215570[0] = delta_x[0] + nom_x[0];
   out_7910267928171215570[1] = delta_x[1] + nom_x[1];
   out_7910267928171215570[2] = delta_x[2] + nom_x[2];
   out_7910267928171215570[3] = delta_x[3] + nom_x[3];
   out_7910267928171215570[4] = delta_x[4] + nom_x[4];
   out_7910267928171215570[5] = delta_x[5] + nom_x[5];
   out_7910267928171215570[6] = delta_x[6] + nom_x[6];
   out_7910267928171215570[7] = delta_x[7] + nom_x[7];
   out_7910267928171215570[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7159291503430419295) {
   out_7159291503430419295[0] = -nom_x[0] + true_x[0];
   out_7159291503430419295[1] = -nom_x[1] + true_x[1];
   out_7159291503430419295[2] = -nom_x[2] + true_x[2];
   out_7159291503430419295[3] = -nom_x[3] + true_x[3];
   out_7159291503430419295[4] = -nom_x[4] + true_x[4];
   out_7159291503430419295[5] = -nom_x[5] + true_x[5];
   out_7159291503430419295[6] = -nom_x[6] + true_x[6];
   out_7159291503430419295[7] = -nom_x[7] + true_x[7];
   out_7159291503430419295[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_5496957526712014953) {
   out_5496957526712014953[0] = 1.0;
   out_5496957526712014953[1] = 0.0;
   out_5496957526712014953[2] = 0.0;
   out_5496957526712014953[3] = 0.0;
   out_5496957526712014953[4] = 0.0;
   out_5496957526712014953[5] = 0.0;
   out_5496957526712014953[6] = 0.0;
   out_5496957526712014953[7] = 0.0;
   out_5496957526712014953[8] = 0.0;
   out_5496957526712014953[9] = 0.0;
   out_5496957526712014953[10] = 1.0;
   out_5496957526712014953[11] = 0.0;
   out_5496957526712014953[12] = 0.0;
   out_5496957526712014953[13] = 0.0;
   out_5496957526712014953[14] = 0.0;
   out_5496957526712014953[15] = 0.0;
   out_5496957526712014953[16] = 0.0;
   out_5496957526712014953[17] = 0.0;
   out_5496957526712014953[18] = 0.0;
   out_5496957526712014953[19] = 0.0;
   out_5496957526712014953[20] = 1.0;
   out_5496957526712014953[21] = 0.0;
   out_5496957526712014953[22] = 0.0;
   out_5496957526712014953[23] = 0.0;
   out_5496957526712014953[24] = 0.0;
   out_5496957526712014953[25] = 0.0;
   out_5496957526712014953[26] = 0.0;
   out_5496957526712014953[27] = 0.0;
   out_5496957526712014953[28] = 0.0;
   out_5496957526712014953[29] = 0.0;
   out_5496957526712014953[30] = 1.0;
   out_5496957526712014953[31] = 0.0;
   out_5496957526712014953[32] = 0.0;
   out_5496957526712014953[33] = 0.0;
   out_5496957526712014953[34] = 0.0;
   out_5496957526712014953[35] = 0.0;
   out_5496957526712014953[36] = 0.0;
   out_5496957526712014953[37] = 0.0;
   out_5496957526712014953[38] = 0.0;
   out_5496957526712014953[39] = 0.0;
   out_5496957526712014953[40] = 1.0;
   out_5496957526712014953[41] = 0.0;
   out_5496957526712014953[42] = 0.0;
   out_5496957526712014953[43] = 0.0;
   out_5496957526712014953[44] = 0.0;
   out_5496957526712014953[45] = 0.0;
   out_5496957526712014953[46] = 0.0;
   out_5496957526712014953[47] = 0.0;
   out_5496957526712014953[48] = 0.0;
   out_5496957526712014953[49] = 0.0;
   out_5496957526712014953[50] = 1.0;
   out_5496957526712014953[51] = 0.0;
   out_5496957526712014953[52] = 0.0;
   out_5496957526712014953[53] = 0.0;
   out_5496957526712014953[54] = 0.0;
   out_5496957526712014953[55] = 0.0;
   out_5496957526712014953[56] = 0.0;
   out_5496957526712014953[57] = 0.0;
   out_5496957526712014953[58] = 0.0;
   out_5496957526712014953[59] = 0.0;
   out_5496957526712014953[60] = 1.0;
   out_5496957526712014953[61] = 0.0;
   out_5496957526712014953[62] = 0.0;
   out_5496957526712014953[63] = 0.0;
   out_5496957526712014953[64] = 0.0;
   out_5496957526712014953[65] = 0.0;
   out_5496957526712014953[66] = 0.0;
   out_5496957526712014953[67] = 0.0;
   out_5496957526712014953[68] = 0.0;
   out_5496957526712014953[69] = 0.0;
   out_5496957526712014953[70] = 1.0;
   out_5496957526712014953[71] = 0.0;
   out_5496957526712014953[72] = 0.0;
   out_5496957526712014953[73] = 0.0;
   out_5496957526712014953[74] = 0.0;
   out_5496957526712014953[75] = 0.0;
   out_5496957526712014953[76] = 0.0;
   out_5496957526712014953[77] = 0.0;
   out_5496957526712014953[78] = 0.0;
   out_5496957526712014953[79] = 0.0;
   out_5496957526712014953[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1029461127856364564) {
   out_1029461127856364564[0] = state[0];
   out_1029461127856364564[1] = state[1];
   out_1029461127856364564[2] = state[2];
   out_1029461127856364564[3] = state[3];
   out_1029461127856364564[4] = state[4];
   out_1029461127856364564[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1029461127856364564[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1029461127856364564[7] = state[7];
   out_1029461127856364564[8] = state[8];
}
void F_fun(double *state, double dt, double *out_5756828007788109456) {
   out_5756828007788109456[0] = 1;
   out_5756828007788109456[1] = 0;
   out_5756828007788109456[2] = 0;
   out_5756828007788109456[3] = 0;
   out_5756828007788109456[4] = 0;
   out_5756828007788109456[5] = 0;
   out_5756828007788109456[6] = 0;
   out_5756828007788109456[7] = 0;
   out_5756828007788109456[8] = 0;
   out_5756828007788109456[9] = 0;
   out_5756828007788109456[10] = 1;
   out_5756828007788109456[11] = 0;
   out_5756828007788109456[12] = 0;
   out_5756828007788109456[13] = 0;
   out_5756828007788109456[14] = 0;
   out_5756828007788109456[15] = 0;
   out_5756828007788109456[16] = 0;
   out_5756828007788109456[17] = 0;
   out_5756828007788109456[18] = 0;
   out_5756828007788109456[19] = 0;
   out_5756828007788109456[20] = 1;
   out_5756828007788109456[21] = 0;
   out_5756828007788109456[22] = 0;
   out_5756828007788109456[23] = 0;
   out_5756828007788109456[24] = 0;
   out_5756828007788109456[25] = 0;
   out_5756828007788109456[26] = 0;
   out_5756828007788109456[27] = 0;
   out_5756828007788109456[28] = 0;
   out_5756828007788109456[29] = 0;
   out_5756828007788109456[30] = 1;
   out_5756828007788109456[31] = 0;
   out_5756828007788109456[32] = 0;
   out_5756828007788109456[33] = 0;
   out_5756828007788109456[34] = 0;
   out_5756828007788109456[35] = 0;
   out_5756828007788109456[36] = 0;
   out_5756828007788109456[37] = 0;
   out_5756828007788109456[38] = 0;
   out_5756828007788109456[39] = 0;
   out_5756828007788109456[40] = 1;
   out_5756828007788109456[41] = 0;
   out_5756828007788109456[42] = 0;
   out_5756828007788109456[43] = 0;
   out_5756828007788109456[44] = 0;
   out_5756828007788109456[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_5756828007788109456[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_5756828007788109456[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5756828007788109456[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5756828007788109456[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_5756828007788109456[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_5756828007788109456[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_5756828007788109456[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_5756828007788109456[53] = -9.8100000000000005*dt;
   out_5756828007788109456[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_5756828007788109456[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_5756828007788109456[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5756828007788109456[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5756828007788109456[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_5756828007788109456[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_5756828007788109456[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_5756828007788109456[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5756828007788109456[62] = 0;
   out_5756828007788109456[63] = 0;
   out_5756828007788109456[64] = 0;
   out_5756828007788109456[65] = 0;
   out_5756828007788109456[66] = 0;
   out_5756828007788109456[67] = 0;
   out_5756828007788109456[68] = 0;
   out_5756828007788109456[69] = 0;
   out_5756828007788109456[70] = 1;
   out_5756828007788109456[71] = 0;
   out_5756828007788109456[72] = 0;
   out_5756828007788109456[73] = 0;
   out_5756828007788109456[74] = 0;
   out_5756828007788109456[75] = 0;
   out_5756828007788109456[76] = 0;
   out_5756828007788109456[77] = 0;
   out_5756828007788109456[78] = 0;
   out_5756828007788109456[79] = 0;
   out_5756828007788109456[80] = 1;
}
void h_25(double *state, double *unused, double *out_4771532071622313326) {
   out_4771532071622313326[0] = state[6];
}
void H_25(double *state, double *unused, double *out_5028448514023676858) {
   out_5028448514023676858[0] = 0;
   out_5028448514023676858[1] = 0;
   out_5028448514023676858[2] = 0;
   out_5028448514023676858[3] = 0;
   out_5028448514023676858[4] = 0;
   out_5028448514023676858[5] = 0;
   out_5028448514023676858[6] = 1;
   out_5028448514023676858[7] = 0;
   out_5028448514023676858[8] = 0;
}
void h_24(double *state, double *unused, double *out_3583680814182296994) {
   out_3583680814182296994[0] = state[4];
   out_3583680814182296994[1] = state[5];
}
void H_24(double *state, double *unused, double *out_1470522915221353722) {
   out_1470522915221353722[0] = 0;
   out_1470522915221353722[1] = 0;
   out_1470522915221353722[2] = 0;
   out_1470522915221353722[3] = 0;
   out_1470522915221353722[4] = 1;
   out_1470522915221353722[5] = 0;
   out_1470522915221353722[6] = 0;
   out_1470522915221353722[7] = 0;
   out_1470522915221353722[8] = 0;
   out_1470522915221353722[9] = 0;
   out_1470522915221353722[10] = 0;
   out_1470522915221353722[11] = 0;
   out_1470522915221353722[12] = 0;
   out_1470522915221353722[13] = 0;
   out_1470522915221353722[14] = 1;
   out_1470522915221353722[15] = 0;
   out_1470522915221353722[16] = 0;
   out_1470522915221353722[17] = 0;
}
void h_30(double *state, double *unused, double *out_4811100577194217104) {
   out_4811100577194217104[0] = state[4];
}
void H_30(double *state, double *unused, double *out_8890599229558266560) {
   out_8890599229558266560[0] = 0;
   out_8890599229558266560[1] = 0;
   out_8890599229558266560[2] = 0;
   out_8890599229558266560[3] = 0;
   out_8890599229558266560[4] = 1;
   out_8890599229558266560[5] = 0;
   out_8890599229558266560[6] = 0;
   out_8890599229558266560[7] = 0;
   out_8890599229558266560[8] = 0;
}
void h_26(double *state, double *unused, double *out_3985208887071780036) {
   out_3985208887071780036[0] = state[7];
}
void H_26(double *state, double *unused, double *out_8769951832897733082) {
   out_8769951832897733082[0] = 0;
   out_8769951832897733082[1] = 0;
   out_8769951832897733082[2] = 0;
   out_8769951832897733082[3] = 0;
   out_8769951832897733082[4] = 0;
   out_8769951832897733082[5] = 0;
   out_8769951832897733082[6] = 0;
   out_8769951832897733082[7] = 1;
   out_8769951832897733082[8] = 0;
}
void h_27(double *state, double *unused, double *out_6240068567658537477) {
   out_6240068567658537477[0] = state[3];
}
void H_27(double *state, double *unused, double *out_6715835917757841649) {
   out_6715835917757841649[0] = 0;
   out_6715835917757841649[1] = 0;
   out_6715835917757841649[2] = 0;
   out_6715835917757841649[3] = 1;
   out_6715835917757841649[4] = 0;
   out_6715835917757841649[5] = 0;
   out_6715835917757841649[6] = 0;
   out_6715835917757841649[7] = 0;
   out_6715835917757841649[8] = 0;
}
void h_29(double *state, double *unused, double *out_72954527687403174) {
   out_72954527687403174[0] = state[1];
}
void H_29(double *state, double *unused, double *out_9045913499836892872) {
   out_9045913499836892872[0] = 0;
   out_9045913499836892872[1] = 1;
   out_9045913499836892872[2] = 0;
   out_9045913499836892872[3] = 0;
   out_9045913499836892872[4] = 0;
   out_9045913499836892872[5] = 0;
   out_9045913499836892872[6] = 0;
   out_9045913499836892872[7] = 0;
   out_9045913499836892872[8] = 0;
}
void h_28(double *state, double *unused, double *out_7205002238390168494) {
   out_7205002238390168494[0] = state[0];
}
void H_28(double *state, double *unused, double *out_7082283228271566621) {
   out_7082283228271566621[0] = 1;
   out_7082283228271566621[1] = 0;
   out_7082283228271566621[2] = 0;
   out_7082283228271566621[3] = 0;
   out_7082283228271566621[4] = 0;
   out_7082283228271566621[5] = 0;
   out_7082283228271566621[6] = 0;
   out_7082283228271566621[7] = 0;
   out_7082283228271566621[8] = 0;
}
void h_31(double *state, double *unused, double *out_2689026692626574999) {
   out_2689026692626574999[0] = state[8];
}
void H_31(double *state, double *unused, double *out_9050584138578467058) {
   out_9050584138578467058[0] = 0;
   out_9050584138578467058[1] = 0;
   out_9050584138578467058[2] = 0;
   out_9050584138578467058[3] = 0;
   out_9050584138578467058[4] = 0;
   out_9050584138578467058[5] = 0;
   out_9050584138578467058[6] = 0;
   out_9050584138578467058[7] = 0;
   out_9050584138578467058[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_7910267928171215570) {
  err_fun(nom_x, delta_x, out_7910267928171215570);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7159291503430419295) {
  inv_err_fun(nom_x, true_x, out_7159291503430419295);
}
void car_H_mod_fun(double *state, double *out_5496957526712014953) {
  H_mod_fun(state, out_5496957526712014953);
}
void car_f_fun(double *state, double dt, double *out_1029461127856364564) {
  f_fun(state,  dt, out_1029461127856364564);
}
void car_F_fun(double *state, double dt, double *out_5756828007788109456) {
  F_fun(state,  dt, out_5756828007788109456);
}
void car_h_25(double *state, double *unused, double *out_4771532071622313326) {
  h_25(state, unused, out_4771532071622313326);
}
void car_H_25(double *state, double *unused, double *out_5028448514023676858) {
  H_25(state, unused, out_5028448514023676858);
}
void car_h_24(double *state, double *unused, double *out_3583680814182296994) {
  h_24(state, unused, out_3583680814182296994);
}
void car_H_24(double *state, double *unused, double *out_1470522915221353722) {
  H_24(state, unused, out_1470522915221353722);
}
void car_h_30(double *state, double *unused, double *out_4811100577194217104) {
  h_30(state, unused, out_4811100577194217104);
}
void car_H_30(double *state, double *unused, double *out_8890599229558266560) {
  H_30(state, unused, out_8890599229558266560);
}
void car_h_26(double *state, double *unused, double *out_3985208887071780036) {
  h_26(state, unused, out_3985208887071780036);
}
void car_H_26(double *state, double *unused, double *out_8769951832897733082) {
  H_26(state, unused, out_8769951832897733082);
}
void car_h_27(double *state, double *unused, double *out_6240068567658537477) {
  h_27(state, unused, out_6240068567658537477);
}
void car_H_27(double *state, double *unused, double *out_6715835917757841649) {
  H_27(state, unused, out_6715835917757841649);
}
void car_h_29(double *state, double *unused, double *out_72954527687403174) {
  h_29(state, unused, out_72954527687403174);
}
void car_H_29(double *state, double *unused, double *out_9045913499836892872) {
  H_29(state, unused, out_9045913499836892872);
}
void car_h_28(double *state, double *unused, double *out_7205002238390168494) {
  h_28(state, unused, out_7205002238390168494);
}
void car_H_28(double *state, double *unused, double *out_7082283228271566621) {
  H_28(state, unused, out_7082283228271566621);
}
void car_h_31(double *state, double *unused, double *out_2689026692626574999) {
  h_31(state, unused, out_2689026692626574999);
}
void car_H_31(double *state, double *unused, double *out_9050584138578467058) {
  H_31(state, unused, out_9050584138578467058);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
