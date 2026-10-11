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
void err_fun(double *nom_x, double *delta_x, double *out_4008055473606692387) {
   out_4008055473606692387[0] = delta_x[0] + nom_x[0];
   out_4008055473606692387[1] = delta_x[1] + nom_x[1];
   out_4008055473606692387[2] = delta_x[2] + nom_x[2];
   out_4008055473606692387[3] = delta_x[3] + nom_x[3];
   out_4008055473606692387[4] = delta_x[4] + nom_x[4];
   out_4008055473606692387[5] = delta_x[5] + nom_x[5];
   out_4008055473606692387[6] = delta_x[6] + nom_x[6];
   out_4008055473606692387[7] = delta_x[7] + nom_x[7];
   out_4008055473606692387[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6014015634177356234) {
   out_6014015634177356234[0] = -nom_x[0] + true_x[0];
   out_6014015634177356234[1] = -nom_x[1] + true_x[1];
   out_6014015634177356234[2] = -nom_x[2] + true_x[2];
   out_6014015634177356234[3] = -nom_x[3] + true_x[3];
   out_6014015634177356234[4] = -nom_x[4] + true_x[4];
   out_6014015634177356234[5] = -nom_x[5] + true_x[5];
   out_6014015634177356234[6] = -nom_x[6] + true_x[6];
   out_6014015634177356234[7] = -nom_x[7] + true_x[7];
   out_6014015634177356234[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_2369935343221964708) {
   out_2369935343221964708[0] = 1.0;
   out_2369935343221964708[1] = 0.0;
   out_2369935343221964708[2] = 0.0;
   out_2369935343221964708[3] = 0.0;
   out_2369935343221964708[4] = 0.0;
   out_2369935343221964708[5] = 0.0;
   out_2369935343221964708[6] = 0.0;
   out_2369935343221964708[7] = 0.0;
   out_2369935343221964708[8] = 0.0;
   out_2369935343221964708[9] = 0.0;
   out_2369935343221964708[10] = 1.0;
   out_2369935343221964708[11] = 0.0;
   out_2369935343221964708[12] = 0.0;
   out_2369935343221964708[13] = 0.0;
   out_2369935343221964708[14] = 0.0;
   out_2369935343221964708[15] = 0.0;
   out_2369935343221964708[16] = 0.0;
   out_2369935343221964708[17] = 0.0;
   out_2369935343221964708[18] = 0.0;
   out_2369935343221964708[19] = 0.0;
   out_2369935343221964708[20] = 1.0;
   out_2369935343221964708[21] = 0.0;
   out_2369935343221964708[22] = 0.0;
   out_2369935343221964708[23] = 0.0;
   out_2369935343221964708[24] = 0.0;
   out_2369935343221964708[25] = 0.0;
   out_2369935343221964708[26] = 0.0;
   out_2369935343221964708[27] = 0.0;
   out_2369935343221964708[28] = 0.0;
   out_2369935343221964708[29] = 0.0;
   out_2369935343221964708[30] = 1.0;
   out_2369935343221964708[31] = 0.0;
   out_2369935343221964708[32] = 0.0;
   out_2369935343221964708[33] = 0.0;
   out_2369935343221964708[34] = 0.0;
   out_2369935343221964708[35] = 0.0;
   out_2369935343221964708[36] = 0.0;
   out_2369935343221964708[37] = 0.0;
   out_2369935343221964708[38] = 0.0;
   out_2369935343221964708[39] = 0.0;
   out_2369935343221964708[40] = 1.0;
   out_2369935343221964708[41] = 0.0;
   out_2369935343221964708[42] = 0.0;
   out_2369935343221964708[43] = 0.0;
   out_2369935343221964708[44] = 0.0;
   out_2369935343221964708[45] = 0.0;
   out_2369935343221964708[46] = 0.0;
   out_2369935343221964708[47] = 0.0;
   out_2369935343221964708[48] = 0.0;
   out_2369935343221964708[49] = 0.0;
   out_2369935343221964708[50] = 1.0;
   out_2369935343221964708[51] = 0.0;
   out_2369935343221964708[52] = 0.0;
   out_2369935343221964708[53] = 0.0;
   out_2369935343221964708[54] = 0.0;
   out_2369935343221964708[55] = 0.0;
   out_2369935343221964708[56] = 0.0;
   out_2369935343221964708[57] = 0.0;
   out_2369935343221964708[58] = 0.0;
   out_2369935343221964708[59] = 0.0;
   out_2369935343221964708[60] = 1.0;
   out_2369935343221964708[61] = 0.0;
   out_2369935343221964708[62] = 0.0;
   out_2369935343221964708[63] = 0.0;
   out_2369935343221964708[64] = 0.0;
   out_2369935343221964708[65] = 0.0;
   out_2369935343221964708[66] = 0.0;
   out_2369935343221964708[67] = 0.0;
   out_2369935343221964708[68] = 0.0;
   out_2369935343221964708[69] = 0.0;
   out_2369935343221964708[70] = 1.0;
   out_2369935343221964708[71] = 0.0;
   out_2369935343221964708[72] = 0.0;
   out_2369935343221964708[73] = 0.0;
   out_2369935343221964708[74] = 0.0;
   out_2369935343221964708[75] = 0.0;
   out_2369935343221964708[76] = 0.0;
   out_2369935343221964708[77] = 0.0;
   out_2369935343221964708[78] = 0.0;
   out_2369935343221964708[79] = 0.0;
   out_2369935343221964708[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1926748051812530846) {
   out_1926748051812530846[0] = state[0];
   out_1926748051812530846[1] = state[1];
   out_1926748051812530846[2] = state[2];
   out_1926748051812530846[3] = state[3];
   out_1926748051812530846[4] = state[4];
   out_1926748051812530846[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1926748051812530846[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1926748051812530846[7] = state[7];
   out_1926748051812530846[8] = state[8];
}
void F_fun(double *state, double dt, double *out_6644089939752104464) {
   out_6644089939752104464[0] = 1;
   out_6644089939752104464[1] = 0;
   out_6644089939752104464[2] = 0;
   out_6644089939752104464[3] = 0;
   out_6644089939752104464[4] = 0;
   out_6644089939752104464[5] = 0;
   out_6644089939752104464[6] = 0;
   out_6644089939752104464[7] = 0;
   out_6644089939752104464[8] = 0;
   out_6644089939752104464[9] = 0;
   out_6644089939752104464[10] = 1;
   out_6644089939752104464[11] = 0;
   out_6644089939752104464[12] = 0;
   out_6644089939752104464[13] = 0;
   out_6644089939752104464[14] = 0;
   out_6644089939752104464[15] = 0;
   out_6644089939752104464[16] = 0;
   out_6644089939752104464[17] = 0;
   out_6644089939752104464[18] = 0;
   out_6644089939752104464[19] = 0;
   out_6644089939752104464[20] = 1;
   out_6644089939752104464[21] = 0;
   out_6644089939752104464[22] = 0;
   out_6644089939752104464[23] = 0;
   out_6644089939752104464[24] = 0;
   out_6644089939752104464[25] = 0;
   out_6644089939752104464[26] = 0;
   out_6644089939752104464[27] = 0;
   out_6644089939752104464[28] = 0;
   out_6644089939752104464[29] = 0;
   out_6644089939752104464[30] = 1;
   out_6644089939752104464[31] = 0;
   out_6644089939752104464[32] = 0;
   out_6644089939752104464[33] = 0;
   out_6644089939752104464[34] = 0;
   out_6644089939752104464[35] = 0;
   out_6644089939752104464[36] = 0;
   out_6644089939752104464[37] = 0;
   out_6644089939752104464[38] = 0;
   out_6644089939752104464[39] = 0;
   out_6644089939752104464[40] = 1;
   out_6644089939752104464[41] = 0;
   out_6644089939752104464[42] = 0;
   out_6644089939752104464[43] = 0;
   out_6644089939752104464[44] = 0;
   out_6644089939752104464[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6644089939752104464[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6644089939752104464[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6644089939752104464[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6644089939752104464[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6644089939752104464[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6644089939752104464[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6644089939752104464[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6644089939752104464[53] = -9.8100000000000005*dt;
   out_6644089939752104464[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6644089939752104464[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6644089939752104464[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6644089939752104464[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6644089939752104464[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6644089939752104464[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6644089939752104464[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6644089939752104464[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6644089939752104464[62] = 0;
   out_6644089939752104464[63] = 0;
   out_6644089939752104464[64] = 0;
   out_6644089939752104464[65] = 0;
   out_6644089939752104464[66] = 0;
   out_6644089939752104464[67] = 0;
   out_6644089939752104464[68] = 0;
   out_6644089939752104464[69] = 0;
   out_6644089939752104464[70] = 1;
   out_6644089939752104464[71] = 0;
   out_6644089939752104464[72] = 0;
   out_6644089939752104464[73] = 0;
   out_6644089939752104464[74] = 0;
   out_6644089939752104464[75] = 0;
   out_6644089939752104464[76] = 0;
   out_6644089939752104464[77] = 0;
   out_6644089939752104464[78] = 0;
   out_6644089939752104464[79] = 0;
   out_6644089939752104464[80] = 1;
}
void h_25(double *state, double *unused, double *out_258381263417220356) {
   out_258381263417220356[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2533756481962700132) {
   out_2533756481962700132[0] = 0;
   out_2533756481962700132[1] = 0;
   out_2533756481962700132[2] = 0;
   out_2533756481962700132[3] = 0;
   out_2533756481962700132[4] = 0;
   out_2533756481962700132[5] = 0;
   out_2533756481962700132[6] = 1;
   out_2533756481962700132[7] = 0;
   out_2533756481962700132[8] = 0;
}
void h_24(double *state, double *unused, double *out_6738890471142770327) {
   out_6738890471142770327[0] = state[4];
   out_6738890471142770327[1] = state[5];
}
void H_24(double *state, double *unused, double *out_7807250764875712726) {
   out_7807250764875712726[0] = 0;
   out_7807250764875712726[1] = 0;
   out_7807250764875712726[2] = 0;
   out_7807250764875712726[3] = 0;
   out_7807250764875712726[4] = 1;
   out_7807250764875712726[5] = 0;
   out_7807250764875712726[6] = 0;
   out_7807250764875712726[7] = 0;
   out_7807250764875712726[8] = 0;
   out_7807250764875712726[9] = 0;
   out_7807250764875712726[10] = 0;
   out_7807250764875712726[11] = 0;
   out_7807250764875712726[12] = 0;
   out_7807250764875712726[13] = 0;
   out_7807250764875712726[14] = 1;
   out_7807250764875712726[15] = 0;
   out_7807250764875712726[16] = 0;
   out_7807250764875712726[17] = 0;
}
void h_30(double *state, double *unused, double *out_3206107253772705317) {
   out_3206107253772705317[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5052089440469948759) {
   out_5052089440469948759[0] = 0;
   out_5052089440469948759[1] = 0;
   out_5052089440469948759[2] = 0;
   out_5052089440469948759[3] = 0;
   out_5052089440469948759[4] = 1;
   out_5052089440469948759[5] = 0;
   out_5052089440469948759[6] = 0;
   out_5052089440469948759[7] = 0;
   out_5052089440469948759[8] = 0;
}
void h_26(double *state, double *unused, double *out_4149617106674367752) {
   out_4149617106674367752[0] = state[7];
}
void H_26(double *state, double *unused, double *out_1207746836911356092) {
   out_1207746836911356092[0] = 0;
   out_1207746836911356092[1] = 0;
   out_1207746836911356092[2] = 0;
   out_1207746836911356092[3] = 0;
   out_1207746836911356092[4] = 0;
   out_1207746836911356092[5] = 0;
   out_1207746836911356092[6] = 0;
   out_1207746836911356092[7] = 1;
   out_1207746836911356092[8] = 0;
}
void h_27(double *state, double *unused, double *out_2778404788027112329) {
   out_2778404788027112329[0] = state[3];
}
void H_27(double *state, double *unused, double *out_229654223019035151) {
   out_229654223019035151[0] = 0;
   out_229654223019035151[1] = 0;
   out_229654223019035151[2] = 0;
   out_229654223019035151[3] = 1;
   out_229654223019035151[4] = 0;
   out_229654223019035151[5] = 0;
   out_229654223019035151[6] = 0;
   out_229654223019035151[7] = 0;
   out_229654223019035151[8] = 0;
}
void h_29(double *state, double *unused, double *out_8207780521433550962) {
   out_8207780521433550962[0] = state[1];
}
void H_29(double *state, double *unused, double *out_1483708503850515882) {
   out_1483708503850515882[0] = 0;
   out_1483708503850515882[1] = 1;
   out_1483708503850515882[2] = 0;
   out_1483708503850515882[3] = 0;
   out_1483708503850515882[4] = 0;
   out_1483708503850515882[5] = 0;
   out_1483708503850515882[6] = 0;
   out_1483708503850515882[7] = 0;
   out_1483708503850515882[8] = 0;
}
void h_28(double *state, double *unused, double *out_2223268479633733316) {
   out_2223268479633733316[0] = state[0];
}
void H_28(double *state, double *unused, double *out_479921767714810369) {
   out_479921767714810369[0] = 1;
   out_479921767714810369[1] = 0;
   out_479921767714810369[2] = 0;
   out_479921767714810369[3] = 0;
   out_479921767714810369[4] = 0;
   out_479921767714810369[5] = 0;
   out_479921767714810369[6] = 0;
   out_479921767714810369[7] = 0;
   out_479921767714810369[8] = 0;
}
void h_31(double *state, double *unused, double *out_16812798867285533) {
   out_16812798867285533[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2564402443839660560) {
   out_2564402443839660560[0] = 0;
   out_2564402443839660560[1] = 0;
   out_2564402443839660560[2] = 0;
   out_2564402443839660560[3] = 0;
   out_2564402443839660560[4] = 0;
   out_2564402443839660560[5] = 0;
   out_2564402443839660560[6] = 0;
   out_2564402443839660560[7] = 0;
   out_2564402443839660560[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_4008055473606692387) {
  err_fun(nom_x, delta_x, out_4008055473606692387);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6014015634177356234) {
  inv_err_fun(nom_x, true_x, out_6014015634177356234);
}
void car_H_mod_fun(double *state, double *out_2369935343221964708) {
  H_mod_fun(state, out_2369935343221964708);
}
void car_f_fun(double *state, double dt, double *out_1926748051812530846) {
  f_fun(state,  dt, out_1926748051812530846);
}
void car_F_fun(double *state, double dt, double *out_6644089939752104464) {
  F_fun(state,  dt, out_6644089939752104464);
}
void car_h_25(double *state, double *unused, double *out_258381263417220356) {
  h_25(state, unused, out_258381263417220356);
}
void car_H_25(double *state, double *unused, double *out_2533756481962700132) {
  H_25(state, unused, out_2533756481962700132);
}
void car_h_24(double *state, double *unused, double *out_6738890471142770327) {
  h_24(state, unused, out_6738890471142770327);
}
void car_H_24(double *state, double *unused, double *out_7807250764875712726) {
  H_24(state, unused, out_7807250764875712726);
}
void car_h_30(double *state, double *unused, double *out_3206107253772705317) {
  h_30(state, unused, out_3206107253772705317);
}
void car_H_30(double *state, double *unused, double *out_5052089440469948759) {
  H_30(state, unused, out_5052089440469948759);
}
void car_h_26(double *state, double *unused, double *out_4149617106674367752) {
  h_26(state, unused, out_4149617106674367752);
}
void car_H_26(double *state, double *unused, double *out_1207746836911356092) {
  H_26(state, unused, out_1207746836911356092);
}
void car_h_27(double *state, double *unused, double *out_2778404788027112329) {
  h_27(state, unused, out_2778404788027112329);
}
void car_H_27(double *state, double *unused, double *out_229654223019035151) {
  H_27(state, unused, out_229654223019035151);
}
void car_h_29(double *state, double *unused, double *out_8207780521433550962) {
  h_29(state, unused, out_8207780521433550962);
}
void car_H_29(double *state, double *unused, double *out_1483708503850515882) {
  H_29(state, unused, out_1483708503850515882);
}
void car_h_28(double *state, double *unused, double *out_2223268479633733316) {
  h_28(state, unused, out_2223268479633733316);
}
void car_H_28(double *state, double *unused, double *out_479921767714810369) {
  H_28(state, unused, out_479921767714810369);
}
void car_h_31(double *state, double *unused, double *out_16812798867285533) {
  h_31(state, unused, out_16812798867285533);
}
void car_H_31(double *state, double *unused, double *out_2564402443839660560) {
  H_31(state, unused, out_2564402443839660560);
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
