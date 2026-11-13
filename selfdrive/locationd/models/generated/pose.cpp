#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_5580352609619235680) {
   out_5580352609619235680[0] = delta_x[0] + nom_x[0];
   out_5580352609619235680[1] = delta_x[1] + nom_x[1];
   out_5580352609619235680[2] = delta_x[2] + nom_x[2];
   out_5580352609619235680[3] = delta_x[3] + nom_x[3];
   out_5580352609619235680[4] = delta_x[4] + nom_x[4];
   out_5580352609619235680[5] = delta_x[5] + nom_x[5];
   out_5580352609619235680[6] = delta_x[6] + nom_x[6];
   out_5580352609619235680[7] = delta_x[7] + nom_x[7];
   out_5580352609619235680[8] = delta_x[8] + nom_x[8];
   out_5580352609619235680[9] = delta_x[9] + nom_x[9];
   out_5580352609619235680[10] = delta_x[10] + nom_x[10];
   out_5580352609619235680[11] = delta_x[11] + nom_x[11];
   out_5580352609619235680[12] = delta_x[12] + nom_x[12];
   out_5580352609619235680[13] = delta_x[13] + nom_x[13];
   out_5580352609619235680[14] = delta_x[14] + nom_x[14];
   out_5580352609619235680[15] = delta_x[15] + nom_x[15];
   out_5580352609619235680[16] = delta_x[16] + nom_x[16];
   out_5580352609619235680[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_977978120678855025) {
   out_977978120678855025[0] = -nom_x[0] + true_x[0];
   out_977978120678855025[1] = -nom_x[1] + true_x[1];
   out_977978120678855025[2] = -nom_x[2] + true_x[2];
   out_977978120678855025[3] = -nom_x[3] + true_x[3];
   out_977978120678855025[4] = -nom_x[4] + true_x[4];
   out_977978120678855025[5] = -nom_x[5] + true_x[5];
   out_977978120678855025[6] = -nom_x[6] + true_x[6];
   out_977978120678855025[7] = -nom_x[7] + true_x[7];
   out_977978120678855025[8] = -nom_x[8] + true_x[8];
   out_977978120678855025[9] = -nom_x[9] + true_x[9];
   out_977978120678855025[10] = -nom_x[10] + true_x[10];
   out_977978120678855025[11] = -nom_x[11] + true_x[11];
   out_977978120678855025[12] = -nom_x[12] + true_x[12];
   out_977978120678855025[13] = -nom_x[13] + true_x[13];
   out_977978120678855025[14] = -nom_x[14] + true_x[14];
   out_977978120678855025[15] = -nom_x[15] + true_x[15];
   out_977978120678855025[16] = -nom_x[16] + true_x[16];
   out_977978120678855025[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_6079805720435821411) {
   out_6079805720435821411[0] = 1.0;
   out_6079805720435821411[1] = 0.0;
   out_6079805720435821411[2] = 0.0;
   out_6079805720435821411[3] = 0.0;
   out_6079805720435821411[4] = 0.0;
   out_6079805720435821411[5] = 0.0;
   out_6079805720435821411[6] = 0.0;
   out_6079805720435821411[7] = 0.0;
   out_6079805720435821411[8] = 0.0;
   out_6079805720435821411[9] = 0.0;
   out_6079805720435821411[10] = 0.0;
   out_6079805720435821411[11] = 0.0;
   out_6079805720435821411[12] = 0.0;
   out_6079805720435821411[13] = 0.0;
   out_6079805720435821411[14] = 0.0;
   out_6079805720435821411[15] = 0.0;
   out_6079805720435821411[16] = 0.0;
   out_6079805720435821411[17] = 0.0;
   out_6079805720435821411[18] = 0.0;
   out_6079805720435821411[19] = 1.0;
   out_6079805720435821411[20] = 0.0;
   out_6079805720435821411[21] = 0.0;
   out_6079805720435821411[22] = 0.0;
   out_6079805720435821411[23] = 0.0;
   out_6079805720435821411[24] = 0.0;
   out_6079805720435821411[25] = 0.0;
   out_6079805720435821411[26] = 0.0;
   out_6079805720435821411[27] = 0.0;
   out_6079805720435821411[28] = 0.0;
   out_6079805720435821411[29] = 0.0;
   out_6079805720435821411[30] = 0.0;
   out_6079805720435821411[31] = 0.0;
   out_6079805720435821411[32] = 0.0;
   out_6079805720435821411[33] = 0.0;
   out_6079805720435821411[34] = 0.0;
   out_6079805720435821411[35] = 0.0;
   out_6079805720435821411[36] = 0.0;
   out_6079805720435821411[37] = 0.0;
   out_6079805720435821411[38] = 1.0;
   out_6079805720435821411[39] = 0.0;
   out_6079805720435821411[40] = 0.0;
   out_6079805720435821411[41] = 0.0;
   out_6079805720435821411[42] = 0.0;
   out_6079805720435821411[43] = 0.0;
   out_6079805720435821411[44] = 0.0;
   out_6079805720435821411[45] = 0.0;
   out_6079805720435821411[46] = 0.0;
   out_6079805720435821411[47] = 0.0;
   out_6079805720435821411[48] = 0.0;
   out_6079805720435821411[49] = 0.0;
   out_6079805720435821411[50] = 0.0;
   out_6079805720435821411[51] = 0.0;
   out_6079805720435821411[52] = 0.0;
   out_6079805720435821411[53] = 0.0;
   out_6079805720435821411[54] = 0.0;
   out_6079805720435821411[55] = 0.0;
   out_6079805720435821411[56] = 0.0;
   out_6079805720435821411[57] = 1.0;
   out_6079805720435821411[58] = 0.0;
   out_6079805720435821411[59] = 0.0;
   out_6079805720435821411[60] = 0.0;
   out_6079805720435821411[61] = 0.0;
   out_6079805720435821411[62] = 0.0;
   out_6079805720435821411[63] = 0.0;
   out_6079805720435821411[64] = 0.0;
   out_6079805720435821411[65] = 0.0;
   out_6079805720435821411[66] = 0.0;
   out_6079805720435821411[67] = 0.0;
   out_6079805720435821411[68] = 0.0;
   out_6079805720435821411[69] = 0.0;
   out_6079805720435821411[70] = 0.0;
   out_6079805720435821411[71] = 0.0;
   out_6079805720435821411[72] = 0.0;
   out_6079805720435821411[73] = 0.0;
   out_6079805720435821411[74] = 0.0;
   out_6079805720435821411[75] = 0.0;
   out_6079805720435821411[76] = 1.0;
   out_6079805720435821411[77] = 0.0;
   out_6079805720435821411[78] = 0.0;
   out_6079805720435821411[79] = 0.0;
   out_6079805720435821411[80] = 0.0;
   out_6079805720435821411[81] = 0.0;
   out_6079805720435821411[82] = 0.0;
   out_6079805720435821411[83] = 0.0;
   out_6079805720435821411[84] = 0.0;
   out_6079805720435821411[85] = 0.0;
   out_6079805720435821411[86] = 0.0;
   out_6079805720435821411[87] = 0.0;
   out_6079805720435821411[88] = 0.0;
   out_6079805720435821411[89] = 0.0;
   out_6079805720435821411[90] = 0.0;
   out_6079805720435821411[91] = 0.0;
   out_6079805720435821411[92] = 0.0;
   out_6079805720435821411[93] = 0.0;
   out_6079805720435821411[94] = 0.0;
   out_6079805720435821411[95] = 1.0;
   out_6079805720435821411[96] = 0.0;
   out_6079805720435821411[97] = 0.0;
   out_6079805720435821411[98] = 0.0;
   out_6079805720435821411[99] = 0.0;
   out_6079805720435821411[100] = 0.0;
   out_6079805720435821411[101] = 0.0;
   out_6079805720435821411[102] = 0.0;
   out_6079805720435821411[103] = 0.0;
   out_6079805720435821411[104] = 0.0;
   out_6079805720435821411[105] = 0.0;
   out_6079805720435821411[106] = 0.0;
   out_6079805720435821411[107] = 0.0;
   out_6079805720435821411[108] = 0.0;
   out_6079805720435821411[109] = 0.0;
   out_6079805720435821411[110] = 0.0;
   out_6079805720435821411[111] = 0.0;
   out_6079805720435821411[112] = 0.0;
   out_6079805720435821411[113] = 0.0;
   out_6079805720435821411[114] = 1.0;
   out_6079805720435821411[115] = 0.0;
   out_6079805720435821411[116] = 0.0;
   out_6079805720435821411[117] = 0.0;
   out_6079805720435821411[118] = 0.0;
   out_6079805720435821411[119] = 0.0;
   out_6079805720435821411[120] = 0.0;
   out_6079805720435821411[121] = 0.0;
   out_6079805720435821411[122] = 0.0;
   out_6079805720435821411[123] = 0.0;
   out_6079805720435821411[124] = 0.0;
   out_6079805720435821411[125] = 0.0;
   out_6079805720435821411[126] = 0.0;
   out_6079805720435821411[127] = 0.0;
   out_6079805720435821411[128] = 0.0;
   out_6079805720435821411[129] = 0.0;
   out_6079805720435821411[130] = 0.0;
   out_6079805720435821411[131] = 0.0;
   out_6079805720435821411[132] = 0.0;
   out_6079805720435821411[133] = 1.0;
   out_6079805720435821411[134] = 0.0;
   out_6079805720435821411[135] = 0.0;
   out_6079805720435821411[136] = 0.0;
   out_6079805720435821411[137] = 0.0;
   out_6079805720435821411[138] = 0.0;
   out_6079805720435821411[139] = 0.0;
   out_6079805720435821411[140] = 0.0;
   out_6079805720435821411[141] = 0.0;
   out_6079805720435821411[142] = 0.0;
   out_6079805720435821411[143] = 0.0;
   out_6079805720435821411[144] = 0.0;
   out_6079805720435821411[145] = 0.0;
   out_6079805720435821411[146] = 0.0;
   out_6079805720435821411[147] = 0.0;
   out_6079805720435821411[148] = 0.0;
   out_6079805720435821411[149] = 0.0;
   out_6079805720435821411[150] = 0.0;
   out_6079805720435821411[151] = 0.0;
   out_6079805720435821411[152] = 1.0;
   out_6079805720435821411[153] = 0.0;
   out_6079805720435821411[154] = 0.0;
   out_6079805720435821411[155] = 0.0;
   out_6079805720435821411[156] = 0.0;
   out_6079805720435821411[157] = 0.0;
   out_6079805720435821411[158] = 0.0;
   out_6079805720435821411[159] = 0.0;
   out_6079805720435821411[160] = 0.0;
   out_6079805720435821411[161] = 0.0;
   out_6079805720435821411[162] = 0.0;
   out_6079805720435821411[163] = 0.0;
   out_6079805720435821411[164] = 0.0;
   out_6079805720435821411[165] = 0.0;
   out_6079805720435821411[166] = 0.0;
   out_6079805720435821411[167] = 0.0;
   out_6079805720435821411[168] = 0.0;
   out_6079805720435821411[169] = 0.0;
   out_6079805720435821411[170] = 0.0;
   out_6079805720435821411[171] = 1.0;
   out_6079805720435821411[172] = 0.0;
   out_6079805720435821411[173] = 0.0;
   out_6079805720435821411[174] = 0.0;
   out_6079805720435821411[175] = 0.0;
   out_6079805720435821411[176] = 0.0;
   out_6079805720435821411[177] = 0.0;
   out_6079805720435821411[178] = 0.0;
   out_6079805720435821411[179] = 0.0;
   out_6079805720435821411[180] = 0.0;
   out_6079805720435821411[181] = 0.0;
   out_6079805720435821411[182] = 0.0;
   out_6079805720435821411[183] = 0.0;
   out_6079805720435821411[184] = 0.0;
   out_6079805720435821411[185] = 0.0;
   out_6079805720435821411[186] = 0.0;
   out_6079805720435821411[187] = 0.0;
   out_6079805720435821411[188] = 0.0;
   out_6079805720435821411[189] = 0.0;
   out_6079805720435821411[190] = 1.0;
   out_6079805720435821411[191] = 0.0;
   out_6079805720435821411[192] = 0.0;
   out_6079805720435821411[193] = 0.0;
   out_6079805720435821411[194] = 0.0;
   out_6079805720435821411[195] = 0.0;
   out_6079805720435821411[196] = 0.0;
   out_6079805720435821411[197] = 0.0;
   out_6079805720435821411[198] = 0.0;
   out_6079805720435821411[199] = 0.0;
   out_6079805720435821411[200] = 0.0;
   out_6079805720435821411[201] = 0.0;
   out_6079805720435821411[202] = 0.0;
   out_6079805720435821411[203] = 0.0;
   out_6079805720435821411[204] = 0.0;
   out_6079805720435821411[205] = 0.0;
   out_6079805720435821411[206] = 0.0;
   out_6079805720435821411[207] = 0.0;
   out_6079805720435821411[208] = 0.0;
   out_6079805720435821411[209] = 1.0;
   out_6079805720435821411[210] = 0.0;
   out_6079805720435821411[211] = 0.0;
   out_6079805720435821411[212] = 0.0;
   out_6079805720435821411[213] = 0.0;
   out_6079805720435821411[214] = 0.0;
   out_6079805720435821411[215] = 0.0;
   out_6079805720435821411[216] = 0.0;
   out_6079805720435821411[217] = 0.0;
   out_6079805720435821411[218] = 0.0;
   out_6079805720435821411[219] = 0.0;
   out_6079805720435821411[220] = 0.0;
   out_6079805720435821411[221] = 0.0;
   out_6079805720435821411[222] = 0.0;
   out_6079805720435821411[223] = 0.0;
   out_6079805720435821411[224] = 0.0;
   out_6079805720435821411[225] = 0.0;
   out_6079805720435821411[226] = 0.0;
   out_6079805720435821411[227] = 0.0;
   out_6079805720435821411[228] = 1.0;
   out_6079805720435821411[229] = 0.0;
   out_6079805720435821411[230] = 0.0;
   out_6079805720435821411[231] = 0.0;
   out_6079805720435821411[232] = 0.0;
   out_6079805720435821411[233] = 0.0;
   out_6079805720435821411[234] = 0.0;
   out_6079805720435821411[235] = 0.0;
   out_6079805720435821411[236] = 0.0;
   out_6079805720435821411[237] = 0.0;
   out_6079805720435821411[238] = 0.0;
   out_6079805720435821411[239] = 0.0;
   out_6079805720435821411[240] = 0.0;
   out_6079805720435821411[241] = 0.0;
   out_6079805720435821411[242] = 0.0;
   out_6079805720435821411[243] = 0.0;
   out_6079805720435821411[244] = 0.0;
   out_6079805720435821411[245] = 0.0;
   out_6079805720435821411[246] = 0.0;
   out_6079805720435821411[247] = 1.0;
   out_6079805720435821411[248] = 0.0;
   out_6079805720435821411[249] = 0.0;
   out_6079805720435821411[250] = 0.0;
   out_6079805720435821411[251] = 0.0;
   out_6079805720435821411[252] = 0.0;
   out_6079805720435821411[253] = 0.0;
   out_6079805720435821411[254] = 0.0;
   out_6079805720435821411[255] = 0.0;
   out_6079805720435821411[256] = 0.0;
   out_6079805720435821411[257] = 0.0;
   out_6079805720435821411[258] = 0.0;
   out_6079805720435821411[259] = 0.0;
   out_6079805720435821411[260] = 0.0;
   out_6079805720435821411[261] = 0.0;
   out_6079805720435821411[262] = 0.0;
   out_6079805720435821411[263] = 0.0;
   out_6079805720435821411[264] = 0.0;
   out_6079805720435821411[265] = 0.0;
   out_6079805720435821411[266] = 1.0;
   out_6079805720435821411[267] = 0.0;
   out_6079805720435821411[268] = 0.0;
   out_6079805720435821411[269] = 0.0;
   out_6079805720435821411[270] = 0.0;
   out_6079805720435821411[271] = 0.0;
   out_6079805720435821411[272] = 0.0;
   out_6079805720435821411[273] = 0.0;
   out_6079805720435821411[274] = 0.0;
   out_6079805720435821411[275] = 0.0;
   out_6079805720435821411[276] = 0.0;
   out_6079805720435821411[277] = 0.0;
   out_6079805720435821411[278] = 0.0;
   out_6079805720435821411[279] = 0.0;
   out_6079805720435821411[280] = 0.0;
   out_6079805720435821411[281] = 0.0;
   out_6079805720435821411[282] = 0.0;
   out_6079805720435821411[283] = 0.0;
   out_6079805720435821411[284] = 0.0;
   out_6079805720435821411[285] = 1.0;
   out_6079805720435821411[286] = 0.0;
   out_6079805720435821411[287] = 0.0;
   out_6079805720435821411[288] = 0.0;
   out_6079805720435821411[289] = 0.0;
   out_6079805720435821411[290] = 0.0;
   out_6079805720435821411[291] = 0.0;
   out_6079805720435821411[292] = 0.0;
   out_6079805720435821411[293] = 0.0;
   out_6079805720435821411[294] = 0.0;
   out_6079805720435821411[295] = 0.0;
   out_6079805720435821411[296] = 0.0;
   out_6079805720435821411[297] = 0.0;
   out_6079805720435821411[298] = 0.0;
   out_6079805720435821411[299] = 0.0;
   out_6079805720435821411[300] = 0.0;
   out_6079805720435821411[301] = 0.0;
   out_6079805720435821411[302] = 0.0;
   out_6079805720435821411[303] = 0.0;
   out_6079805720435821411[304] = 1.0;
   out_6079805720435821411[305] = 0.0;
   out_6079805720435821411[306] = 0.0;
   out_6079805720435821411[307] = 0.0;
   out_6079805720435821411[308] = 0.0;
   out_6079805720435821411[309] = 0.0;
   out_6079805720435821411[310] = 0.0;
   out_6079805720435821411[311] = 0.0;
   out_6079805720435821411[312] = 0.0;
   out_6079805720435821411[313] = 0.0;
   out_6079805720435821411[314] = 0.0;
   out_6079805720435821411[315] = 0.0;
   out_6079805720435821411[316] = 0.0;
   out_6079805720435821411[317] = 0.0;
   out_6079805720435821411[318] = 0.0;
   out_6079805720435821411[319] = 0.0;
   out_6079805720435821411[320] = 0.0;
   out_6079805720435821411[321] = 0.0;
   out_6079805720435821411[322] = 0.0;
   out_6079805720435821411[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_600860468097978335) {
   out_600860468097978335[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_600860468097978335[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_600860468097978335[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_600860468097978335[3] = dt*state[12] + state[3];
   out_600860468097978335[4] = dt*state[13] + state[4];
   out_600860468097978335[5] = dt*state[14] + state[5];
   out_600860468097978335[6] = state[6];
   out_600860468097978335[7] = state[7];
   out_600860468097978335[8] = state[8];
   out_600860468097978335[9] = state[9];
   out_600860468097978335[10] = state[10];
   out_600860468097978335[11] = state[11];
   out_600860468097978335[12] = state[12];
   out_600860468097978335[13] = state[13];
   out_600860468097978335[14] = state[14];
   out_600860468097978335[15] = state[15];
   out_600860468097978335[16] = state[16];
   out_600860468097978335[17] = state[17];
}
void F_fun(double *state, double dt, double *out_1300580827509148386) {
   out_1300580827509148386[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1300580827509148386[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1300580827509148386[2] = 0;
   out_1300580827509148386[3] = 0;
   out_1300580827509148386[4] = 0;
   out_1300580827509148386[5] = 0;
   out_1300580827509148386[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1300580827509148386[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1300580827509148386[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1300580827509148386[9] = 0;
   out_1300580827509148386[10] = 0;
   out_1300580827509148386[11] = 0;
   out_1300580827509148386[12] = 0;
   out_1300580827509148386[13] = 0;
   out_1300580827509148386[14] = 0;
   out_1300580827509148386[15] = 0;
   out_1300580827509148386[16] = 0;
   out_1300580827509148386[17] = 0;
   out_1300580827509148386[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1300580827509148386[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1300580827509148386[20] = 0;
   out_1300580827509148386[21] = 0;
   out_1300580827509148386[22] = 0;
   out_1300580827509148386[23] = 0;
   out_1300580827509148386[24] = 0;
   out_1300580827509148386[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1300580827509148386[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1300580827509148386[27] = 0;
   out_1300580827509148386[28] = 0;
   out_1300580827509148386[29] = 0;
   out_1300580827509148386[30] = 0;
   out_1300580827509148386[31] = 0;
   out_1300580827509148386[32] = 0;
   out_1300580827509148386[33] = 0;
   out_1300580827509148386[34] = 0;
   out_1300580827509148386[35] = 0;
   out_1300580827509148386[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1300580827509148386[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1300580827509148386[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1300580827509148386[39] = 0;
   out_1300580827509148386[40] = 0;
   out_1300580827509148386[41] = 0;
   out_1300580827509148386[42] = 0;
   out_1300580827509148386[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1300580827509148386[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1300580827509148386[45] = 0;
   out_1300580827509148386[46] = 0;
   out_1300580827509148386[47] = 0;
   out_1300580827509148386[48] = 0;
   out_1300580827509148386[49] = 0;
   out_1300580827509148386[50] = 0;
   out_1300580827509148386[51] = 0;
   out_1300580827509148386[52] = 0;
   out_1300580827509148386[53] = 0;
   out_1300580827509148386[54] = 0;
   out_1300580827509148386[55] = 0;
   out_1300580827509148386[56] = 0;
   out_1300580827509148386[57] = 1;
   out_1300580827509148386[58] = 0;
   out_1300580827509148386[59] = 0;
   out_1300580827509148386[60] = 0;
   out_1300580827509148386[61] = 0;
   out_1300580827509148386[62] = 0;
   out_1300580827509148386[63] = 0;
   out_1300580827509148386[64] = 0;
   out_1300580827509148386[65] = 0;
   out_1300580827509148386[66] = dt;
   out_1300580827509148386[67] = 0;
   out_1300580827509148386[68] = 0;
   out_1300580827509148386[69] = 0;
   out_1300580827509148386[70] = 0;
   out_1300580827509148386[71] = 0;
   out_1300580827509148386[72] = 0;
   out_1300580827509148386[73] = 0;
   out_1300580827509148386[74] = 0;
   out_1300580827509148386[75] = 0;
   out_1300580827509148386[76] = 1;
   out_1300580827509148386[77] = 0;
   out_1300580827509148386[78] = 0;
   out_1300580827509148386[79] = 0;
   out_1300580827509148386[80] = 0;
   out_1300580827509148386[81] = 0;
   out_1300580827509148386[82] = 0;
   out_1300580827509148386[83] = 0;
   out_1300580827509148386[84] = 0;
   out_1300580827509148386[85] = dt;
   out_1300580827509148386[86] = 0;
   out_1300580827509148386[87] = 0;
   out_1300580827509148386[88] = 0;
   out_1300580827509148386[89] = 0;
   out_1300580827509148386[90] = 0;
   out_1300580827509148386[91] = 0;
   out_1300580827509148386[92] = 0;
   out_1300580827509148386[93] = 0;
   out_1300580827509148386[94] = 0;
   out_1300580827509148386[95] = 1;
   out_1300580827509148386[96] = 0;
   out_1300580827509148386[97] = 0;
   out_1300580827509148386[98] = 0;
   out_1300580827509148386[99] = 0;
   out_1300580827509148386[100] = 0;
   out_1300580827509148386[101] = 0;
   out_1300580827509148386[102] = 0;
   out_1300580827509148386[103] = 0;
   out_1300580827509148386[104] = dt;
   out_1300580827509148386[105] = 0;
   out_1300580827509148386[106] = 0;
   out_1300580827509148386[107] = 0;
   out_1300580827509148386[108] = 0;
   out_1300580827509148386[109] = 0;
   out_1300580827509148386[110] = 0;
   out_1300580827509148386[111] = 0;
   out_1300580827509148386[112] = 0;
   out_1300580827509148386[113] = 0;
   out_1300580827509148386[114] = 1;
   out_1300580827509148386[115] = 0;
   out_1300580827509148386[116] = 0;
   out_1300580827509148386[117] = 0;
   out_1300580827509148386[118] = 0;
   out_1300580827509148386[119] = 0;
   out_1300580827509148386[120] = 0;
   out_1300580827509148386[121] = 0;
   out_1300580827509148386[122] = 0;
   out_1300580827509148386[123] = 0;
   out_1300580827509148386[124] = 0;
   out_1300580827509148386[125] = 0;
   out_1300580827509148386[126] = 0;
   out_1300580827509148386[127] = 0;
   out_1300580827509148386[128] = 0;
   out_1300580827509148386[129] = 0;
   out_1300580827509148386[130] = 0;
   out_1300580827509148386[131] = 0;
   out_1300580827509148386[132] = 0;
   out_1300580827509148386[133] = 1;
   out_1300580827509148386[134] = 0;
   out_1300580827509148386[135] = 0;
   out_1300580827509148386[136] = 0;
   out_1300580827509148386[137] = 0;
   out_1300580827509148386[138] = 0;
   out_1300580827509148386[139] = 0;
   out_1300580827509148386[140] = 0;
   out_1300580827509148386[141] = 0;
   out_1300580827509148386[142] = 0;
   out_1300580827509148386[143] = 0;
   out_1300580827509148386[144] = 0;
   out_1300580827509148386[145] = 0;
   out_1300580827509148386[146] = 0;
   out_1300580827509148386[147] = 0;
   out_1300580827509148386[148] = 0;
   out_1300580827509148386[149] = 0;
   out_1300580827509148386[150] = 0;
   out_1300580827509148386[151] = 0;
   out_1300580827509148386[152] = 1;
   out_1300580827509148386[153] = 0;
   out_1300580827509148386[154] = 0;
   out_1300580827509148386[155] = 0;
   out_1300580827509148386[156] = 0;
   out_1300580827509148386[157] = 0;
   out_1300580827509148386[158] = 0;
   out_1300580827509148386[159] = 0;
   out_1300580827509148386[160] = 0;
   out_1300580827509148386[161] = 0;
   out_1300580827509148386[162] = 0;
   out_1300580827509148386[163] = 0;
   out_1300580827509148386[164] = 0;
   out_1300580827509148386[165] = 0;
   out_1300580827509148386[166] = 0;
   out_1300580827509148386[167] = 0;
   out_1300580827509148386[168] = 0;
   out_1300580827509148386[169] = 0;
   out_1300580827509148386[170] = 0;
   out_1300580827509148386[171] = 1;
   out_1300580827509148386[172] = 0;
   out_1300580827509148386[173] = 0;
   out_1300580827509148386[174] = 0;
   out_1300580827509148386[175] = 0;
   out_1300580827509148386[176] = 0;
   out_1300580827509148386[177] = 0;
   out_1300580827509148386[178] = 0;
   out_1300580827509148386[179] = 0;
   out_1300580827509148386[180] = 0;
   out_1300580827509148386[181] = 0;
   out_1300580827509148386[182] = 0;
   out_1300580827509148386[183] = 0;
   out_1300580827509148386[184] = 0;
   out_1300580827509148386[185] = 0;
   out_1300580827509148386[186] = 0;
   out_1300580827509148386[187] = 0;
   out_1300580827509148386[188] = 0;
   out_1300580827509148386[189] = 0;
   out_1300580827509148386[190] = 1;
   out_1300580827509148386[191] = 0;
   out_1300580827509148386[192] = 0;
   out_1300580827509148386[193] = 0;
   out_1300580827509148386[194] = 0;
   out_1300580827509148386[195] = 0;
   out_1300580827509148386[196] = 0;
   out_1300580827509148386[197] = 0;
   out_1300580827509148386[198] = 0;
   out_1300580827509148386[199] = 0;
   out_1300580827509148386[200] = 0;
   out_1300580827509148386[201] = 0;
   out_1300580827509148386[202] = 0;
   out_1300580827509148386[203] = 0;
   out_1300580827509148386[204] = 0;
   out_1300580827509148386[205] = 0;
   out_1300580827509148386[206] = 0;
   out_1300580827509148386[207] = 0;
   out_1300580827509148386[208] = 0;
   out_1300580827509148386[209] = 1;
   out_1300580827509148386[210] = 0;
   out_1300580827509148386[211] = 0;
   out_1300580827509148386[212] = 0;
   out_1300580827509148386[213] = 0;
   out_1300580827509148386[214] = 0;
   out_1300580827509148386[215] = 0;
   out_1300580827509148386[216] = 0;
   out_1300580827509148386[217] = 0;
   out_1300580827509148386[218] = 0;
   out_1300580827509148386[219] = 0;
   out_1300580827509148386[220] = 0;
   out_1300580827509148386[221] = 0;
   out_1300580827509148386[222] = 0;
   out_1300580827509148386[223] = 0;
   out_1300580827509148386[224] = 0;
   out_1300580827509148386[225] = 0;
   out_1300580827509148386[226] = 0;
   out_1300580827509148386[227] = 0;
   out_1300580827509148386[228] = 1;
   out_1300580827509148386[229] = 0;
   out_1300580827509148386[230] = 0;
   out_1300580827509148386[231] = 0;
   out_1300580827509148386[232] = 0;
   out_1300580827509148386[233] = 0;
   out_1300580827509148386[234] = 0;
   out_1300580827509148386[235] = 0;
   out_1300580827509148386[236] = 0;
   out_1300580827509148386[237] = 0;
   out_1300580827509148386[238] = 0;
   out_1300580827509148386[239] = 0;
   out_1300580827509148386[240] = 0;
   out_1300580827509148386[241] = 0;
   out_1300580827509148386[242] = 0;
   out_1300580827509148386[243] = 0;
   out_1300580827509148386[244] = 0;
   out_1300580827509148386[245] = 0;
   out_1300580827509148386[246] = 0;
   out_1300580827509148386[247] = 1;
   out_1300580827509148386[248] = 0;
   out_1300580827509148386[249] = 0;
   out_1300580827509148386[250] = 0;
   out_1300580827509148386[251] = 0;
   out_1300580827509148386[252] = 0;
   out_1300580827509148386[253] = 0;
   out_1300580827509148386[254] = 0;
   out_1300580827509148386[255] = 0;
   out_1300580827509148386[256] = 0;
   out_1300580827509148386[257] = 0;
   out_1300580827509148386[258] = 0;
   out_1300580827509148386[259] = 0;
   out_1300580827509148386[260] = 0;
   out_1300580827509148386[261] = 0;
   out_1300580827509148386[262] = 0;
   out_1300580827509148386[263] = 0;
   out_1300580827509148386[264] = 0;
   out_1300580827509148386[265] = 0;
   out_1300580827509148386[266] = 1;
   out_1300580827509148386[267] = 0;
   out_1300580827509148386[268] = 0;
   out_1300580827509148386[269] = 0;
   out_1300580827509148386[270] = 0;
   out_1300580827509148386[271] = 0;
   out_1300580827509148386[272] = 0;
   out_1300580827509148386[273] = 0;
   out_1300580827509148386[274] = 0;
   out_1300580827509148386[275] = 0;
   out_1300580827509148386[276] = 0;
   out_1300580827509148386[277] = 0;
   out_1300580827509148386[278] = 0;
   out_1300580827509148386[279] = 0;
   out_1300580827509148386[280] = 0;
   out_1300580827509148386[281] = 0;
   out_1300580827509148386[282] = 0;
   out_1300580827509148386[283] = 0;
   out_1300580827509148386[284] = 0;
   out_1300580827509148386[285] = 1;
   out_1300580827509148386[286] = 0;
   out_1300580827509148386[287] = 0;
   out_1300580827509148386[288] = 0;
   out_1300580827509148386[289] = 0;
   out_1300580827509148386[290] = 0;
   out_1300580827509148386[291] = 0;
   out_1300580827509148386[292] = 0;
   out_1300580827509148386[293] = 0;
   out_1300580827509148386[294] = 0;
   out_1300580827509148386[295] = 0;
   out_1300580827509148386[296] = 0;
   out_1300580827509148386[297] = 0;
   out_1300580827509148386[298] = 0;
   out_1300580827509148386[299] = 0;
   out_1300580827509148386[300] = 0;
   out_1300580827509148386[301] = 0;
   out_1300580827509148386[302] = 0;
   out_1300580827509148386[303] = 0;
   out_1300580827509148386[304] = 1;
   out_1300580827509148386[305] = 0;
   out_1300580827509148386[306] = 0;
   out_1300580827509148386[307] = 0;
   out_1300580827509148386[308] = 0;
   out_1300580827509148386[309] = 0;
   out_1300580827509148386[310] = 0;
   out_1300580827509148386[311] = 0;
   out_1300580827509148386[312] = 0;
   out_1300580827509148386[313] = 0;
   out_1300580827509148386[314] = 0;
   out_1300580827509148386[315] = 0;
   out_1300580827509148386[316] = 0;
   out_1300580827509148386[317] = 0;
   out_1300580827509148386[318] = 0;
   out_1300580827509148386[319] = 0;
   out_1300580827509148386[320] = 0;
   out_1300580827509148386[321] = 0;
   out_1300580827509148386[322] = 0;
   out_1300580827509148386[323] = 1;
}
void h_4(double *state, double *unused, double *out_1401553991619916962) {
   out_1401553991619916962[0] = state[6] + state[9];
   out_1401553991619916962[1] = state[7] + state[10];
   out_1401553991619916962[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_6518856546521771484) {
   out_6518856546521771484[0] = 0;
   out_6518856546521771484[1] = 0;
   out_6518856546521771484[2] = 0;
   out_6518856546521771484[3] = 0;
   out_6518856546521771484[4] = 0;
   out_6518856546521771484[5] = 0;
   out_6518856546521771484[6] = 1;
   out_6518856546521771484[7] = 0;
   out_6518856546521771484[8] = 0;
   out_6518856546521771484[9] = 1;
   out_6518856546521771484[10] = 0;
   out_6518856546521771484[11] = 0;
   out_6518856546521771484[12] = 0;
   out_6518856546521771484[13] = 0;
   out_6518856546521771484[14] = 0;
   out_6518856546521771484[15] = 0;
   out_6518856546521771484[16] = 0;
   out_6518856546521771484[17] = 0;
   out_6518856546521771484[18] = 0;
   out_6518856546521771484[19] = 0;
   out_6518856546521771484[20] = 0;
   out_6518856546521771484[21] = 0;
   out_6518856546521771484[22] = 0;
   out_6518856546521771484[23] = 0;
   out_6518856546521771484[24] = 0;
   out_6518856546521771484[25] = 1;
   out_6518856546521771484[26] = 0;
   out_6518856546521771484[27] = 0;
   out_6518856546521771484[28] = 1;
   out_6518856546521771484[29] = 0;
   out_6518856546521771484[30] = 0;
   out_6518856546521771484[31] = 0;
   out_6518856546521771484[32] = 0;
   out_6518856546521771484[33] = 0;
   out_6518856546521771484[34] = 0;
   out_6518856546521771484[35] = 0;
   out_6518856546521771484[36] = 0;
   out_6518856546521771484[37] = 0;
   out_6518856546521771484[38] = 0;
   out_6518856546521771484[39] = 0;
   out_6518856546521771484[40] = 0;
   out_6518856546521771484[41] = 0;
   out_6518856546521771484[42] = 0;
   out_6518856546521771484[43] = 0;
   out_6518856546521771484[44] = 1;
   out_6518856546521771484[45] = 0;
   out_6518856546521771484[46] = 0;
   out_6518856546521771484[47] = 1;
   out_6518856546521771484[48] = 0;
   out_6518856546521771484[49] = 0;
   out_6518856546521771484[50] = 0;
   out_6518856546521771484[51] = 0;
   out_6518856546521771484[52] = 0;
   out_6518856546521771484[53] = 0;
}
void h_10(double *state, double *unused, double *out_2150734223135501944) {
   out_2150734223135501944[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_2150734223135501944[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_2150734223135501944[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5900386540209642427) {
   out_5900386540209642427[0] = 0;
   out_5900386540209642427[1] = 9.8100000000000005*cos(state[1]);
   out_5900386540209642427[2] = 0;
   out_5900386540209642427[3] = 0;
   out_5900386540209642427[4] = -state[8];
   out_5900386540209642427[5] = state[7];
   out_5900386540209642427[6] = 0;
   out_5900386540209642427[7] = state[5];
   out_5900386540209642427[8] = -state[4];
   out_5900386540209642427[9] = 0;
   out_5900386540209642427[10] = 0;
   out_5900386540209642427[11] = 0;
   out_5900386540209642427[12] = 1;
   out_5900386540209642427[13] = 0;
   out_5900386540209642427[14] = 0;
   out_5900386540209642427[15] = 1;
   out_5900386540209642427[16] = 0;
   out_5900386540209642427[17] = 0;
   out_5900386540209642427[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5900386540209642427[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5900386540209642427[20] = 0;
   out_5900386540209642427[21] = state[8];
   out_5900386540209642427[22] = 0;
   out_5900386540209642427[23] = -state[6];
   out_5900386540209642427[24] = -state[5];
   out_5900386540209642427[25] = 0;
   out_5900386540209642427[26] = state[3];
   out_5900386540209642427[27] = 0;
   out_5900386540209642427[28] = 0;
   out_5900386540209642427[29] = 0;
   out_5900386540209642427[30] = 0;
   out_5900386540209642427[31] = 1;
   out_5900386540209642427[32] = 0;
   out_5900386540209642427[33] = 0;
   out_5900386540209642427[34] = 1;
   out_5900386540209642427[35] = 0;
   out_5900386540209642427[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5900386540209642427[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5900386540209642427[38] = 0;
   out_5900386540209642427[39] = -state[7];
   out_5900386540209642427[40] = state[6];
   out_5900386540209642427[41] = 0;
   out_5900386540209642427[42] = state[4];
   out_5900386540209642427[43] = -state[3];
   out_5900386540209642427[44] = 0;
   out_5900386540209642427[45] = 0;
   out_5900386540209642427[46] = 0;
   out_5900386540209642427[47] = 0;
   out_5900386540209642427[48] = 0;
   out_5900386540209642427[49] = 0;
   out_5900386540209642427[50] = 1;
   out_5900386540209642427[51] = 0;
   out_5900386540209642427[52] = 0;
   out_5900386540209642427[53] = 1;
}
void h_13(double *state, double *unused, double *out_5542331391789455258) {
   out_5542331391789455258[0] = state[3];
   out_5542331391789455258[1] = state[4];
   out_5542331391789455258[2] = state[5];
}
void H_13(double *state, double *unused, double *out_8715613701855447331) {
   out_8715613701855447331[0] = 0;
   out_8715613701855447331[1] = 0;
   out_8715613701855447331[2] = 0;
   out_8715613701855447331[3] = 1;
   out_8715613701855447331[4] = 0;
   out_8715613701855447331[5] = 0;
   out_8715613701855447331[6] = 0;
   out_8715613701855447331[7] = 0;
   out_8715613701855447331[8] = 0;
   out_8715613701855447331[9] = 0;
   out_8715613701855447331[10] = 0;
   out_8715613701855447331[11] = 0;
   out_8715613701855447331[12] = 0;
   out_8715613701855447331[13] = 0;
   out_8715613701855447331[14] = 0;
   out_8715613701855447331[15] = 0;
   out_8715613701855447331[16] = 0;
   out_8715613701855447331[17] = 0;
   out_8715613701855447331[18] = 0;
   out_8715613701855447331[19] = 0;
   out_8715613701855447331[20] = 0;
   out_8715613701855447331[21] = 0;
   out_8715613701855447331[22] = 1;
   out_8715613701855447331[23] = 0;
   out_8715613701855447331[24] = 0;
   out_8715613701855447331[25] = 0;
   out_8715613701855447331[26] = 0;
   out_8715613701855447331[27] = 0;
   out_8715613701855447331[28] = 0;
   out_8715613701855447331[29] = 0;
   out_8715613701855447331[30] = 0;
   out_8715613701855447331[31] = 0;
   out_8715613701855447331[32] = 0;
   out_8715613701855447331[33] = 0;
   out_8715613701855447331[34] = 0;
   out_8715613701855447331[35] = 0;
   out_8715613701855447331[36] = 0;
   out_8715613701855447331[37] = 0;
   out_8715613701855447331[38] = 0;
   out_8715613701855447331[39] = 0;
   out_8715613701855447331[40] = 0;
   out_8715613701855447331[41] = 1;
   out_8715613701855447331[42] = 0;
   out_8715613701855447331[43] = 0;
   out_8715613701855447331[44] = 0;
   out_8715613701855447331[45] = 0;
   out_8715613701855447331[46] = 0;
   out_8715613701855447331[47] = 0;
   out_8715613701855447331[48] = 0;
   out_8715613701855447331[49] = 0;
   out_8715613701855447331[50] = 0;
   out_8715613701855447331[51] = 0;
   out_8715613701855447331[52] = 0;
   out_8715613701855447331[53] = 0;
}
void h_14(double *state, double *unused, double *out_1432390697257379482) {
   out_1432390697257379482[0] = state[6];
   out_1432390697257379482[1] = state[7];
   out_1432390697257379482[2] = state[8];
}
void H_14(double *state, double *unused, double *out_7964646670848295603) {
   out_7964646670848295603[0] = 0;
   out_7964646670848295603[1] = 0;
   out_7964646670848295603[2] = 0;
   out_7964646670848295603[3] = 0;
   out_7964646670848295603[4] = 0;
   out_7964646670848295603[5] = 0;
   out_7964646670848295603[6] = 1;
   out_7964646670848295603[7] = 0;
   out_7964646670848295603[8] = 0;
   out_7964646670848295603[9] = 0;
   out_7964646670848295603[10] = 0;
   out_7964646670848295603[11] = 0;
   out_7964646670848295603[12] = 0;
   out_7964646670848295603[13] = 0;
   out_7964646670848295603[14] = 0;
   out_7964646670848295603[15] = 0;
   out_7964646670848295603[16] = 0;
   out_7964646670848295603[17] = 0;
   out_7964646670848295603[18] = 0;
   out_7964646670848295603[19] = 0;
   out_7964646670848295603[20] = 0;
   out_7964646670848295603[21] = 0;
   out_7964646670848295603[22] = 0;
   out_7964646670848295603[23] = 0;
   out_7964646670848295603[24] = 0;
   out_7964646670848295603[25] = 1;
   out_7964646670848295603[26] = 0;
   out_7964646670848295603[27] = 0;
   out_7964646670848295603[28] = 0;
   out_7964646670848295603[29] = 0;
   out_7964646670848295603[30] = 0;
   out_7964646670848295603[31] = 0;
   out_7964646670848295603[32] = 0;
   out_7964646670848295603[33] = 0;
   out_7964646670848295603[34] = 0;
   out_7964646670848295603[35] = 0;
   out_7964646670848295603[36] = 0;
   out_7964646670848295603[37] = 0;
   out_7964646670848295603[38] = 0;
   out_7964646670848295603[39] = 0;
   out_7964646670848295603[40] = 0;
   out_7964646670848295603[41] = 0;
   out_7964646670848295603[42] = 0;
   out_7964646670848295603[43] = 0;
   out_7964646670848295603[44] = 1;
   out_7964646670848295603[45] = 0;
   out_7964646670848295603[46] = 0;
   out_7964646670848295603[47] = 0;
   out_7964646670848295603[48] = 0;
   out_7964646670848295603[49] = 0;
   out_7964646670848295603[50] = 0;
   out_7964646670848295603[51] = 0;
   out_7964646670848295603[52] = 0;
   out_7964646670848295603[53] = 0;
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

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_5580352609619235680) {
  err_fun(nom_x, delta_x, out_5580352609619235680);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_977978120678855025) {
  inv_err_fun(nom_x, true_x, out_977978120678855025);
}
void pose_H_mod_fun(double *state, double *out_6079805720435821411) {
  H_mod_fun(state, out_6079805720435821411);
}
void pose_f_fun(double *state, double dt, double *out_600860468097978335) {
  f_fun(state,  dt, out_600860468097978335);
}
void pose_F_fun(double *state, double dt, double *out_1300580827509148386) {
  F_fun(state,  dt, out_1300580827509148386);
}
void pose_h_4(double *state, double *unused, double *out_1401553991619916962) {
  h_4(state, unused, out_1401553991619916962);
}
void pose_H_4(double *state, double *unused, double *out_6518856546521771484) {
  H_4(state, unused, out_6518856546521771484);
}
void pose_h_10(double *state, double *unused, double *out_2150734223135501944) {
  h_10(state, unused, out_2150734223135501944);
}
void pose_H_10(double *state, double *unused, double *out_5900386540209642427) {
  H_10(state, unused, out_5900386540209642427);
}
void pose_h_13(double *state, double *unused, double *out_5542331391789455258) {
  h_13(state, unused, out_5542331391789455258);
}
void pose_H_13(double *state, double *unused, double *out_8715613701855447331) {
  H_13(state, unused, out_8715613701855447331);
}
void pose_h_14(double *state, double *unused, double *out_1432390697257379482) {
  h_14(state, unused, out_1432390697257379482);
}
void pose_H_14(double *state, double *unused, double *out_7964646670848295603) {
  H_14(state, unused, out_7964646670848295603);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
