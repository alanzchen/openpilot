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
void err_fun(double *nom_x, double *delta_x, double *out_1637031099889953199) {
   out_1637031099889953199[0] = delta_x[0] + nom_x[0];
   out_1637031099889953199[1] = delta_x[1] + nom_x[1];
   out_1637031099889953199[2] = delta_x[2] + nom_x[2];
   out_1637031099889953199[3] = delta_x[3] + nom_x[3];
   out_1637031099889953199[4] = delta_x[4] + nom_x[4];
   out_1637031099889953199[5] = delta_x[5] + nom_x[5];
   out_1637031099889953199[6] = delta_x[6] + nom_x[6];
   out_1637031099889953199[7] = delta_x[7] + nom_x[7];
   out_1637031099889953199[8] = delta_x[8] + nom_x[8];
   out_1637031099889953199[9] = delta_x[9] + nom_x[9];
   out_1637031099889953199[10] = delta_x[10] + nom_x[10];
   out_1637031099889953199[11] = delta_x[11] + nom_x[11];
   out_1637031099889953199[12] = delta_x[12] + nom_x[12];
   out_1637031099889953199[13] = delta_x[13] + nom_x[13];
   out_1637031099889953199[14] = delta_x[14] + nom_x[14];
   out_1637031099889953199[15] = delta_x[15] + nom_x[15];
   out_1637031099889953199[16] = delta_x[16] + nom_x[16];
   out_1637031099889953199[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3175856492030467000) {
   out_3175856492030467000[0] = -nom_x[0] + true_x[0];
   out_3175856492030467000[1] = -nom_x[1] + true_x[1];
   out_3175856492030467000[2] = -nom_x[2] + true_x[2];
   out_3175856492030467000[3] = -nom_x[3] + true_x[3];
   out_3175856492030467000[4] = -nom_x[4] + true_x[4];
   out_3175856492030467000[5] = -nom_x[5] + true_x[5];
   out_3175856492030467000[6] = -nom_x[6] + true_x[6];
   out_3175856492030467000[7] = -nom_x[7] + true_x[7];
   out_3175856492030467000[8] = -nom_x[8] + true_x[8];
   out_3175856492030467000[9] = -nom_x[9] + true_x[9];
   out_3175856492030467000[10] = -nom_x[10] + true_x[10];
   out_3175856492030467000[11] = -nom_x[11] + true_x[11];
   out_3175856492030467000[12] = -nom_x[12] + true_x[12];
   out_3175856492030467000[13] = -nom_x[13] + true_x[13];
   out_3175856492030467000[14] = -nom_x[14] + true_x[14];
   out_3175856492030467000[15] = -nom_x[15] + true_x[15];
   out_3175856492030467000[16] = -nom_x[16] + true_x[16];
   out_3175856492030467000[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_3254015575193517177) {
   out_3254015575193517177[0] = 1.0;
   out_3254015575193517177[1] = 0.0;
   out_3254015575193517177[2] = 0.0;
   out_3254015575193517177[3] = 0.0;
   out_3254015575193517177[4] = 0.0;
   out_3254015575193517177[5] = 0.0;
   out_3254015575193517177[6] = 0.0;
   out_3254015575193517177[7] = 0.0;
   out_3254015575193517177[8] = 0.0;
   out_3254015575193517177[9] = 0.0;
   out_3254015575193517177[10] = 0.0;
   out_3254015575193517177[11] = 0.0;
   out_3254015575193517177[12] = 0.0;
   out_3254015575193517177[13] = 0.0;
   out_3254015575193517177[14] = 0.0;
   out_3254015575193517177[15] = 0.0;
   out_3254015575193517177[16] = 0.0;
   out_3254015575193517177[17] = 0.0;
   out_3254015575193517177[18] = 0.0;
   out_3254015575193517177[19] = 1.0;
   out_3254015575193517177[20] = 0.0;
   out_3254015575193517177[21] = 0.0;
   out_3254015575193517177[22] = 0.0;
   out_3254015575193517177[23] = 0.0;
   out_3254015575193517177[24] = 0.0;
   out_3254015575193517177[25] = 0.0;
   out_3254015575193517177[26] = 0.0;
   out_3254015575193517177[27] = 0.0;
   out_3254015575193517177[28] = 0.0;
   out_3254015575193517177[29] = 0.0;
   out_3254015575193517177[30] = 0.0;
   out_3254015575193517177[31] = 0.0;
   out_3254015575193517177[32] = 0.0;
   out_3254015575193517177[33] = 0.0;
   out_3254015575193517177[34] = 0.0;
   out_3254015575193517177[35] = 0.0;
   out_3254015575193517177[36] = 0.0;
   out_3254015575193517177[37] = 0.0;
   out_3254015575193517177[38] = 1.0;
   out_3254015575193517177[39] = 0.0;
   out_3254015575193517177[40] = 0.0;
   out_3254015575193517177[41] = 0.0;
   out_3254015575193517177[42] = 0.0;
   out_3254015575193517177[43] = 0.0;
   out_3254015575193517177[44] = 0.0;
   out_3254015575193517177[45] = 0.0;
   out_3254015575193517177[46] = 0.0;
   out_3254015575193517177[47] = 0.0;
   out_3254015575193517177[48] = 0.0;
   out_3254015575193517177[49] = 0.0;
   out_3254015575193517177[50] = 0.0;
   out_3254015575193517177[51] = 0.0;
   out_3254015575193517177[52] = 0.0;
   out_3254015575193517177[53] = 0.0;
   out_3254015575193517177[54] = 0.0;
   out_3254015575193517177[55] = 0.0;
   out_3254015575193517177[56] = 0.0;
   out_3254015575193517177[57] = 1.0;
   out_3254015575193517177[58] = 0.0;
   out_3254015575193517177[59] = 0.0;
   out_3254015575193517177[60] = 0.0;
   out_3254015575193517177[61] = 0.0;
   out_3254015575193517177[62] = 0.0;
   out_3254015575193517177[63] = 0.0;
   out_3254015575193517177[64] = 0.0;
   out_3254015575193517177[65] = 0.0;
   out_3254015575193517177[66] = 0.0;
   out_3254015575193517177[67] = 0.0;
   out_3254015575193517177[68] = 0.0;
   out_3254015575193517177[69] = 0.0;
   out_3254015575193517177[70] = 0.0;
   out_3254015575193517177[71] = 0.0;
   out_3254015575193517177[72] = 0.0;
   out_3254015575193517177[73] = 0.0;
   out_3254015575193517177[74] = 0.0;
   out_3254015575193517177[75] = 0.0;
   out_3254015575193517177[76] = 1.0;
   out_3254015575193517177[77] = 0.0;
   out_3254015575193517177[78] = 0.0;
   out_3254015575193517177[79] = 0.0;
   out_3254015575193517177[80] = 0.0;
   out_3254015575193517177[81] = 0.0;
   out_3254015575193517177[82] = 0.0;
   out_3254015575193517177[83] = 0.0;
   out_3254015575193517177[84] = 0.0;
   out_3254015575193517177[85] = 0.0;
   out_3254015575193517177[86] = 0.0;
   out_3254015575193517177[87] = 0.0;
   out_3254015575193517177[88] = 0.0;
   out_3254015575193517177[89] = 0.0;
   out_3254015575193517177[90] = 0.0;
   out_3254015575193517177[91] = 0.0;
   out_3254015575193517177[92] = 0.0;
   out_3254015575193517177[93] = 0.0;
   out_3254015575193517177[94] = 0.0;
   out_3254015575193517177[95] = 1.0;
   out_3254015575193517177[96] = 0.0;
   out_3254015575193517177[97] = 0.0;
   out_3254015575193517177[98] = 0.0;
   out_3254015575193517177[99] = 0.0;
   out_3254015575193517177[100] = 0.0;
   out_3254015575193517177[101] = 0.0;
   out_3254015575193517177[102] = 0.0;
   out_3254015575193517177[103] = 0.0;
   out_3254015575193517177[104] = 0.0;
   out_3254015575193517177[105] = 0.0;
   out_3254015575193517177[106] = 0.0;
   out_3254015575193517177[107] = 0.0;
   out_3254015575193517177[108] = 0.0;
   out_3254015575193517177[109] = 0.0;
   out_3254015575193517177[110] = 0.0;
   out_3254015575193517177[111] = 0.0;
   out_3254015575193517177[112] = 0.0;
   out_3254015575193517177[113] = 0.0;
   out_3254015575193517177[114] = 1.0;
   out_3254015575193517177[115] = 0.0;
   out_3254015575193517177[116] = 0.0;
   out_3254015575193517177[117] = 0.0;
   out_3254015575193517177[118] = 0.0;
   out_3254015575193517177[119] = 0.0;
   out_3254015575193517177[120] = 0.0;
   out_3254015575193517177[121] = 0.0;
   out_3254015575193517177[122] = 0.0;
   out_3254015575193517177[123] = 0.0;
   out_3254015575193517177[124] = 0.0;
   out_3254015575193517177[125] = 0.0;
   out_3254015575193517177[126] = 0.0;
   out_3254015575193517177[127] = 0.0;
   out_3254015575193517177[128] = 0.0;
   out_3254015575193517177[129] = 0.0;
   out_3254015575193517177[130] = 0.0;
   out_3254015575193517177[131] = 0.0;
   out_3254015575193517177[132] = 0.0;
   out_3254015575193517177[133] = 1.0;
   out_3254015575193517177[134] = 0.0;
   out_3254015575193517177[135] = 0.0;
   out_3254015575193517177[136] = 0.0;
   out_3254015575193517177[137] = 0.0;
   out_3254015575193517177[138] = 0.0;
   out_3254015575193517177[139] = 0.0;
   out_3254015575193517177[140] = 0.0;
   out_3254015575193517177[141] = 0.0;
   out_3254015575193517177[142] = 0.0;
   out_3254015575193517177[143] = 0.0;
   out_3254015575193517177[144] = 0.0;
   out_3254015575193517177[145] = 0.0;
   out_3254015575193517177[146] = 0.0;
   out_3254015575193517177[147] = 0.0;
   out_3254015575193517177[148] = 0.0;
   out_3254015575193517177[149] = 0.0;
   out_3254015575193517177[150] = 0.0;
   out_3254015575193517177[151] = 0.0;
   out_3254015575193517177[152] = 1.0;
   out_3254015575193517177[153] = 0.0;
   out_3254015575193517177[154] = 0.0;
   out_3254015575193517177[155] = 0.0;
   out_3254015575193517177[156] = 0.0;
   out_3254015575193517177[157] = 0.0;
   out_3254015575193517177[158] = 0.0;
   out_3254015575193517177[159] = 0.0;
   out_3254015575193517177[160] = 0.0;
   out_3254015575193517177[161] = 0.0;
   out_3254015575193517177[162] = 0.0;
   out_3254015575193517177[163] = 0.0;
   out_3254015575193517177[164] = 0.0;
   out_3254015575193517177[165] = 0.0;
   out_3254015575193517177[166] = 0.0;
   out_3254015575193517177[167] = 0.0;
   out_3254015575193517177[168] = 0.0;
   out_3254015575193517177[169] = 0.0;
   out_3254015575193517177[170] = 0.0;
   out_3254015575193517177[171] = 1.0;
   out_3254015575193517177[172] = 0.0;
   out_3254015575193517177[173] = 0.0;
   out_3254015575193517177[174] = 0.0;
   out_3254015575193517177[175] = 0.0;
   out_3254015575193517177[176] = 0.0;
   out_3254015575193517177[177] = 0.0;
   out_3254015575193517177[178] = 0.0;
   out_3254015575193517177[179] = 0.0;
   out_3254015575193517177[180] = 0.0;
   out_3254015575193517177[181] = 0.0;
   out_3254015575193517177[182] = 0.0;
   out_3254015575193517177[183] = 0.0;
   out_3254015575193517177[184] = 0.0;
   out_3254015575193517177[185] = 0.0;
   out_3254015575193517177[186] = 0.0;
   out_3254015575193517177[187] = 0.0;
   out_3254015575193517177[188] = 0.0;
   out_3254015575193517177[189] = 0.0;
   out_3254015575193517177[190] = 1.0;
   out_3254015575193517177[191] = 0.0;
   out_3254015575193517177[192] = 0.0;
   out_3254015575193517177[193] = 0.0;
   out_3254015575193517177[194] = 0.0;
   out_3254015575193517177[195] = 0.0;
   out_3254015575193517177[196] = 0.0;
   out_3254015575193517177[197] = 0.0;
   out_3254015575193517177[198] = 0.0;
   out_3254015575193517177[199] = 0.0;
   out_3254015575193517177[200] = 0.0;
   out_3254015575193517177[201] = 0.0;
   out_3254015575193517177[202] = 0.0;
   out_3254015575193517177[203] = 0.0;
   out_3254015575193517177[204] = 0.0;
   out_3254015575193517177[205] = 0.0;
   out_3254015575193517177[206] = 0.0;
   out_3254015575193517177[207] = 0.0;
   out_3254015575193517177[208] = 0.0;
   out_3254015575193517177[209] = 1.0;
   out_3254015575193517177[210] = 0.0;
   out_3254015575193517177[211] = 0.0;
   out_3254015575193517177[212] = 0.0;
   out_3254015575193517177[213] = 0.0;
   out_3254015575193517177[214] = 0.0;
   out_3254015575193517177[215] = 0.0;
   out_3254015575193517177[216] = 0.0;
   out_3254015575193517177[217] = 0.0;
   out_3254015575193517177[218] = 0.0;
   out_3254015575193517177[219] = 0.0;
   out_3254015575193517177[220] = 0.0;
   out_3254015575193517177[221] = 0.0;
   out_3254015575193517177[222] = 0.0;
   out_3254015575193517177[223] = 0.0;
   out_3254015575193517177[224] = 0.0;
   out_3254015575193517177[225] = 0.0;
   out_3254015575193517177[226] = 0.0;
   out_3254015575193517177[227] = 0.0;
   out_3254015575193517177[228] = 1.0;
   out_3254015575193517177[229] = 0.0;
   out_3254015575193517177[230] = 0.0;
   out_3254015575193517177[231] = 0.0;
   out_3254015575193517177[232] = 0.0;
   out_3254015575193517177[233] = 0.0;
   out_3254015575193517177[234] = 0.0;
   out_3254015575193517177[235] = 0.0;
   out_3254015575193517177[236] = 0.0;
   out_3254015575193517177[237] = 0.0;
   out_3254015575193517177[238] = 0.0;
   out_3254015575193517177[239] = 0.0;
   out_3254015575193517177[240] = 0.0;
   out_3254015575193517177[241] = 0.0;
   out_3254015575193517177[242] = 0.0;
   out_3254015575193517177[243] = 0.0;
   out_3254015575193517177[244] = 0.0;
   out_3254015575193517177[245] = 0.0;
   out_3254015575193517177[246] = 0.0;
   out_3254015575193517177[247] = 1.0;
   out_3254015575193517177[248] = 0.0;
   out_3254015575193517177[249] = 0.0;
   out_3254015575193517177[250] = 0.0;
   out_3254015575193517177[251] = 0.0;
   out_3254015575193517177[252] = 0.0;
   out_3254015575193517177[253] = 0.0;
   out_3254015575193517177[254] = 0.0;
   out_3254015575193517177[255] = 0.0;
   out_3254015575193517177[256] = 0.0;
   out_3254015575193517177[257] = 0.0;
   out_3254015575193517177[258] = 0.0;
   out_3254015575193517177[259] = 0.0;
   out_3254015575193517177[260] = 0.0;
   out_3254015575193517177[261] = 0.0;
   out_3254015575193517177[262] = 0.0;
   out_3254015575193517177[263] = 0.0;
   out_3254015575193517177[264] = 0.0;
   out_3254015575193517177[265] = 0.0;
   out_3254015575193517177[266] = 1.0;
   out_3254015575193517177[267] = 0.0;
   out_3254015575193517177[268] = 0.0;
   out_3254015575193517177[269] = 0.0;
   out_3254015575193517177[270] = 0.0;
   out_3254015575193517177[271] = 0.0;
   out_3254015575193517177[272] = 0.0;
   out_3254015575193517177[273] = 0.0;
   out_3254015575193517177[274] = 0.0;
   out_3254015575193517177[275] = 0.0;
   out_3254015575193517177[276] = 0.0;
   out_3254015575193517177[277] = 0.0;
   out_3254015575193517177[278] = 0.0;
   out_3254015575193517177[279] = 0.0;
   out_3254015575193517177[280] = 0.0;
   out_3254015575193517177[281] = 0.0;
   out_3254015575193517177[282] = 0.0;
   out_3254015575193517177[283] = 0.0;
   out_3254015575193517177[284] = 0.0;
   out_3254015575193517177[285] = 1.0;
   out_3254015575193517177[286] = 0.0;
   out_3254015575193517177[287] = 0.0;
   out_3254015575193517177[288] = 0.0;
   out_3254015575193517177[289] = 0.0;
   out_3254015575193517177[290] = 0.0;
   out_3254015575193517177[291] = 0.0;
   out_3254015575193517177[292] = 0.0;
   out_3254015575193517177[293] = 0.0;
   out_3254015575193517177[294] = 0.0;
   out_3254015575193517177[295] = 0.0;
   out_3254015575193517177[296] = 0.0;
   out_3254015575193517177[297] = 0.0;
   out_3254015575193517177[298] = 0.0;
   out_3254015575193517177[299] = 0.0;
   out_3254015575193517177[300] = 0.0;
   out_3254015575193517177[301] = 0.0;
   out_3254015575193517177[302] = 0.0;
   out_3254015575193517177[303] = 0.0;
   out_3254015575193517177[304] = 1.0;
   out_3254015575193517177[305] = 0.0;
   out_3254015575193517177[306] = 0.0;
   out_3254015575193517177[307] = 0.0;
   out_3254015575193517177[308] = 0.0;
   out_3254015575193517177[309] = 0.0;
   out_3254015575193517177[310] = 0.0;
   out_3254015575193517177[311] = 0.0;
   out_3254015575193517177[312] = 0.0;
   out_3254015575193517177[313] = 0.0;
   out_3254015575193517177[314] = 0.0;
   out_3254015575193517177[315] = 0.0;
   out_3254015575193517177[316] = 0.0;
   out_3254015575193517177[317] = 0.0;
   out_3254015575193517177[318] = 0.0;
   out_3254015575193517177[319] = 0.0;
   out_3254015575193517177[320] = 0.0;
   out_3254015575193517177[321] = 0.0;
   out_3254015575193517177[322] = 0.0;
   out_3254015575193517177[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5452727561293164163) {
   out_5452727561293164163[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5452727561293164163[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5452727561293164163[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5452727561293164163[3] = dt*state[12] + state[3];
   out_5452727561293164163[4] = dt*state[13] + state[4];
   out_5452727561293164163[5] = dt*state[14] + state[5];
   out_5452727561293164163[6] = state[6];
   out_5452727561293164163[7] = state[7];
   out_5452727561293164163[8] = state[8];
   out_5452727561293164163[9] = state[9];
   out_5452727561293164163[10] = state[10];
   out_5452727561293164163[11] = state[11];
   out_5452727561293164163[12] = state[12];
   out_5452727561293164163[13] = state[13];
   out_5452727561293164163[14] = state[14];
   out_5452727561293164163[15] = state[15];
   out_5452727561293164163[16] = state[16];
   out_5452727561293164163[17] = state[17];
}
void F_fun(double *state, double dt, double *out_1596973880245959678) {
   out_1596973880245959678[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1596973880245959678[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1596973880245959678[2] = 0;
   out_1596973880245959678[3] = 0;
   out_1596973880245959678[4] = 0;
   out_1596973880245959678[5] = 0;
   out_1596973880245959678[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1596973880245959678[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1596973880245959678[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_1596973880245959678[9] = 0;
   out_1596973880245959678[10] = 0;
   out_1596973880245959678[11] = 0;
   out_1596973880245959678[12] = 0;
   out_1596973880245959678[13] = 0;
   out_1596973880245959678[14] = 0;
   out_1596973880245959678[15] = 0;
   out_1596973880245959678[16] = 0;
   out_1596973880245959678[17] = 0;
   out_1596973880245959678[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1596973880245959678[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1596973880245959678[20] = 0;
   out_1596973880245959678[21] = 0;
   out_1596973880245959678[22] = 0;
   out_1596973880245959678[23] = 0;
   out_1596973880245959678[24] = 0;
   out_1596973880245959678[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1596973880245959678[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_1596973880245959678[27] = 0;
   out_1596973880245959678[28] = 0;
   out_1596973880245959678[29] = 0;
   out_1596973880245959678[30] = 0;
   out_1596973880245959678[31] = 0;
   out_1596973880245959678[32] = 0;
   out_1596973880245959678[33] = 0;
   out_1596973880245959678[34] = 0;
   out_1596973880245959678[35] = 0;
   out_1596973880245959678[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1596973880245959678[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1596973880245959678[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1596973880245959678[39] = 0;
   out_1596973880245959678[40] = 0;
   out_1596973880245959678[41] = 0;
   out_1596973880245959678[42] = 0;
   out_1596973880245959678[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1596973880245959678[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_1596973880245959678[45] = 0;
   out_1596973880245959678[46] = 0;
   out_1596973880245959678[47] = 0;
   out_1596973880245959678[48] = 0;
   out_1596973880245959678[49] = 0;
   out_1596973880245959678[50] = 0;
   out_1596973880245959678[51] = 0;
   out_1596973880245959678[52] = 0;
   out_1596973880245959678[53] = 0;
   out_1596973880245959678[54] = 0;
   out_1596973880245959678[55] = 0;
   out_1596973880245959678[56] = 0;
   out_1596973880245959678[57] = 1;
   out_1596973880245959678[58] = 0;
   out_1596973880245959678[59] = 0;
   out_1596973880245959678[60] = 0;
   out_1596973880245959678[61] = 0;
   out_1596973880245959678[62] = 0;
   out_1596973880245959678[63] = 0;
   out_1596973880245959678[64] = 0;
   out_1596973880245959678[65] = 0;
   out_1596973880245959678[66] = dt;
   out_1596973880245959678[67] = 0;
   out_1596973880245959678[68] = 0;
   out_1596973880245959678[69] = 0;
   out_1596973880245959678[70] = 0;
   out_1596973880245959678[71] = 0;
   out_1596973880245959678[72] = 0;
   out_1596973880245959678[73] = 0;
   out_1596973880245959678[74] = 0;
   out_1596973880245959678[75] = 0;
   out_1596973880245959678[76] = 1;
   out_1596973880245959678[77] = 0;
   out_1596973880245959678[78] = 0;
   out_1596973880245959678[79] = 0;
   out_1596973880245959678[80] = 0;
   out_1596973880245959678[81] = 0;
   out_1596973880245959678[82] = 0;
   out_1596973880245959678[83] = 0;
   out_1596973880245959678[84] = 0;
   out_1596973880245959678[85] = dt;
   out_1596973880245959678[86] = 0;
   out_1596973880245959678[87] = 0;
   out_1596973880245959678[88] = 0;
   out_1596973880245959678[89] = 0;
   out_1596973880245959678[90] = 0;
   out_1596973880245959678[91] = 0;
   out_1596973880245959678[92] = 0;
   out_1596973880245959678[93] = 0;
   out_1596973880245959678[94] = 0;
   out_1596973880245959678[95] = 1;
   out_1596973880245959678[96] = 0;
   out_1596973880245959678[97] = 0;
   out_1596973880245959678[98] = 0;
   out_1596973880245959678[99] = 0;
   out_1596973880245959678[100] = 0;
   out_1596973880245959678[101] = 0;
   out_1596973880245959678[102] = 0;
   out_1596973880245959678[103] = 0;
   out_1596973880245959678[104] = dt;
   out_1596973880245959678[105] = 0;
   out_1596973880245959678[106] = 0;
   out_1596973880245959678[107] = 0;
   out_1596973880245959678[108] = 0;
   out_1596973880245959678[109] = 0;
   out_1596973880245959678[110] = 0;
   out_1596973880245959678[111] = 0;
   out_1596973880245959678[112] = 0;
   out_1596973880245959678[113] = 0;
   out_1596973880245959678[114] = 1;
   out_1596973880245959678[115] = 0;
   out_1596973880245959678[116] = 0;
   out_1596973880245959678[117] = 0;
   out_1596973880245959678[118] = 0;
   out_1596973880245959678[119] = 0;
   out_1596973880245959678[120] = 0;
   out_1596973880245959678[121] = 0;
   out_1596973880245959678[122] = 0;
   out_1596973880245959678[123] = 0;
   out_1596973880245959678[124] = 0;
   out_1596973880245959678[125] = 0;
   out_1596973880245959678[126] = 0;
   out_1596973880245959678[127] = 0;
   out_1596973880245959678[128] = 0;
   out_1596973880245959678[129] = 0;
   out_1596973880245959678[130] = 0;
   out_1596973880245959678[131] = 0;
   out_1596973880245959678[132] = 0;
   out_1596973880245959678[133] = 1;
   out_1596973880245959678[134] = 0;
   out_1596973880245959678[135] = 0;
   out_1596973880245959678[136] = 0;
   out_1596973880245959678[137] = 0;
   out_1596973880245959678[138] = 0;
   out_1596973880245959678[139] = 0;
   out_1596973880245959678[140] = 0;
   out_1596973880245959678[141] = 0;
   out_1596973880245959678[142] = 0;
   out_1596973880245959678[143] = 0;
   out_1596973880245959678[144] = 0;
   out_1596973880245959678[145] = 0;
   out_1596973880245959678[146] = 0;
   out_1596973880245959678[147] = 0;
   out_1596973880245959678[148] = 0;
   out_1596973880245959678[149] = 0;
   out_1596973880245959678[150] = 0;
   out_1596973880245959678[151] = 0;
   out_1596973880245959678[152] = 1;
   out_1596973880245959678[153] = 0;
   out_1596973880245959678[154] = 0;
   out_1596973880245959678[155] = 0;
   out_1596973880245959678[156] = 0;
   out_1596973880245959678[157] = 0;
   out_1596973880245959678[158] = 0;
   out_1596973880245959678[159] = 0;
   out_1596973880245959678[160] = 0;
   out_1596973880245959678[161] = 0;
   out_1596973880245959678[162] = 0;
   out_1596973880245959678[163] = 0;
   out_1596973880245959678[164] = 0;
   out_1596973880245959678[165] = 0;
   out_1596973880245959678[166] = 0;
   out_1596973880245959678[167] = 0;
   out_1596973880245959678[168] = 0;
   out_1596973880245959678[169] = 0;
   out_1596973880245959678[170] = 0;
   out_1596973880245959678[171] = 1;
   out_1596973880245959678[172] = 0;
   out_1596973880245959678[173] = 0;
   out_1596973880245959678[174] = 0;
   out_1596973880245959678[175] = 0;
   out_1596973880245959678[176] = 0;
   out_1596973880245959678[177] = 0;
   out_1596973880245959678[178] = 0;
   out_1596973880245959678[179] = 0;
   out_1596973880245959678[180] = 0;
   out_1596973880245959678[181] = 0;
   out_1596973880245959678[182] = 0;
   out_1596973880245959678[183] = 0;
   out_1596973880245959678[184] = 0;
   out_1596973880245959678[185] = 0;
   out_1596973880245959678[186] = 0;
   out_1596973880245959678[187] = 0;
   out_1596973880245959678[188] = 0;
   out_1596973880245959678[189] = 0;
   out_1596973880245959678[190] = 1;
   out_1596973880245959678[191] = 0;
   out_1596973880245959678[192] = 0;
   out_1596973880245959678[193] = 0;
   out_1596973880245959678[194] = 0;
   out_1596973880245959678[195] = 0;
   out_1596973880245959678[196] = 0;
   out_1596973880245959678[197] = 0;
   out_1596973880245959678[198] = 0;
   out_1596973880245959678[199] = 0;
   out_1596973880245959678[200] = 0;
   out_1596973880245959678[201] = 0;
   out_1596973880245959678[202] = 0;
   out_1596973880245959678[203] = 0;
   out_1596973880245959678[204] = 0;
   out_1596973880245959678[205] = 0;
   out_1596973880245959678[206] = 0;
   out_1596973880245959678[207] = 0;
   out_1596973880245959678[208] = 0;
   out_1596973880245959678[209] = 1;
   out_1596973880245959678[210] = 0;
   out_1596973880245959678[211] = 0;
   out_1596973880245959678[212] = 0;
   out_1596973880245959678[213] = 0;
   out_1596973880245959678[214] = 0;
   out_1596973880245959678[215] = 0;
   out_1596973880245959678[216] = 0;
   out_1596973880245959678[217] = 0;
   out_1596973880245959678[218] = 0;
   out_1596973880245959678[219] = 0;
   out_1596973880245959678[220] = 0;
   out_1596973880245959678[221] = 0;
   out_1596973880245959678[222] = 0;
   out_1596973880245959678[223] = 0;
   out_1596973880245959678[224] = 0;
   out_1596973880245959678[225] = 0;
   out_1596973880245959678[226] = 0;
   out_1596973880245959678[227] = 0;
   out_1596973880245959678[228] = 1;
   out_1596973880245959678[229] = 0;
   out_1596973880245959678[230] = 0;
   out_1596973880245959678[231] = 0;
   out_1596973880245959678[232] = 0;
   out_1596973880245959678[233] = 0;
   out_1596973880245959678[234] = 0;
   out_1596973880245959678[235] = 0;
   out_1596973880245959678[236] = 0;
   out_1596973880245959678[237] = 0;
   out_1596973880245959678[238] = 0;
   out_1596973880245959678[239] = 0;
   out_1596973880245959678[240] = 0;
   out_1596973880245959678[241] = 0;
   out_1596973880245959678[242] = 0;
   out_1596973880245959678[243] = 0;
   out_1596973880245959678[244] = 0;
   out_1596973880245959678[245] = 0;
   out_1596973880245959678[246] = 0;
   out_1596973880245959678[247] = 1;
   out_1596973880245959678[248] = 0;
   out_1596973880245959678[249] = 0;
   out_1596973880245959678[250] = 0;
   out_1596973880245959678[251] = 0;
   out_1596973880245959678[252] = 0;
   out_1596973880245959678[253] = 0;
   out_1596973880245959678[254] = 0;
   out_1596973880245959678[255] = 0;
   out_1596973880245959678[256] = 0;
   out_1596973880245959678[257] = 0;
   out_1596973880245959678[258] = 0;
   out_1596973880245959678[259] = 0;
   out_1596973880245959678[260] = 0;
   out_1596973880245959678[261] = 0;
   out_1596973880245959678[262] = 0;
   out_1596973880245959678[263] = 0;
   out_1596973880245959678[264] = 0;
   out_1596973880245959678[265] = 0;
   out_1596973880245959678[266] = 1;
   out_1596973880245959678[267] = 0;
   out_1596973880245959678[268] = 0;
   out_1596973880245959678[269] = 0;
   out_1596973880245959678[270] = 0;
   out_1596973880245959678[271] = 0;
   out_1596973880245959678[272] = 0;
   out_1596973880245959678[273] = 0;
   out_1596973880245959678[274] = 0;
   out_1596973880245959678[275] = 0;
   out_1596973880245959678[276] = 0;
   out_1596973880245959678[277] = 0;
   out_1596973880245959678[278] = 0;
   out_1596973880245959678[279] = 0;
   out_1596973880245959678[280] = 0;
   out_1596973880245959678[281] = 0;
   out_1596973880245959678[282] = 0;
   out_1596973880245959678[283] = 0;
   out_1596973880245959678[284] = 0;
   out_1596973880245959678[285] = 1;
   out_1596973880245959678[286] = 0;
   out_1596973880245959678[287] = 0;
   out_1596973880245959678[288] = 0;
   out_1596973880245959678[289] = 0;
   out_1596973880245959678[290] = 0;
   out_1596973880245959678[291] = 0;
   out_1596973880245959678[292] = 0;
   out_1596973880245959678[293] = 0;
   out_1596973880245959678[294] = 0;
   out_1596973880245959678[295] = 0;
   out_1596973880245959678[296] = 0;
   out_1596973880245959678[297] = 0;
   out_1596973880245959678[298] = 0;
   out_1596973880245959678[299] = 0;
   out_1596973880245959678[300] = 0;
   out_1596973880245959678[301] = 0;
   out_1596973880245959678[302] = 0;
   out_1596973880245959678[303] = 0;
   out_1596973880245959678[304] = 1;
   out_1596973880245959678[305] = 0;
   out_1596973880245959678[306] = 0;
   out_1596973880245959678[307] = 0;
   out_1596973880245959678[308] = 0;
   out_1596973880245959678[309] = 0;
   out_1596973880245959678[310] = 0;
   out_1596973880245959678[311] = 0;
   out_1596973880245959678[312] = 0;
   out_1596973880245959678[313] = 0;
   out_1596973880245959678[314] = 0;
   out_1596973880245959678[315] = 0;
   out_1596973880245959678[316] = 0;
   out_1596973880245959678[317] = 0;
   out_1596973880245959678[318] = 0;
   out_1596973880245959678[319] = 0;
   out_1596973880245959678[320] = 0;
   out_1596973880245959678[321] = 0;
   out_1596973880245959678[322] = 0;
   out_1596973880245959678[323] = 1;
}
void h_4(double *state, double *unused, double *out_622781828802881011) {
   out_622781828802881011[0] = state[6] + state[9];
   out_622781828802881011[1] = state[7] + state[10];
   out_622781828802881011[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_1261848416734986970) {
   out_1261848416734986970[0] = 0;
   out_1261848416734986970[1] = 0;
   out_1261848416734986970[2] = 0;
   out_1261848416734986970[3] = 0;
   out_1261848416734986970[4] = 0;
   out_1261848416734986970[5] = 0;
   out_1261848416734986970[6] = 1;
   out_1261848416734986970[7] = 0;
   out_1261848416734986970[8] = 0;
   out_1261848416734986970[9] = 1;
   out_1261848416734986970[10] = 0;
   out_1261848416734986970[11] = 0;
   out_1261848416734986970[12] = 0;
   out_1261848416734986970[13] = 0;
   out_1261848416734986970[14] = 0;
   out_1261848416734986970[15] = 0;
   out_1261848416734986970[16] = 0;
   out_1261848416734986970[17] = 0;
   out_1261848416734986970[18] = 0;
   out_1261848416734986970[19] = 0;
   out_1261848416734986970[20] = 0;
   out_1261848416734986970[21] = 0;
   out_1261848416734986970[22] = 0;
   out_1261848416734986970[23] = 0;
   out_1261848416734986970[24] = 0;
   out_1261848416734986970[25] = 1;
   out_1261848416734986970[26] = 0;
   out_1261848416734986970[27] = 0;
   out_1261848416734986970[28] = 1;
   out_1261848416734986970[29] = 0;
   out_1261848416734986970[30] = 0;
   out_1261848416734986970[31] = 0;
   out_1261848416734986970[32] = 0;
   out_1261848416734986970[33] = 0;
   out_1261848416734986970[34] = 0;
   out_1261848416734986970[35] = 0;
   out_1261848416734986970[36] = 0;
   out_1261848416734986970[37] = 0;
   out_1261848416734986970[38] = 0;
   out_1261848416734986970[39] = 0;
   out_1261848416734986970[40] = 0;
   out_1261848416734986970[41] = 0;
   out_1261848416734986970[42] = 0;
   out_1261848416734986970[43] = 0;
   out_1261848416734986970[44] = 1;
   out_1261848416734986970[45] = 0;
   out_1261848416734986970[46] = 0;
   out_1261848416734986970[47] = 1;
   out_1261848416734986970[48] = 0;
   out_1261848416734986970[49] = 0;
   out_1261848416734986970[50] = 0;
   out_1261848416734986970[51] = 0;
   out_1261848416734986970[52] = 0;
   out_1261848416734986970[53] = 0;
}
void h_10(double *state, double *unused, double *out_5306685819502841937) {
   out_5306685819502841937[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_5306685819502841937[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_5306685819502841937[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_8350343691043762535) {
   out_8350343691043762535[0] = 0;
   out_8350343691043762535[1] = 9.8100000000000005*cos(state[1]);
   out_8350343691043762535[2] = 0;
   out_8350343691043762535[3] = 0;
   out_8350343691043762535[4] = -state[8];
   out_8350343691043762535[5] = state[7];
   out_8350343691043762535[6] = 0;
   out_8350343691043762535[7] = state[5];
   out_8350343691043762535[8] = -state[4];
   out_8350343691043762535[9] = 0;
   out_8350343691043762535[10] = 0;
   out_8350343691043762535[11] = 0;
   out_8350343691043762535[12] = 1;
   out_8350343691043762535[13] = 0;
   out_8350343691043762535[14] = 0;
   out_8350343691043762535[15] = 1;
   out_8350343691043762535[16] = 0;
   out_8350343691043762535[17] = 0;
   out_8350343691043762535[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_8350343691043762535[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_8350343691043762535[20] = 0;
   out_8350343691043762535[21] = state[8];
   out_8350343691043762535[22] = 0;
   out_8350343691043762535[23] = -state[6];
   out_8350343691043762535[24] = -state[5];
   out_8350343691043762535[25] = 0;
   out_8350343691043762535[26] = state[3];
   out_8350343691043762535[27] = 0;
   out_8350343691043762535[28] = 0;
   out_8350343691043762535[29] = 0;
   out_8350343691043762535[30] = 0;
   out_8350343691043762535[31] = 1;
   out_8350343691043762535[32] = 0;
   out_8350343691043762535[33] = 0;
   out_8350343691043762535[34] = 1;
   out_8350343691043762535[35] = 0;
   out_8350343691043762535[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_8350343691043762535[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_8350343691043762535[38] = 0;
   out_8350343691043762535[39] = -state[7];
   out_8350343691043762535[40] = state[6];
   out_8350343691043762535[41] = 0;
   out_8350343691043762535[42] = state[4];
   out_8350343691043762535[43] = -state[3];
   out_8350343691043762535[44] = 0;
   out_8350343691043762535[45] = 0;
   out_8350343691043762535[46] = 0;
   out_8350343691043762535[47] = 0;
   out_8350343691043762535[48] = 0;
   out_8350343691043762535[49] = 0;
   out_8350343691043762535[50] = 1;
   out_8350343691043762535[51] = 0;
   out_8350343691043762535[52] = 0;
   out_8350343691043762535[53] = 1;
}
void h_13(double *state, double *unused, double *out_9048838141074958611) {
   out_9048838141074958611[0] = state[3];
   out_9048838141074958611[1] = state[4];
   out_9048838141074958611[2] = state[5];
}
void H_13(double *state, double *unused, double *out_1950425408597345831) {
   out_1950425408597345831[0] = 0;
   out_1950425408597345831[1] = 0;
   out_1950425408597345831[2] = 0;
   out_1950425408597345831[3] = 1;
   out_1950425408597345831[4] = 0;
   out_1950425408597345831[5] = 0;
   out_1950425408597345831[6] = 0;
   out_1950425408597345831[7] = 0;
   out_1950425408597345831[8] = 0;
   out_1950425408597345831[9] = 0;
   out_1950425408597345831[10] = 0;
   out_1950425408597345831[11] = 0;
   out_1950425408597345831[12] = 0;
   out_1950425408597345831[13] = 0;
   out_1950425408597345831[14] = 0;
   out_1950425408597345831[15] = 0;
   out_1950425408597345831[16] = 0;
   out_1950425408597345831[17] = 0;
   out_1950425408597345831[18] = 0;
   out_1950425408597345831[19] = 0;
   out_1950425408597345831[20] = 0;
   out_1950425408597345831[21] = 0;
   out_1950425408597345831[22] = 1;
   out_1950425408597345831[23] = 0;
   out_1950425408597345831[24] = 0;
   out_1950425408597345831[25] = 0;
   out_1950425408597345831[26] = 0;
   out_1950425408597345831[27] = 0;
   out_1950425408597345831[28] = 0;
   out_1950425408597345831[29] = 0;
   out_1950425408597345831[30] = 0;
   out_1950425408597345831[31] = 0;
   out_1950425408597345831[32] = 0;
   out_1950425408597345831[33] = 0;
   out_1950425408597345831[34] = 0;
   out_1950425408597345831[35] = 0;
   out_1950425408597345831[36] = 0;
   out_1950425408597345831[37] = 0;
   out_1950425408597345831[38] = 0;
   out_1950425408597345831[39] = 0;
   out_1950425408597345831[40] = 0;
   out_1950425408597345831[41] = 1;
   out_1950425408597345831[42] = 0;
   out_1950425408597345831[43] = 0;
   out_1950425408597345831[44] = 0;
   out_1950425408597345831[45] = 0;
   out_1950425408597345831[46] = 0;
   out_1950425408597345831[47] = 0;
   out_1950425408597345831[48] = 0;
   out_1950425408597345831[49] = 0;
   out_1950425408597345831[50] = 0;
   out_1950425408597345831[51] = 0;
   out_1950425408597345831[52] = 0;
   out_1950425408597345831[53] = 0;
}
void h_14(double *state, double *unused, double *out_2303399324759113969) {
   out_2303399324759113969[0] = state[6];
   out_2303399324759113969[1] = state[7];
   out_2303399324759113969[2] = state[8];
}
void H_14(double *state, double *unused, double *out_2701392439604497559) {
   out_2701392439604497559[0] = 0;
   out_2701392439604497559[1] = 0;
   out_2701392439604497559[2] = 0;
   out_2701392439604497559[3] = 0;
   out_2701392439604497559[4] = 0;
   out_2701392439604497559[5] = 0;
   out_2701392439604497559[6] = 1;
   out_2701392439604497559[7] = 0;
   out_2701392439604497559[8] = 0;
   out_2701392439604497559[9] = 0;
   out_2701392439604497559[10] = 0;
   out_2701392439604497559[11] = 0;
   out_2701392439604497559[12] = 0;
   out_2701392439604497559[13] = 0;
   out_2701392439604497559[14] = 0;
   out_2701392439604497559[15] = 0;
   out_2701392439604497559[16] = 0;
   out_2701392439604497559[17] = 0;
   out_2701392439604497559[18] = 0;
   out_2701392439604497559[19] = 0;
   out_2701392439604497559[20] = 0;
   out_2701392439604497559[21] = 0;
   out_2701392439604497559[22] = 0;
   out_2701392439604497559[23] = 0;
   out_2701392439604497559[24] = 0;
   out_2701392439604497559[25] = 1;
   out_2701392439604497559[26] = 0;
   out_2701392439604497559[27] = 0;
   out_2701392439604497559[28] = 0;
   out_2701392439604497559[29] = 0;
   out_2701392439604497559[30] = 0;
   out_2701392439604497559[31] = 0;
   out_2701392439604497559[32] = 0;
   out_2701392439604497559[33] = 0;
   out_2701392439604497559[34] = 0;
   out_2701392439604497559[35] = 0;
   out_2701392439604497559[36] = 0;
   out_2701392439604497559[37] = 0;
   out_2701392439604497559[38] = 0;
   out_2701392439604497559[39] = 0;
   out_2701392439604497559[40] = 0;
   out_2701392439604497559[41] = 0;
   out_2701392439604497559[42] = 0;
   out_2701392439604497559[43] = 0;
   out_2701392439604497559[44] = 1;
   out_2701392439604497559[45] = 0;
   out_2701392439604497559[46] = 0;
   out_2701392439604497559[47] = 0;
   out_2701392439604497559[48] = 0;
   out_2701392439604497559[49] = 0;
   out_2701392439604497559[50] = 0;
   out_2701392439604497559[51] = 0;
   out_2701392439604497559[52] = 0;
   out_2701392439604497559[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_1637031099889953199) {
  err_fun(nom_x, delta_x, out_1637031099889953199);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3175856492030467000) {
  inv_err_fun(nom_x, true_x, out_3175856492030467000);
}
void pose_H_mod_fun(double *state, double *out_3254015575193517177) {
  H_mod_fun(state, out_3254015575193517177);
}
void pose_f_fun(double *state, double dt, double *out_5452727561293164163) {
  f_fun(state,  dt, out_5452727561293164163);
}
void pose_F_fun(double *state, double dt, double *out_1596973880245959678) {
  F_fun(state,  dt, out_1596973880245959678);
}
void pose_h_4(double *state, double *unused, double *out_622781828802881011) {
  h_4(state, unused, out_622781828802881011);
}
void pose_H_4(double *state, double *unused, double *out_1261848416734986970) {
  H_4(state, unused, out_1261848416734986970);
}
void pose_h_10(double *state, double *unused, double *out_5306685819502841937) {
  h_10(state, unused, out_5306685819502841937);
}
void pose_H_10(double *state, double *unused, double *out_8350343691043762535) {
  H_10(state, unused, out_8350343691043762535);
}
void pose_h_13(double *state, double *unused, double *out_9048838141074958611) {
  h_13(state, unused, out_9048838141074958611);
}
void pose_H_13(double *state, double *unused, double *out_1950425408597345831) {
  H_13(state, unused, out_1950425408597345831);
}
void pose_h_14(double *state, double *unused, double *out_2303399324759113969) {
  h_14(state, unused, out_2303399324759113969);
}
void pose_H_14(double *state, double *unused, double *out_2701392439604497559) {
  H_14(state, unused, out_2701392439604497559);
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
