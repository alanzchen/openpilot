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
void err_fun(double *nom_x, double *delta_x, double *out_2238114088573503330) {
   out_2238114088573503330[0] = delta_x[0] + nom_x[0];
   out_2238114088573503330[1] = delta_x[1] + nom_x[1];
   out_2238114088573503330[2] = delta_x[2] + nom_x[2];
   out_2238114088573503330[3] = delta_x[3] + nom_x[3];
   out_2238114088573503330[4] = delta_x[4] + nom_x[4];
   out_2238114088573503330[5] = delta_x[5] + nom_x[5];
   out_2238114088573503330[6] = delta_x[6] + nom_x[6];
   out_2238114088573503330[7] = delta_x[7] + nom_x[7];
   out_2238114088573503330[8] = delta_x[8] + nom_x[8];
   out_2238114088573503330[9] = delta_x[9] + nom_x[9];
   out_2238114088573503330[10] = delta_x[10] + nom_x[10];
   out_2238114088573503330[11] = delta_x[11] + nom_x[11];
   out_2238114088573503330[12] = delta_x[12] + nom_x[12];
   out_2238114088573503330[13] = delta_x[13] + nom_x[13];
   out_2238114088573503330[14] = delta_x[14] + nom_x[14];
   out_2238114088573503330[15] = delta_x[15] + nom_x[15];
   out_2238114088573503330[16] = delta_x[16] + nom_x[16];
   out_2238114088573503330[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3877213446685125163) {
   out_3877213446685125163[0] = -nom_x[0] + true_x[0];
   out_3877213446685125163[1] = -nom_x[1] + true_x[1];
   out_3877213446685125163[2] = -nom_x[2] + true_x[2];
   out_3877213446685125163[3] = -nom_x[3] + true_x[3];
   out_3877213446685125163[4] = -nom_x[4] + true_x[4];
   out_3877213446685125163[5] = -nom_x[5] + true_x[5];
   out_3877213446685125163[6] = -nom_x[6] + true_x[6];
   out_3877213446685125163[7] = -nom_x[7] + true_x[7];
   out_3877213446685125163[8] = -nom_x[8] + true_x[8];
   out_3877213446685125163[9] = -nom_x[9] + true_x[9];
   out_3877213446685125163[10] = -nom_x[10] + true_x[10];
   out_3877213446685125163[11] = -nom_x[11] + true_x[11];
   out_3877213446685125163[12] = -nom_x[12] + true_x[12];
   out_3877213446685125163[13] = -nom_x[13] + true_x[13];
   out_3877213446685125163[14] = -nom_x[14] + true_x[14];
   out_3877213446685125163[15] = -nom_x[15] + true_x[15];
   out_3877213446685125163[16] = -nom_x[16] + true_x[16];
   out_3877213446685125163[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_7869929370112138903) {
   out_7869929370112138903[0] = 1.0;
   out_7869929370112138903[1] = 0.0;
   out_7869929370112138903[2] = 0.0;
   out_7869929370112138903[3] = 0.0;
   out_7869929370112138903[4] = 0.0;
   out_7869929370112138903[5] = 0.0;
   out_7869929370112138903[6] = 0.0;
   out_7869929370112138903[7] = 0.0;
   out_7869929370112138903[8] = 0.0;
   out_7869929370112138903[9] = 0.0;
   out_7869929370112138903[10] = 0.0;
   out_7869929370112138903[11] = 0.0;
   out_7869929370112138903[12] = 0.0;
   out_7869929370112138903[13] = 0.0;
   out_7869929370112138903[14] = 0.0;
   out_7869929370112138903[15] = 0.0;
   out_7869929370112138903[16] = 0.0;
   out_7869929370112138903[17] = 0.0;
   out_7869929370112138903[18] = 0.0;
   out_7869929370112138903[19] = 1.0;
   out_7869929370112138903[20] = 0.0;
   out_7869929370112138903[21] = 0.0;
   out_7869929370112138903[22] = 0.0;
   out_7869929370112138903[23] = 0.0;
   out_7869929370112138903[24] = 0.0;
   out_7869929370112138903[25] = 0.0;
   out_7869929370112138903[26] = 0.0;
   out_7869929370112138903[27] = 0.0;
   out_7869929370112138903[28] = 0.0;
   out_7869929370112138903[29] = 0.0;
   out_7869929370112138903[30] = 0.0;
   out_7869929370112138903[31] = 0.0;
   out_7869929370112138903[32] = 0.0;
   out_7869929370112138903[33] = 0.0;
   out_7869929370112138903[34] = 0.0;
   out_7869929370112138903[35] = 0.0;
   out_7869929370112138903[36] = 0.0;
   out_7869929370112138903[37] = 0.0;
   out_7869929370112138903[38] = 1.0;
   out_7869929370112138903[39] = 0.0;
   out_7869929370112138903[40] = 0.0;
   out_7869929370112138903[41] = 0.0;
   out_7869929370112138903[42] = 0.0;
   out_7869929370112138903[43] = 0.0;
   out_7869929370112138903[44] = 0.0;
   out_7869929370112138903[45] = 0.0;
   out_7869929370112138903[46] = 0.0;
   out_7869929370112138903[47] = 0.0;
   out_7869929370112138903[48] = 0.0;
   out_7869929370112138903[49] = 0.0;
   out_7869929370112138903[50] = 0.0;
   out_7869929370112138903[51] = 0.0;
   out_7869929370112138903[52] = 0.0;
   out_7869929370112138903[53] = 0.0;
   out_7869929370112138903[54] = 0.0;
   out_7869929370112138903[55] = 0.0;
   out_7869929370112138903[56] = 0.0;
   out_7869929370112138903[57] = 1.0;
   out_7869929370112138903[58] = 0.0;
   out_7869929370112138903[59] = 0.0;
   out_7869929370112138903[60] = 0.0;
   out_7869929370112138903[61] = 0.0;
   out_7869929370112138903[62] = 0.0;
   out_7869929370112138903[63] = 0.0;
   out_7869929370112138903[64] = 0.0;
   out_7869929370112138903[65] = 0.0;
   out_7869929370112138903[66] = 0.0;
   out_7869929370112138903[67] = 0.0;
   out_7869929370112138903[68] = 0.0;
   out_7869929370112138903[69] = 0.0;
   out_7869929370112138903[70] = 0.0;
   out_7869929370112138903[71] = 0.0;
   out_7869929370112138903[72] = 0.0;
   out_7869929370112138903[73] = 0.0;
   out_7869929370112138903[74] = 0.0;
   out_7869929370112138903[75] = 0.0;
   out_7869929370112138903[76] = 1.0;
   out_7869929370112138903[77] = 0.0;
   out_7869929370112138903[78] = 0.0;
   out_7869929370112138903[79] = 0.0;
   out_7869929370112138903[80] = 0.0;
   out_7869929370112138903[81] = 0.0;
   out_7869929370112138903[82] = 0.0;
   out_7869929370112138903[83] = 0.0;
   out_7869929370112138903[84] = 0.0;
   out_7869929370112138903[85] = 0.0;
   out_7869929370112138903[86] = 0.0;
   out_7869929370112138903[87] = 0.0;
   out_7869929370112138903[88] = 0.0;
   out_7869929370112138903[89] = 0.0;
   out_7869929370112138903[90] = 0.0;
   out_7869929370112138903[91] = 0.0;
   out_7869929370112138903[92] = 0.0;
   out_7869929370112138903[93] = 0.0;
   out_7869929370112138903[94] = 0.0;
   out_7869929370112138903[95] = 1.0;
   out_7869929370112138903[96] = 0.0;
   out_7869929370112138903[97] = 0.0;
   out_7869929370112138903[98] = 0.0;
   out_7869929370112138903[99] = 0.0;
   out_7869929370112138903[100] = 0.0;
   out_7869929370112138903[101] = 0.0;
   out_7869929370112138903[102] = 0.0;
   out_7869929370112138903[103] = 0.0;
   out_7869929370112138903[104] = 0.0;
   out_7869929370112138903[105] = 0.0;
   out_7869929370112138903[106] = 0.0;
   out_7869929370112138903[107] = 0.0;
   out_7869929370112138903[108] = 0.0;
   out_7869929370112138903[109] = 0.0;
   out_7869929370112138903[110] = 0.0;
   out_7869929370112138903[111] = 0.0;
   out_7869929370112138903[112] = 0.0;
   out_7869929370112138903[113] = 0.0;
   out_7869929370112138903[114] = 1.0;
   out_7869929370112138903[115] = 0.0;
   out_7869929370112138903[116] = 0.0;
   out_7869929370112138903[117] = 0.0;
   out_7869929370112138903[118] = 0.0;
   out_7869929370112138903[119] = 0.0;
   out_7869929370112138903[120] = 0.0;
   out_7869929370112138903[121] = 0.0;
   out_7869929370112138903[122] = 0.0;
   out_7869929370112138903[123] = 0.0;
   out_7869929370112138903[124] = 0.0;
   out_7869929370112138903[125] = 0.0;
   out_7869929370112138903[126] = 0.0;
   out_7869929370112138903[127] = 0.0;
   out_7869929370112138903[128] = 0.0;
   out_7869929370112138903[129] = 0.0;
   out_7869929370112138903[130] = 0.0;
   out_7869929370112138903[131] = 0.0;
   out_7869929370112138903[132] = 0.0;
   out_7869929370112138903[133] = 1.0;
   out_7869929370112138903[134] = 0.0;
   out_7869929370112138903[135] = 0.0;
   out_7869929370112138903[136] = 0.0;
   out_7869929370112138903[137] = 0.0;
   out_7869929370112138903[138] = 0.0;
   out_7869929370112138903[139] = 0.0;
   out_7869929370112138903[140] = 0.0;
   out_7869929370112138903[141] = 0.0;
   out_7869929370112138903[142] = 0.0;
   out_7869929370112138903[143] = 0.0;
   out_7869929370112138903[144] = 0.0;
   out_7869929370112138903[145] = 0.0;
   out_7869929370112138903[146] = 0.0;
   out_7869929370112138903[147] = 0.0;
   out_7869929370112138903[148] = 0.0;
   out_7869929370112138903[149] = 0.0;
   out_7869929370112138903[150] = 0.0;
   out_7869929370112138903[151] = 0.0;
   out_7869929370112138903[152] = 1.0;
   out_7869929370112138903[153] = 0.0;
   out_7869929370112138903[154] = 0.0;
   out_7869929370112138903[155] = 0.0;
   out_7869929370112138903[156] = 0.0;
   out_7869929370112138903[157] = 0.0;
   out_7869929370112138903[158] = 0.0;
   out_7869929370112138903[159] = 0.0;
   out_7869929370112138903[160] = 0.0;
   out_7869929370112138903[161] = 0.0;
   out_7869929370112138903[162] = 0.0;
   out_7869929370112138903[163] = 0.0;
   out_7869929370112138903[164] = 0.0;
   out_7869929370112138903[165] = 0.0;
   out_7869929370112138903[166] = 0.0;
   out_7869929370112138903[167] = 0.0;
   out_7869929370112138903[168] = 0.0;
   out_7869929370112138903[169] = 0.0;
   out_7869929370112138903[170] = 0.0;
   out_7869929370112138903[171] = 1.0;
   out_7869929370112138903[172] = 0.0;
   out_7869929370112138903[173] = 0.0;
   out_7869929370112138903[174] = 0.0;
   out_7869929370112138903[175] = 0.0;
   out_7869929370112138903[176] = 0.0;
   out_7869929370112138903[177] = 0.0;
   out_7869929370112138903[178] = 0.0;
   out_7869929370112138903[179] = 0.0;
   out_7869929370112138903[180] = 0.0;
   out_7869929370112138903[181] = 0.0;
   out_7869929370112138903[182] = 0.0;
   out_7869929370112138903[183] = 0.0;
   out_7869929370112138903[184] = 0.0;
   out_7869929370112138903[185] = 0.0;
   out_7869929370112138903[186] = 0.0;
   out_7869929370112138903[187] = 0.0;
   out_7869929370112138903[188] = 0.0;
   out_7869929370112138903[189] = 0.0;
   out_7869929370112138903[190] = 1.0;
   out_7869929370112138903[191] = 0.0;
   out_7869929370112138903[192] = 0.0;
   out_7869929370112138903[193] = 0.0;
   out_7869929370112138903[194] = 0.0;
   out_7869929370112138903[195] = 0.0;
   out_7869929370112138903[196] = 0.0;
   out_7869929370112138903[197] = 0.0;
   out_7869929370112138903[198] = 0.0;
   out_7869929370112138903[199] = 0.0;
   out_7869929370112138903[200] = 0.0;
   out_7869929370112138903[201] = 0.0;
   out_7869929370112138903[202] = 0.0;
   out_7869929370112138903[203] = 0.0;
   out_7869929370112138903[204] = 0.0;
   out_7869929370112138903[205] = 0.0;
   out_7869929370112138903[206] = 0.0;
   out_7869929370112138903[207] = 0.0;
   out_7869929370112138903[208] = 0.0;
   out_7869929370112138903[209] = 1.0;
   out_7869929370112138903[210] = 0.0;
   out_7869929370112138903[211] = 0.0;
   out_7869929370112138903[212] = 0.0;
   out_7869929370112138903[213] = 0.0;
   out_7869929370112138903[214] = 0.0;
   out_7869929370112138903[215] = 0.0;
   out_7869929370112138903[216] = 0.0;
   out_7869929370112138903[217] = 0.0;
   out_7869929370112138903[218] = 0.0;
   out_7869929370112138903[219] = 0.0;
   out_7869929370112138903[220] = 0.0;
   out_7869929370112138903[221] = 0.0;
   out_7869929370112138903[222] = 0.0;
   out_7869929370112138903[223] = 0.0;
   out_7869929370112138903[224] = 0.0;
   out_7869929370112138903[225] = 0.0;
   out_7869929370112138903[226] = 0.0;
   out_7869929370112138903[227] = 0.0;
   out_7869929370112138903[228] = 1.0;
   out_7869929370112138903[229] = 0.0;
   out_7869929370112138903[230] = 0.0;
   out_7869929370112138903[231] = 0.0;
   out_7869929370112138903[232] = 0.0;
   out_7869929370112138903[233] = 0.0;
   out_7869929370112138903[234] = 0.0;
   out_7869929370112138903[235] = 0.0;
   out_7869929370112138903[236] = 0.0;
   out_7869929370112138903[237] = 0.0;
   out_7869929370112138903[238] = 0.0;
   out_7869929370112138903[239] = 0.0;
   out_7869929370112138903[240] = 0.0;
   out_7869929370112138903[241] = 0.0;
   out_7869929370112138903[242] = 0.0;
   out_7869929370112138903[243] = 0.0;
   out_7869929370112138903[244] = 0.0;
   out_7869929370112138903[245] = 0.0;
   out_7869929370112138903[246] = 0.0;
   out_7869929370112138903[247] = 1.0;
   out_7869929370112138903[248] = 0.0;
   out_7869929370112138903[249] = 0.0;
   out_7869929370112138903[250] = 0.0;
   out_7869929370112138903[251] = 0.0;
   out_7869929370112138903[252] = 0.0;
   out_7869929370112138903[253] = 0.0;
   out_7869929370112138903[254] = 0.0;
   out_7869929370112138903[255] = 0.0;
   out_7869929370112138903[256] = 0.0;
   out_7869929370112138903[257] = 0.0;
   out_7869929370112138903[258] = 0.0;
   out_7869929370112138903[259] = 0.0;
   out_7869929370112138903[260] = 0.0;
   out_7869929370112138903[261] = 0.0;
   out_7869929370112138903[262] = 0.0;
   out_7869929370112138903[263] = 0.0;
   out_7869929370112138903[264] = 0.0;
   out_7869929370112138903[265] = 0.0;
   out_7869929370112138903[266] = 1.0;
   out_7869929370112138903[267] = 0.0;
   out_7869929370112138903[268] = 0.0;
   out_7869929370112138903[269] = 0.0;
   out_7869929370112138903[270] = 0.0;
   out_7869929370112138903[271] = 0.0;
   out_7869929370112138903[272] = 0.0;
   out_7869929370112138903[273] = 0.0;
   out_7869929370112138903[274] = 0.0;
   out_7869929370112138903[275] = 0.0;
   out_7869929370112138903[276] = 0.0;
   out_7869929370112138903[277] = 0.0;
   out_7869929370112138903[278] = 0.0;
   out_7869929370112138903[279] = 0.0;
   out_7869929370112138903[280] = 0.0;
   out_7869929370112138903[281] = 0.0;
   out_7869929370112138903[282] = 0.0;
   out_7869929370112138903[283] = 0.0;
   out_7869929370112138903[284] = 0.0;
   out_7869929370112138903[285] = 1.0;
   out_7869929370112138903[286] = 0.0;
   out_7869929370112138903[287] = 0.0;
   out_7869929370112138903[288] = 0.0;
   out_7869929370112138903[289] = 0.0;
   out_7869929370112138903[290] = 0.0;
   out_7869929370112138903[291] = 0.0;
   out_7869929370112138903[292] = 0.0;
   out_7869929370112138903[293] = 0.0;
   out_7869929370112138903[294] = 0.0;
   out_7869929370112138903[295] = 0.0;
   out_7869929370112138903[296] = 0.0;
   out_7869929370112138903[297] = 0.0;
   out_7869929370112138903[298] = 0.0;
   out_7869929370112138903[299] = 0.0;
   out_7869929370112138903[300] = 0.0;
   out_7869929370112138903[301] = 0.0;
   out_7869929370112138903[302] = 0.0;
   out_7869929370112138903[303] = 0.0;
   out_7869929370112138903[304] = 1.0;
   out_7869929370112138903[305] = 0.0;
   out_7869929370112138903[306] = 0.0;
   out_7869929370112138903[307] = 0.0;
   out_7869929370112138903[308] = 0.0;
   out_7869929370112138903[309] = 0.0;
   out_7869929370112138903[310] = 0.0;
   out_7869929370112138903[311] = 0.0;
   out_7869929370112138903[312] = 0.0;
   out_7869929370112138903[313] = 0.0;
   out_7869929370112138903[314] = 0.0;
   out_7869929370112138903[315] = 0.0;
   out_7869929370112138903[316] = 0.0;
   out_7869929370112138903[317] = 0.0;
   out_7869929370112138903[318] = 0.0;
   out_7869929370112138903[319] = 0.0;
   out_7869929370112138903[320] = 0.0;
   out_7869929370112138903[321] = 0.0;
   out_7869929370112138903[322] = 0.0;
   out_7869929370112138903[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5672712038044060314) {
   out_5672712038044060314[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5672712038044060314[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5672712038044060314[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5672712038044060314[3] = dt*state[12] + state[3];
   out_5672712038044060314[4] = dt*state[13] + state[4];
   out_5672712038044060314[5] = dt*state[14] + state[5];
   out_5672712038044060314[6] = state[6];
   out_5672712038044060314[7] = state[7];
   out_5672712038044060314[8] = state[8];
   out_5672712038044060314[9] = state[9];
   out_5672712038044060314[10] = state[10];
   out_5672712038044060314[11] = state[11];
   out_5672712038044060314[12] = state[12];
   out_5672712038044060314[13] = state[13];
   out_5672712038044060314[14] = state[14];
   out_5672712038044060314[15] = state[15];
   out_5672712038044060314[16] = state[16];
   out_5672712038044060314[17] = state[17];
}
void F_fun(double *state, double dt, double *out_4762702412345730966) {
   out_4762702412345730966[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4762702412345730966[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4762702412345730966[2] = 0;
   out_4762702412345730966[3] = 0;
   out_4762702412345730966[4] = 0;
   out_4762702412345730966[5] = 0;
   out_4762702412345730966[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4762702412345730966[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4762702412345730966[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_4762702412345730966[9] = 0;
   out_4762702412345730966[10] = 0;
   out_4762702412345730966[11] = 0;
   out_4762702412345730966[12] = 0;
   out_4762702412345730966[13] = 0;
   out_4762702412345730966[14] = 0;
   out_4762702412345730966[15] = 0;
   out_4762702412345730966[16] = 0;
   out_4762702412345730966[17] = 0;
   out_4762702412345730966[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4762702412345730966[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4762702412345730966[20] = 0;
   out_4762702412345730966[21] = 0;
   out_4762702412345730966[22] = 0;
   out_4762702412345730966[23] = 0;
   out_4762702412345730966[24] = 0;
   out_4762702412345730966[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4762702412345730966[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_4762702412345730966[27] = 0;
   out_4762702412345730966[28] = 0;
   out_4762702412345730966[29] = 0;
   out_4762702412345730966[30] = 0;
   out_4762702412345730966[31] = 0;
   out_4762702412345730966[32] = 0;
   out_4762702412345730966[33] = 0;
   out_4762702412345730966[34] = 0;
   out_4762702412345730966[35] = 0;
   out_4762702412345730966[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4762702412345730966[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4762702412345730966[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4762702412345730966[39] = 0;
   out_4762702412345730966[40] = 0;
   out_4762702412345730966[41] = 0;
   out_4762702412345730966[42] = 0;
   out_4762702412345730966[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4762702412345730966[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_4762702412345730966[45] = 0;
   out_4762702412345730966[46] = 0;
   out_4762702412345730966[47] = 0;
   out_4762702412345730966[48] = 0;
   out_4762702412345730966[49] = 0;
   out_4762702412345730966[50] = 0;
   out_4762702412345730966[51] = 0;
   out_4762702412345730966[52] = 0;
   out_4762702412345730966[53] = 0;
   out_4762702412345730966[54] = 0;
   out_4762702412345730966[55] = 0;
   out_4762702412345730966[56] = 0;
   out_4762702412345730966[57] = 1;
   out_4762702412345730966[58] = 0;
   out_4762702412345730966[59] = 0;
   out_4762702412345730966[60] = 0;
   out_4762702412345730966[61] = 0;
   out_4762702412345730966[62] = 0;
   out_4762702412345730966[63] = 0;
   out_4762702412345730966[64] = 0;
   out_4762702412345730966[65] = 0;
   out_4762702412345730966[66] = dt;
   out_4762702412345730966[67] = 0;
   out_4762702412345730966[68] = 0;
   out_4762702412345730966[69] = 0;
   out_4762702412345730966[70] = 0;
   out_4762702412345730966[71] = 0;
   out_4762702412345730966[72] = 0;
   out_4762702412345730966[73] = 0;
   out_4762702412345730966[74] = 0;
   out_4762702412345730966[75] = 0;
   out_4762702412345730966[76] = 1;
   out_4762702412345730966[77] = 0;
   out_4762702412345730966[78] = 0;
   out_4762702412345730966[79] = 0;
   out_4762702412345730966[80] = 0;
   out_4762702412345730966[81] = 0;
   out_4762702412345730966[82] = 0;
   out_4762702412345730966[83] = 0;
   out_4762702412345730966[84] = 0;
   out_4762702412345730966[85] = dt;
   out_4762702412345730966[86] = 0;
   out_4762702412345730966[87] = 0;
   out_4762702412345730966[88] = 0;
   out_4762702412345730966[89] = 0;
   out_4762702412345730966[90] = 0;
   out_4762702412345730966[91] = 0;
   out_4762702412345730966[92] = 0;
   out_4762702412345730966[93] = 0;
   out_4762702412345730966[94] = 0;
   out_4762702412345730966[95] = 1;
   out_4762702412345730966[96] = 0;
   out_4762702412345730966[97] = 0;
   out_4762702412345730966[98] = 0;
   out_4762702412345730966[99] = 0;
   out_4762702412345730966[100] = 0;
   out_4762702412345730966[101] = 0;
   out_4762702412345730966[102] = 0;
   out_4762702412345730966[103] = 0;
   out_4762702412345730966[104] = dt;
   out_4762702412345730966[105] = 0;
   out_4762702412345730966[106] = 0;
   out_4762702412345730966[107] = 0;
   out_4762702412345730966[108] = 0;
   out_4762702412345730966[109] = 0;
   out_4762702412345730966[110] = 0;
   out_4762702412345730966[111] = 0;
   out_4762702412345730966[112] = 0;
   out_4762702412345730966[113] = 0;
   out_4762702412345730966[114] = 1;
   out_4762702412345730966[115] = 0;
   out_4762702412345730966[116] = 0;
   out_4762702412345730966[117] = 0;
   out_4762702412345730966[118] = 0;
   out_4762702412345730966[119] = 0;
   out_4762702412345730966[120] = 0;
   out_4762702412345730966[121] = 0;
   out_4762702412345730966[122] = 0;
   out_4762702412345730966[123] = 0;
   out_4762702412345730966[124] = 0;
   out_4762702412345730966[125] = 0;
   out_4762702412345730966[126] = 0;
   out_4762702412345730966[127] = 0;
   out_4762702412345730966[128] = 0;
   out_4762702412345730966[129] = 0;
   out_4762702412345730966[130] = 0;
   out_4762702412345730966[131] = 0;
   out_4762702412345730966[132] = 0;
   out_4762702412345730966[133] = 1;
   out_4762702412345730966[134] = 0;
   out_4762702412345730966[135] = 0;
   out_4762702412345730966[136] = 0;
   out_4762702412345730966[137] = 0;
   out_4762702412345730966[138] = 0;
   out_4762702412345730966[139] = 0;
   out_4762702412345730966[140] = 0;
   out_4762702412345730966[141] = 0;
   out_4762702412345730966[142] = 0;
   out_4762702412345730966[143] = 0;
   out_4762702412345730966[144] = 0;
   out_4762702412345730966[145] = 0;
   out_4762702412345730966[146] = 0;
   out_4762702412345730966[147] = 0;
   out_4762702412345730966[148] = 0;
   out_4762702412345730966[149] = 0;
   out_4762702412345730966[150] = 0;
   out_4762702412345730966[151] = 0;
   out_4762702412345730966[152] = 1;
   out_4762702412345730966[153] = 0;
   out_4762702412345730966[154] = 0;
   out_4762702412345730966[155] = 0;
   out_4762702412345730966[156] = 0;
   out_4762702412345730966[157] = 0;
   out_4762702412345730966[158] = 0;
   out_4762702412345730966[159] = 0;
   out_4762702412345730966[160] = 0;
   out_4762702412345730966[161] = 0;
   out_4762702412345730966[162] = 0;
   out_4762702412345730966[163] = 0;
   out_4762702412345730966[164] = 0;
   out_4762702412345730966[165] = 0;
   out_4762702412345730966[166] = 0;
   out_4762702412345730966[167] = 0;
   out_4762702412345730966[168] = 0;
   out_4762702412345730966[169] = 0;
   out_4762702412345730966[170] = 0;
   out_4762702412345730966[171] = 1;
   out_4762702412345730966[172] = 0;
   out_4762702412345730966[173] = 0;
   out_4762702412345730966[174] = 0;
   out_4762702412345730966[175] = 0;
   out_4762702412345730966[176] = 0;
   out_4762702412345730966[177] = 0;
   out_4762702412345730966[178] = 0;
   out_4762702412345730966[179] = 0;
   out_4762702412345730966[180] = 0;
   out_4762702412345730966[181] = 0;
   out_4762702412345730966[182] = 0;
   out_4762702412345730966[183] = 0;
   out_4762702412345730966[184] = 0;
   out_4762702412345730966[185] = 0;
   out_4762702412345730966[186] = 0;
   out_4762702412345730966[187] = 0;
   out_4762702412345730966[188] = 0;
   out_4762702412345730966[189] = 0;
   out_4762702412345730966[190] = 1;
   out_4762702412345730966[191] = 0;
   out_4762702412345730966[192] = 0;
   out_4762702412345730966[193] = 0;
   out_4762702412345730966[194] = 0;
   out_4762702412345730966[195] = 0;
   out_4762702412345730966[196] = 0;
   out_4762702412345730966[197] = 0;
   out_4762702412345730966[198] = 0;
   out_4762702412345730966[199] = 0;
   out_4762702412345730966[200] = 0;
   out_4762702412345730966[201] = 0;
   out_4762702412345730966[202] = 0;
   out_4762702412345730966[203] = 0;
   out_4762702412345730966[204] = 0;
   out_4762702412345730966[205] = 0;
   out_4762702412345730966[206] = 0;
   out_4762702412345730966[207] = 0;
   out_4762702412345730966[208] = 0;
   out_4762702412345730966[209] = 1;
   out_4762702412345730966[210] = 0;
   out_4762702412345730966[211] = 0;
   out_4762702412345730966[212] = 0;
   out_4762702412345730966[213] = 0;
   out_4762702412345730966[214] = 0;
   out_4762702412345730966[215] = 0;
   out_4762702412345730966[216] = 0;
   out_4762702412345730966[217] = 0;
   out_4762702412345730966[218] = 0;
   out_4762702412345730966[219] = 0;
   out_4762702412345730966[220] = 0;
   out_4762702412345730966[221] = 0;
   out_4762702412345730966[222] = 0;
   out_4762702412345730966[223] = 0;
   out_4762702412345730966[224] = 0;
   out_4762702412345730966[225] = 0;
   out_4762702412345730966[226] = 0;
   out_4762702412345730966[227] = 0;
   out_4762702412345730966[228] = 1;
   out_4762702412345730966[229] = 0;
   out_4762702412345730966[230] = 0;
   out_4762702412345730966[231] = 0;
   out_4762702412345730966[232] = 0;
   out_4762702412345730966[233] = 0;
   out_4762702412345730966[234] = 0;
   out_4762702412345730966[235] = 0;
   out_4762702412345730966[236] = 0;
   out_4762702412345730966[237] = 0;
   out_4762702412345730966[238] = 0;
   out_4762702412345730966[239] = 0;
   out_4762702412345730966[240] = 0;
   out_4762702412345730966[241] = 0;
   out_4762702412345730966[242] = 0;
   out_4762702412345730966[243] = 0;
   out_4762702412345730966[244] = 0;
   out_4762702412345730966[245] = 0;
   out_4762702412345730966[246] = 0;
   out_4762702412345730966[247] = 1;
   out_4762702412345730966[248] = 0;
   out_4762702412345730966[249] = 0;
   out_4762702412345730966[250] = 0;
   out_4762702412345730966[251] = 0;
   out_4762702412345730966[252] = 0;
   out_4762702412345730966[253] = 0;
   out_4762702412345730966[254] = 0;
   out_4762702412345730966[255] = 0;
   out_4762702412345730966[256] = 0;
   out_4762702412345730966[257] = 0;
   out_4762702412345730966[258] = 0;
   out_4762702412345730966[259] = 0;
   out_4762702412345730966[260] = 0;
   out_4762702412345730966[261] = 0;
   out_4762702412345730966[262] = 0;
   out_4762702412345730966[263] = 0;
   out_4762702412345730966[264] = 0;
   out_4762702412345730966[265] = 0;
   out_4762702412345730966[266] = 1;
   out_4762702412345730966[267] = 0;
   out_4762702412345730966[268] = 0;
   out_4762702412345730966[269] = 0;
   out_4762702412345730966[270] = 0;
   out_4762702412345730966[271] = 0;
   out_4762702412345730966[272] = 0;
   out_4762702412345730966[273] = 0;
   out_4762702412345730966[274] = 0;
   out_4762702412345730966[275] = 0;
   out_4762702412345730966[276] = 0;
   out_4762702412345730966[277] = 0;
   out_4762702412345730966[278] = 0;
   out_4762702412345730966[279] = 0;
   out_4762702412345730966[280] = 0;
   out_4762702412345730966[281] = 0;
   out_4762702412345730966[282] = 0;
   out_4762702412345730966[283] = 0;
   out_4762702412345730966[284] = 0;
   out_4762702412345730966[285] = 1;
   out_4762702412345730966[286] = 0;
   out_4762702412345730966[287] = 0;
   out_4762702412345730966[288] = 0;
   out_4762702412345730966[289] = 0;
   out_4762702412345730966[290] = 0;
   out_4762702412345730966[291] = 0;
   out_4762702412345730966[292] = 0;
   out_4762702412345730966[293] = 0;
   out_4762702412345730966[294] = 0;
   out_4762702412345730966[295] = 0;
   out_4762702412345730966[296] = 0;
   out_4762702412345730966[297] = 0;
   out_4762702412345730966[298] = 0;
   out_4762702412345730966[299] = 0;
   out_4762702412345730966[300] = 0;
   out_4762702412345730966[301] = 0;
   out_4762702412345730966[302] = 0;
   out_4762702412345730966[303] = 0;
   out_4762702412345730966[304] = 1;
   out_4762702412345730966[305] = 0;
   out_4762702412345730966[306] = 0;
   out_4762702412345730966[307] = 0;
   out_4762702412345730966[308] = 0;
   out_4762702412345730966[309] = 0;
   out_4762702412345730966[310] = 0;
   out_4762702412345730966[311] = 0;
   out_4762702412345730966[312] = 0;
   out_4762702412345730966[313] = 0;
   out_4762702412345730966[314] = 0;
   out_4762702412345730966[315] = 0;
   out_4762702412345730966[316] = 0;
   out_4762702412345730966[317] = 0;
   out_4762702412345730966[318] = 0;
   out_4762702412345730966[319] = 0;
   out_4762702412345730966[320] = 0;
   out_4762702412345730966[321] = 0;
   out_4762702412345730966[322] = 0;
   out_4762702412345730966[323] = 1;
}
void h_4(double *state, double *unused, double *out_6450886787262042442) {
   out_6450886787262042442[0] = state[6] + state[9];
   out_6450886787262042442[1] = state[7] + state[10];
   out_6450886787262042442[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_6805007493753505141) {
   out_6805007493753505141[0] = 0;
   out_6805007493753505141[1] = 0;
   out_6805007493753505141[2] = 0;
   out_6805007493753505141[3] = 0;
   out_6805007493753505141[4] = 0;
   out_6805007493753505141[5] = 0;
   out_6805007493753505141[6] = 1;
   out_6805007493753505141[7] = 0;
   out_6805007493753505141[8] = 0;
   out_6805007493753505141[9] = 1;
   out_6805007493753505141[10] = 0;
   out_6805007493753505141[11] = 0;
   out_6805007493753505141[12] = 0;
   out_6805007493753505141[13] = 0;
   out_6805007493753505141[14] = 0;
   out_6805007493753505141[15] = 0;
   out_6805007493753505141[16] = 0;
   out_6805007493753505141[17] = 0;
   out_6805007493753505141[18] = 0;
   out_6805007493753505141[19] = 0;
   out_6805007493753505141[20] = 0;
   out_6805007493753505141[21] = 0;
   out_6805007493753505141[22] = 0;
   out_6805007493753505141[23] = 0;
   out_6805007493753505141[24] = 0;
   out_6805007493753505141[25] = 1;
   out_6805007493753505141[26] = 0;
   out_6805007493753505141[27] = 0;
   out_6805007493753505141[28] = 1;
   out_6805007493753505141[29] = 0;
   out_6805007493753505141[30] = 0;
   out_6805007493753505141[31] = 0;
   out_6805007493753505141[32] = 0;
   out_6805007493753505141[33] = 0;
   out_6805007493753505141[34] = 0;
   out_6805007493753505141[35] = 0;
   out_6805007493753505141[36] = 0;
   out_6805007493753505141[37] = 0;
   out_6805007493753505141[38] = 0;
   out_6805007493753505141[39] = 0;
   out_6805007493753505141[40] = 0;
   out_6805007493753505141[41] = 0;
   out_6805007493753505141[42] = 0;
   out_6805007493753505141[43] = 0;
   out_6805007493753505141[44] = 1;
   out_6805007493753505141[45] = 0;
   out_6805007493753505141[46] = 0;
   out_6805007493753505141[47] = 1;
   out_6805007493753505141[48] = 0;
   out_6805007493753505141[49] = 0;
   out_6805007493753505141[50] = 0;
   out_6805007493753505141[51] = 0;
   out_6805007493753505141[52] = 0;
   out_6805007493753505141[53] = 0;
}
void h_10(double *state, double *unused, double *out_7174068847243736276) {
   out_7174068847243736276[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_7174068847243736276[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_7174068847243736276[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5919592439627785975) {
   out_5919592439627785975[0] = 0;
   out_5919592439627785975[1] = 9.8100000000000005*cos(state[1]);
   out_5919592439627785975[2] = 0;
   out_5919592439627785975[3] = 0;
   out_5919592439627785975[4] = -state[8];
   out_5919592439627785975[5] = state[7];
   out_5919592439627785975[6] = 0;
   out_5919592439627785975[7] = state[5];
   out_5919592439627785975[8] = -state[4];
   out_5919592439627785975[9] = 0;
   out_5919592439627785975[10] = 0;
   out_5919592439627785975[11] = 0;
   out_5919592439627785975[12] = 1;
   out_5919592439627785975[13] = 0;
   out_5919592439627785975[14] = 0;
   out_5919592439627785975[15] = 1;
   out_5919592439627785975[16] = 0;
   out_5919592439627785975[17] = 0;
   out_5919592439627785975[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5919592439627785975[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5919592439627785975[20] = 0;
   out_5919592439627785975[21] = state[8];
   out_5919592439627785975[22] = 0;
   out_5919592439627785975[23] = -state[6];
   out_5919592439627785975[24] = -state[5];
   out_5919592439627785975[25] = 0;
   out_5919592439627785975[26] = state[3];
   out_5919592439627785975[27] = 0;
   out_5919592439627785975[28] = 0;
   out_5919592439627785975[29] = 0;
   out_5919592439627785975[30] = 0;
   out_5919592439627785975[31] = 1;
   out_5919592439627785975[32] = 0;
   out_5919592439627785975[33] = 0;
   out_5919592439627785975[34] = 1;
   out_5919592439627785975[35] = 0;
   out_5919592439627785975[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5919592439627785975[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5919592439627785975[38] = 0;
   out_5919592439627785975[39] = -state[7];
   out_5919592439627785975[40] = state[6];
   out_5919592439627785975[41] = 0;
   out_5919592439627785975[42] = state[4];
   out_5919592439627785975[43] = -state[3];
   out_5919592439627785975[44] = 0;
   out_5919592439627785975[45] = 0;
   out_5919592439627785975[46] = 0;
   out_5919592439627785975[47] = 0;
   out_5919592439627785975[48] = 0;
   out_5919592439627785975[49] = 0;
   out_5919592439627785975[50] = 1;
   out_5919592439627785975[51] = 0;
   out_5919592439627785975[52] = 0;
   out_5919592439627785975[53] = 1;
}
void h_13(double *state, double *unused, double *out_4032282206185544656) {
   out_4032282206185544656[0] = state[3];
   out_4032282206185544656[1] = state[4];
   out_4032282206185544656[2] = state[5];
}
void H_13(double *state, double *unused, double *out_805623714563195788) {
   out_805623714563195788[0] = 0;
   out_805623714563195788[1] = 0;
   out_805623714563195788[2] = 0;
   out_805623714563195788[3] = 1;
   out_805623714563195788[4] = 0;
   out_805623714563195788[5] = 0;
   out_805623714563195788[6] = 0;
   out_805623714563195788[7] = 0;
   out_805623714563195788[8] = 0;
   out_805623714563195788[9] = 0;
   out_805623714563195788[10] = 0;
   out_805623714563195788[11] = 0;
   out_805623714563195788[12] = 0;
   out_805623714563195788[13] = 0;
   out_805623714563195788[14] = 0;
   out_805623714563195788[15] = 0;
   out_805623714563195788[16] = 0;
   out_805623714563195788[17] = 0;
   out_805623714563195788[18] = 0;
   out_805623714563195788[19] = 0;
   out_805623714563195788[20] = 0;
   out_805623714563195788[21] = 0;
   out_805623714563195788[22] = 1;
   out_805623714563195788[23] = 0;
   out_805623714563195788[24] = 0;
   out_805623714563195788[25] = 0;
   out_805623714563195788[26] = 0;
   out_805623714563195788[27] = 0;
   out_805623714563195788[28] = 0;
   out_805623714563195788[29] = 0;
   out_805623714563195788[30] = 0;
   out_805623714563195788[31] = 0;
   out_805623714563195788[32] = 0;
   out_805623714563195788[33] = 0;
   out_805623714563195788[34] = 0;
   out_805623714563195788[35] = 0;
   out_805623714563195788[36] = 0;
   out_805623714563195788[37] = 0;
   out_805623714563195788[38] = 0;
   out_805623714563195788[39] = 0;
   out_805623714563195788[40] = 0;
   out_805623714563195788[41] = 1;
   out_805623714563195788[42] = 0;
   out_805623714563195788[43] = 0;
   out_805623714563195788[44] = 0;
   out_805623714563195788[45] = 0;
   out_805623714563195788[46] = 0;
   out_805623714563195788[47] = 0;
   out_805623714563195788[48] = 0;
   out_805623714563195788[49] = 0;
   out_805623714563195788[50] = 0;
   out_805623714563195788[51] = 0;
   out_805623714563195788[52] = 0;
   out_805623714563195788[53] = 0;
}
void h_14(double *state, double *unused, double *out_6526222386602091277) {
   out_6526222386602091277[0] = state[6];
   out_6526222386602091277[1] = state[7];
   out_6526222386602091277[2] = state[8];
}
void H_14(double *state, double *unused, double *out_8558948147660674179) {
   out_8558948147660674179[0] = 0;
   out_8558948147660674179[1] = 0;
   out_8558948147660674179[2] = 0;
   out_8558948147660674179[3] = 0;
   out_8558948147660674179[4] = 0;
   out_8558948147660674179[5] = 0;
   out_8558948147660674179[6] = 1;
   out_8558948147660674179[7] = 0;
   out_8558948147660674179[8] = 0;
   out_8558948147660674179[9] = 0;
   out_8558948147660674179[10] = 0;
   out_8558948147660674179[11] = 0;
   out_8558948147660674179[12] = 0;
   out_8558948147660674179[13] = 0;
   out_8558948147660674179[14] = 0;
   out_8558948147660674179[15] = 0;
   out_8558948147660674179[16] = 0;
   out_8558948147660674179[17] = 0;
   out_8558948147660674179[18] = 0;
   out_8558948147660674179[19] = 0;
   out_8558948147660674179[20] = 0;
   out_8558948147660674179[21] = 0;
   out_8558948147660674179[22] = 0;
   out_8558948147660674179[23] = 0;
   out_8558948147660674179[24] = 0;
   out_8558948147660674179[25] = 1;
   out_8558948147660674179[26] = 0;
   out_8558948147660674179[27] = 0;
   out_8558948147660674179[28] = 0;
   out_8558948147660674179[29] = 0;
   out_8558948147660674179[30] = 0;
   out_8558948147660674179[31] = 0;
   out_8558948147660674179[32] = 0;
   out_8558948147660674179[33] = 0;
   out_8558948147660674179[34] = 0;
   out_8558948147660674179[35] = 0;
   out_8558948147660674179[36] = 0;
   out_8558948147660674179[37] = 0;
   out_8558948147660674179[38] = 0;
   out_8558948147660674179[39] = 0;
   out_8558948147660674179[40] = 0;
   out_8558948147660674179[41] = 0;
   out_8558948147660674179[42] = 0;
   out_8558948147660674179[43] = 0;
   out_8558948147660674179[44] = 1;
   out_8558948147660674179[45] = 0;
   out_8558948147660674179[46] = 0;
   out_8558948147660674179[47] = 0;
   out_8558948147660674179[48] = 0;
   out_8558948147660674179[49] = 0;
   out_8558948147660674179[50] = 0;
   out_8558948147660674179[51] = 0;
   out_8558948147660674179[52] = 0;
   out_8558948147660674179[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_2238114088573503330) {
  err_fun(nom_x, delta_x, out_2238114088573503330);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_3877213446685125163) {
  inv_err_fun(nom_x, true_x, out_3877213446685125163);
}
void pose_H_mod_fun(double *state, double *out_7869929370112138903) {
  H_mod_fun(state, out_7869929370112138903);
}
void pose_f_fun(double *state, double dt, double *out_5672712038044060314) {
  f_fun(state,  dt, out_5672712038044060314);
}
void pose_F_fun(double *state, double dt, double *out_4762702412345730966) {
  F_fun(state,  dt, out_4762702412345730966);
}
void pose_h_4(double *state, double *unused, double *out_6450886787262042442) {
  h_4(state, unused, out_6450886787262042442);
}
void pose_H_4(double *state, double *unused, double *out_6805007493753505141) {
  H_4(state, unused, out_6805007493753505141);
}
void pose_h_10(double *state, double *unused, double *out_7174068847243736276) {
  h_10(state, unused, out_7174068847243736276);
}
void pose_H_10(double *state, double *unused, double *out_5919592439627785975) {
  H_10(state, unused, out_5919592439627785975);
}
void pose_h_13(double *state, double *unused, double *out_4032282206185544656) {
  h_13(state, unused, out_4032282206185544656);
}
void pose_H_13(double *state, double *unused, double *out_805623714563195788) {
  H_13(state, unused, out_805623714563195788);
}
void pose_h_14(double *state, double *unused, double *out_6526222386602091277) {
  h_14(state, unused, out_6526222386602091277);
}
void pose_H_14(double *state, double *unused, double *out_8558948147660674179) {
  H_14(state, unused, out_8558948147660674179);
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
