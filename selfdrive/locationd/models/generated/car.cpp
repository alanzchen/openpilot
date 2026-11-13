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
void err_fun(double *nom_x, double *delta_x, double *out_4964988638861534016) {
   out_4964988638861534016[0] = delta_x[0] + nom_x[0];
   out_4964988638861534016[1] = delta_x[1] + nom_x[1];
   out_4964988638861534016[2] = delta_x[2] + nom_x[2];
   out_4964988638861534016[3] = delta_x[3] + nom_x[3];
   out_4964988638861534016[4] = delta_x[4] + nom_x[4];
   out_4964988638861534016[5] = delta_x[5] + nom_x[5];
   out_4964988638861534016[6] = delta_x[6] + nom_x[6];
   out_4964988638861534016[7] = delta_x[7] + nom_x[7];
   out_4964988638861534016[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1640007944371605601) {
   out_1640007944371605601[0] = -nom_x[0] + true_x[0];
   out_1640007944371605601[1] = -nom_x[1] + true_x[1];
   out_1640007944371605601[2] = -nom_x[2] + true_x[2];
   out_1640007944371605601[3] = -nom_x[3] + true_x[3];
   out_1640007944371605601[4] = -nom_x[4] + true_x[4];
   out_1640007944371605601[5] = -nom_x[5] + true_x[5];
   out_1640007944371605601[6] = -nom_x[6] + true_x[6];
   out_1640007944371605601[7] = -nom_x[7] + true_x[7];
   out_1640007944371605601[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_728519293432710064) {
   out_728519293432710064[0] = 1.0;
   out_728519293432710064[1] = 0.0;
   out_728519293432710064[2] = 0.0;
   out_728519293432710064[3] = 0.0;
   out_728519293432710064[4] = 0.0;
   out_728519293432710064[5] = 0.0;
   out_728519293432710064[6] = 0.0;
   out_728519293432710064[7] = 0.0;
   out_728519293432710064[8] = 0.0;
   out_728519293432710064[9] = 0.0;
   out_728519293432710064[10] = 1.0;
   out_728519293432710064[11] = 0.0;
   out_728519293432710064[12] = 0.0;
   out_728519293432710064[13] = 0.0;
   out_728519293432710064[14] = 0.0;
   out_728519293432710064[15] = 0.0;
   out_728519293432710064[16] = 0.0;
   out_728519293432710064[17] = 0.0;
   out_728519293432710064[18] = 0.0;
   out_728519293432710064[19] = 0.0;
   out_728519293432710064[20] = 1.0;
   out_728519293432710064[21] = 0.0;
   out_728519293432710064[22] = 0.0;
   out_728519293432710064[23] = 0.0;
   out_728519293432710064[24] = 0.0;
   out_728519293432710064[25] = 0.0;
   out_728519293432710064[26] = 0.0;
   out_728519293432710064[27] = 0.0;
   out_728519293432710064[28] = 0.0;
   out_728519293432710064[29] = 0.0;
   out_728519293432710064[30] = 1.0;
   out_728519293432710064[31] = 0.0;
   out_728519293432710064[32] = 0.0;
   out_728519293432710064[33] = 0.0;
   out_728519293432710064[34] = 0.0;
   out_728519293432710064[35] = 0.0;
   out_728519293432710064[36] = 0.0;
   out_728519293432710064[37] = 0.0;
   out_728519293432710064[38] = 0.0;
   out_728519293432710064[39] = 0.0;
   out_728519293432710064[40] = 1.0;
   out_728519293432710064[41] = 0.0;
   out_728519293432710064[42] = 0.0;
   out_728519293432710064[43] = 0.0;
   out_728519293432710064[44] = 0.0;
   out_728519293432710064[45] = 0.0;
   out_728519293432710064[46] = 0.0;
   out_728519293432710064[47] = 0.0;
   out_728519293432710064[48] = 0.0;
   out_728519293432710064[49] = 0.0;
   out_728519293432710064[50] = 1.0;
   out_728519293432710064[51] = 0.0;
   out_728519293432710064[52] = 0.0;
   out_728519293432710064[53] = 0.0;
   out_728519293432710064[54] = 0.0;
   out_728519293432710064[55] = 0.0;
   out_728519293432710064[56] = 0.0;
   out_728519293432710064[57] = 0.0;
   out_728519293432710064[58] = 0.0;
   out_728519293432710064[59] = 0.0;
   out_728519293432710064[60] = 1.0;
   out_728519293432710064[61] = 0.0;
   out_728519293432710064[62] = 0.0;
   out_728519293432710064[63] = 0.0;
   out_728519293432710064[64] = 0.0;
   out_728519293432710064[65] = 0.0;
   out_728519293432710064[66] = 0.0;
   out_728519293432710064[67] = 0.0;
   out_728519293432710064[68] = 0.0;
   out_728519293432710064[69] = 0.0;
   out_728519293432710064[70] = 1.0;
   out_728519293432710064[71] = 0.0;
   out_728519293432710064[72] = 0.0;
   out_728519293432710064[73] = 0.0;
   out_728519293432710064[74] = 0.0;
   out_728519293432710064[75] = 0.0;
   out_728519293432710064[76] = 0.0;
   out_728519293432710064[77] = 0.0;
   out_728519293432710064[78] = 0.0;
   out_728519293432710064[79] = 0.0;
   out_728519293432710064[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_2854727407156294866) {
   out_2854727407156294866[0] = state[0];
   out_2854727407156294866[1] = state[1];
   out_2854727407156294866[2] = state[2];
   out_2854727407156294866[3] = state[3];
   out_2854727407156294866[4] = state[4];
   out_2854727407156294866[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_2854727407156294866[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_2854727407156294866[7] = state[7];
   out_2854727407156294866[8] = state[8];
}
void F_fun(double *state, double dt, double *out_2914726948904805167) {
   out_2914726948904805167[0] = 1;
   out_2914726948904805167[1] = 0;
   out_2914726948904805167[2] = 0;
   out_2914726948904805167[3] = 0;
   out_2914726948904805167[4] = 0;
   out_2914726948904805167[5] = 0;
   out_2914726948904805167[6] = 0;
   out_2914726948904805167[7] = 0;
   out_2914726948904805167[8] = 0;
   out_2914726948904805167[9] = 0;
   out_2914726948904805167[10] = 1;
   out_2914726948904805167[11] = 0;
   out_2914726948904805167[12] = 0;
   out_2914726948904805167[13] = 0;
   out_2914726948904805167[14] = 0;
   out_2914726948904805167[15] = 0;
   out_2914726948904805167[16] = 0;
   out_2914726948904805167[17] = 0;
   out_2914726948904805167[18] = 0;
   out_2914726948904805167[19] = 0;
   out_2914726948904805167[20] = 1;
   out_2914726948904805167[21] = 0;
   out_2914726948904805167[22] = 0;
   out_2914726948904805167[23] = 0;
   out_2914726948904805167[24] = 0;
   out_2914726948904805167[25] = 0;
   out_2914726948904805167[26] = 0;
   out_2914726948904805167[27] = 0;
   out_2914726948904805167[28] = 0;
   out_2914726948904805167[29] = 0;
   out_2914726948904805167[30] = 1;
   out_2914726948904805167[31] = 0;
   out_2914726948904805167[32] = 0;
   out_2914726948904805167[33] = 0;
   out_2914726948904805167[34] = 0;
   out_2914726948904805167[35] = 0;
   out_2914726948904805167[36] = 0;
   out_2914726948904805167[37] = 0;
   out_2914726948904805167[38] = 0;
   out_2914726948904805167[39] = 0;
   out_2914726948904805167[40] = 1;
   out_2914726948904805167[41] = 0;
   out_2914726948904805167[42] = 0;
   out_2914726948904805167[43] = 0;
   out_2914726948904805167[44] = 0;
   out_2914726948904805167[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_2914726948904805167[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_2914726948904805167[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2914726948904805167[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2914726948904805167[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_2914726948904805167[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_2914726948904805167[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_2914726948904805167[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_2914726948904805167[53] = -9.8100000000000005*dt;
   out_2914726948904805167[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_2914726948904805167[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_2914726948904805167[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2914726948904805167[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2914726948904805167[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_2914726948904805167[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_2914726948904805167[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_2914726948904805167[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2914726948904805167[62] = 0;
   out_2914726948904805167[63] = 0;
   out_2914726948904805167[64] = 0;
   out_2914726948904805167[65] = 0;
   out_2914726948904805167[66] = 0;
   out_2914726948904805167[67] = 0;
   out_2914726948904805167[68] = 0;
   out_2914726948904805167[69] = 0;
   out_2914726948904805167[70] = 1;
   out_2914726948904805167[71] = 0;
   out_2914726948904805167[72] = 0;
   out_2914726948904805167[73] = 0;
   out_2914726948904805167[74] = 0;
   out_2914726948904805167[75] = 0;
   out_2914726948904805167[76] = 0;
   out_2914726948904805167[77] = 0;
   out_2914726948904805167[78] = 0;
   out_2914726948904805167[79] = 0;
   out_2914726948904805167[80] = 1;
}
void h_25(double *state, double *unused, double *out_6142951501910734792) {
   out_6142951501910734792[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7192818739541149741) {
   out_7192818739541149741[0] = 0;
   out_7192818739541149741[1] = 0;
   out_7192818739541149741[2] = 0;
   out_7192818739541149741[3] = 0;
   out_7192818739541149741[4] = 0;
   out_7192818739541149741[5] = 0;
   out_7192818739541149741[6] = 1;
   out_7192818739541149741[7] = 0;
   out_7192818739541149741[8] = 0;
}
void h_24(double *state, double *unused, double *out_8358013461846682543) {
   out_8358013461846682543[0] = state[4];
   out_8358013461846682543[1] = state[5];
}
void H_24(double *state, double *unused, double *out_5020169140535650175) {
   out_5020169140535650175[0] = 0;
   out_5020169140535650175[1] = 0;
   out_5020169140535650175[2] = 0;
   out_5020169140535650175[3] = 0;
   out_5020169140535650175[4] = 1;
   out_5020169140535650175[5] = 0;
   out_5020169140535650175[6] = 0;
   out_5020169140535650175[7] = 0;
   out_5020169140535650175[8] = 0;
   out_5020169140535650175[9] = 0;
   out_5020169140535650175[10] = 0;
   out_5020169140535650175[11] = 0;
   out_5020169140535650175[12] = 0;
   out_5020169140535650175[13] = 0;
   out_5020169140535650175[14] = 1;
   out_5020169140535650175[15] = 0;
   out_5020169140535650175[16] = 0;
   out_5020169140535650175[17] = 0;
}
void h_30(double *state, double *unused, double *out_3663915095080274940) {
   out_3663915095080274940[0] = state[4];
}
void H_30(double *state, double *unused, double *out_2665122409413541543) {
   out_2665122409413541543[0] = 0;
   out_2665122409413541543[1] = 0;
   out_2665122409413541543[2] = 0;
   out_2665122409413541543[3] = 0;
   out_2665122409413541543[4] = 1;
   out_2665122409413541543[5] = 0;
   out_2665122409413541543[6] = 0;
   out_2665122409413541543[7] = 0;
   out_2665122409413541543[8] = 0;
}
void h_26(double *state, double *unused, double *out_1337758172580570705) {
   out_1337758172580570705[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3451315420667093517) {
   out_3451315420667093517[0] = 0;
   out_3451315420667093517[1] = 0;
   out_3451315420667093517[2] = 0;
   out_3451315420667093517[3] = 0;
   out_3451315420667093517[4] = 0;
   out_3451315420667093517[5] = 0;
   out_3451315420667093517[6] = 0;
   out_3451315420667093517[7] = 1;
   out_3451315420667093517[8] = 0;
}
void h_27(double *state, double *unused, double *out_2574885417957957534) {
   out_2574885417957957534[0] = state[3];
}
void H_27(double *state, double *unused, double *out_490359097613116632) {
   out_490359097613116632[0] = 0;
   out_490359097613116632[1] = 0;
   out_490359097613116632[2] = 0;
   out_490359097613116632[3] = 1;
   out_490359097613116632[4] = 0;
   out_490359097613116632[5] = 0;
   out_490359097613116632[6] = 0;
   out_490359097613116632[7] = 0;
   out_490359097613116632[8] = 0;
}
void h_29(double *state, double *unused, double *out_5368077147829101202) {
   out_5368077147829101202[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3175353753727933727) {
   out_3175353753727933727[0] = 0;
   out_3175353753727933727[1] = 1;
   out_3175353753727933727[2] = 0;
   out_3175353753727933727[3] = 0;
   out_3175353753727933727[4] = 0;
   out_3175353753727933727[5] = 0;
   out_3175353753727933727[6] = 0;
   out_3175353753727933727[7] = 0;
   out_3175353753727933727[8] = 0;
}
void h_28(double *state, double *unused, double *out_7576558685618803179) {
   out_7576558685618803179[0] = state[0];
}
void H_28(double *state, double *unused, double *out_5138984025293259978) {
   out_5138984025293259978[0] = 1;
   out_5138984025293259978[1] = 0;
   out_5138984025293259978[2] = 0;
   out_5138984025293259978[3] = 0;
   out_5138984025293259978[4] = 0;
   out_5138984025293259978[5] = 0;
   out_5138984025293259978[6] = 0;
   out_5138984025293259978[7] = 0;
   out_5138984025293259978[8] = 0;
}
void h_31(double *state, double *unused, double *out_8363636550973010358) {
   out_8363636550973010358[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2825107318433742041) {
   out_2825107318433742041[0] = 0;
   out_2825107318433742041[1] = 0;
   out_2825107318433742041[2] = 0;
   out_2825107318433742041[3] = 0;
   out_2825107318433742041[4] = 0;
   out_2825107318433742041[5] = 0;
   out_2825107318433742041[6] = 0;
   out_2825107318433742041[7] = 0;
   out_2825107318433742041[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_4964988638861534016) {
  err_fun(nom_x, delta_x, out_4964988638861534016);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1640007944371605601) {
  inv_err_fun(nom_x, true_x, out_1640007944371605601);
}
void car_H_mod_fun(double *state, double *out_728519293432710064) {
  H_mod_fun(state, out_728519293432710064);
}
void car_f_fun(double *state, double dt, double *out_2854727407156294866) {
  f_fun(state,  dt, out_2854727407156294866);
}
void car_F_fun(double *state, double dt, double *out_2914726948904805167) {
  F_fun(state,  dt, out_2914726948904805167);
}
void car_h_25(double *state, double *unused, double *out_6142951501910734792) {
  h_25(state, unused, out_6142951501910734792);
}
void car_H_25(double *state, double *unused, double *out_7192818739541149741) {
  H_25(state, unused, out_7192818739541149741);
}
void car_h_24(double *state, double *unused, double *out_8358013461846682543) {
  h_24(state, unused, out_8358013461846682543);
}
void car_H_24(double *state, double *unused, double *out_5020169140535650175) {
  H_24(state, unused, out_5020169140535650175);
}
void car_h_30(double *state, double *unused, double *out_3663915095080274940) {
  h_30(state, unused, out_3663915095080274940);
}
void car_H_30(double *state, double *unused, double *out_2665122409413541543) {
  H_30(state, unused, out_2665122409413541543);
}
void car_h_26(double *state, double *unused, double *out_1337758172580570705) {
  h_26(state, unused, out_1337758172580570705);
}
void car_H_26(double *state, double *unused, double *out_3451315420667093517) {
  H_26(state, unused, out_3451315420667093517);
}
void car_h_27(double *state, double *unused, double *out_2574885417957957534) {
  h_27(state, unused, out_2574885417957957534);
}
void car_H_27(double *state, double *unused, double *out_490359097613116632) {
  H_27(state, unused, out_490359097613116632);
}
void car_h_29(double *state, double *unused, double *out_5368077147829101202) {
  h_29(state, unused, out_5368077147829101202);
}
void car_H_29(double *state, double *unused, double *out_3175353753727933727) {
  H_29(state, unused, out_3175353753727933727);
}
void car_h_28(double *state, double *unused, double *out_7576558685618803179) {
  h_28(state, unused, out_7576558685618803179);
}
void car_H_28(double *state, double *unused, double *out_5138984025293259978) {
  H_28(state, unused, out_5138984025293259978);
}
void car_h_31(double *state, double *unused, double *out_8363636550973010358) {
  h_31(state, unused, out_8363636550973010358);
}
void car_H_31(double *state, double *unused, double *out_2825107318433742041) {
  H_31(state, unused, out_2825107318433742041);
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
