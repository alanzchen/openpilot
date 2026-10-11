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
void err_fun(double *nom_x, double *delta_x, double *out_5884898945529161916) {
   out_5884898945529161916[0] = delta_x[0] + nom_x[0];
   out_5884898945529161916[1] = delta_x[1] + nom_x[1];
   out_5884898945529161916[2] = delta_x[2] + nom_x[2];
   out_5884898945529161916[3] = delta_x[3] + nom_x[3];
   out_5884898945529161916[4] = delta_x[4] + nom_x[4];
   out_5884898945529161916[5] = delta_x[5] + nom_x[5];
   out_5884898945529161916[6] = delta_x[6] + nom_x[6];
   out_5884898945529161916[7] = delta_x[7] + nom_x[7];
   out_5884898945529161916[8] = delta_x[8] + nom_x[8];
   out_5884898945529161916[9] = delta_x[9] + nom_x[9];
   out_5884898945529161916[10] = delta_x[10] + nom_x[10];
   out_5884898945529161916[11] = delta_x[11] + nom_x[11];
   out_5884898945529161916[12] = delta_x[12] + nom_x[12];
   out_5884898945529161916[13] = delta_x[13] + nom_x[13];
   out_5884898945529161916[14] = delta_x[14] + nom_x[14];
   out_5884898945529161916[15] = delta_x[15] + nom_x[15];
   out_5884898945529161916[16] = delta_x[16] + nom_x[16];
   out_5884898945529161916[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2695974985561036334) {
   out_2695974985561036334[0] = -nom_x[0] + true_x[0];
   out_2695974985561036334[1] = -nom_x[1] + true_x[1];
   out_2695974985561036334[2] = -nom_x[2] + true_x[2];
   out_2695974985561036334[3] = -nom_x[3] + true_x[3];
   out_2695974985561036334[4] = -nom_x[4] + true_x[4];
   out_2695974985561036334[5] = -nom_x[5] + true_x[5];
   out_2695974985561036334[6] = -nom_x[6] + true_x[6];
   out_2695974985561036334[7] = -nom_x[7] + true_x[7];
   out_2695974985561036334[8] = -nom_x[8] + true_x[8];
   out_2695974985561036334[9] = -nom_x[9] + true_x[9];
   out_2695974985561036334[10] = -nom_x[10] + true_x[10];
   out_2695974985561036334[11] = -nom_x[11] + true_x[11];
   out_2695974985561036334[12] = -nom_x[12] + true_x[12];
   out_2695974985561036334[13] = -nom_x[13] + true_x[13];
   out_2695974985561036334[14] = -nom_x[14] + true_x[14];
   out_2695974985561036334[15] = -nom_x[15] + true_x[15];
   out_2695974985561036334[16] = -nom_x[16] + true_x[16];
   out_2695974985561036334[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_3237876647370796076) {
   out_3237876647370796076[0] = 1.0;
   out_3237876647370796076[1] = 0.0;
   out_3237876647370796076[2] = 0.0;
   out_3237876647370796076[3] = 0.0;
   out_3237876647370796076[4] = 0.0;
   out_3237876647370796076[5] = 0.0;
   out_3237876647370796076[6] = 0.0;
   out_3237876647370796076[7] = 0.0;
   out_3237876647370796076[8] = 0.0;
   out_3237876647370796076[9] = 0.0;
   out_3237876647370796076[10] = 0.0;
   out_3237876647370796076[11] = 0.0;
   out_3237876647370796076[12] = 0.0;
   out_3237876647370796076[13] = 0.0;
   out_3237876647370796076[14] = 0.0;
   out_3237876647370796076[15] = 0.0;
   out_3237876647370796076[16] = 0.0;
   out_3237876647370796076[17] = 0.0;
   out_3237876647370796076[18] = 0.0;
   out_3237876647370796076[19] = 1.0;
   out_3237876647370796076[20] = 0.0;
   out_3237876647370796076[21] = 0.0;
   out_3237876647370796076[22] = 0.0;
   out_3237876647370796076[23] = 0.0;
   out_3237876647370796076[24] = 0.0;
   out_3237876647370796076[25] = 0.0;
   out_3237876647370796076[26] = 0.0;
   out_3237876647370796076[27] = 0.0;
   out_3237876647370796076[28] = 0.0;
   out_3237876647370796076[29] = 0.0;
   out_3237876647370796076[30] = 0.0;
   out_3237876647370796076[31] = 0.0;
   out_3237876647370796076[32] = 0.0;
   out_3237876647370796076[33] = 0.0;
   out_3237876647370796076[34] = 0.0;
   out_3237876647370796076[35] = 0.0;
   out_3237876647370796076[36] = 0.0;
   out_3237876647370796076[37] = 0.0;
   out_3237876647370796076[38] = 1.0;
   out_3237876647370796076[39] = 0.0;
   out_3237876647370796076[40] = 0.0;
   out_3237876647370796076[41] = 0.0;
   out_3237876647370796076[42] = 0.0;
   out_3237876647370796076[43] = 0.0;
   out_3237876647370796076[44] = 0.0;
   out_3237876647370796076[45] = 0.0;
   out_3237876647370796076[46] = 0.0;
   out_3237876647370796076[47] = 0.0;
   out_3237876647370796076[48] = 0.0;
   out_3237876647370796076[49] = 0.0;
   out_3237876647370796076[50] = 0.0;
   out_3237876647370796076[51] = 0.0;
   out_3237876647370796076[52] = 0.0;
   out_3237876647370796076[53] = 0.0;
   out_3237876647370796076[54] = 0.0;
   out_3237876647370796076[55] = 0.0;
   out_3237876647370796076[56] = 0.0;
   out_3237876647370796076[57] = 1.0;
   out_3237876647370796076[58] = 0.0;
   out_3237876647370796076[59] = 0.0;
   out_3237876647370796076[60] = 0.0;
   out_3237876647370796076[61] = 0.0;
   out_3237876647370796076[62] = 0.0;
   out_3237876647370796076[63] = 0.0;
   out_3237876647370796076[64] = 0.0;
   out_3237876647370796076[65] = 0.0;
   out_3237876647370796076[66] = 0.0;
   out_3237876647370796076[67] = 0.0;
   out_3237876647370796076[68] = 0.0;
   out_3237876647370796076[69] = 0.0;
   out_3237876647370796076[70] = 0.0;
   out_3237876647370796076[71] = 0.0;
   out_3237876647370796076[72] = 0.0;
   out_3237876647370796076[73] = 0.0;
   out_3237876647370796076[74] = 0.0;
   out_3237876647370796076[75] = 0.0;
   out_3237876647370796076[76] = 1.0;
   out_3237876647370796076[77] = 0.0;
   out_3237876647370796076[78] = 0.0;
   out_3237876647370796076[79] = 0.0;
   out_3237876647370796076[80] = 0.0;
   out_3237876647370796076[81] = 0.0;
   out_3237876647370796076[82] = 0.0;
   out_3237876647370796076[83] = 0.0;
   out_3237876647370796076[84] = 0.0;
   out_3237876647370796076[85] = 0.0;
   out_3237876647370796076[86] = 0.0;
   out_3237876647370796076[87] = 0.0;
   out_3237876647370796076[88] = 0.0;
   out_3237876647370796076[89] = 0.0;
   out_3237876647370796076[90] = 0.0;
   out_3237876647370796076[91] = 0.0;
   out_3237876647370796076[92] = 0.0;
   out_3237876647370796076[93] = 0.0;
   out_3237876647370796076[94] = 0.0;
   out_3237876647370796076[95] = 1.0;
   out_3237876647370796076[96] = 0.0;
   out_3237876647370796076[97] = 0.0;
   out_3237876647370796076[98] = 0.0;
   out_3237876647370796076[99] = 0.0;
   out_3237876647370796076[100] = 0.0;
   out_3237876647370796076[101] = 0.0;
   out_3237876647370796076[102] = 0.0;
   out_3237876647370796076[103] = 0.0;
   out_3237876647370796076[104] = 0.0;
   out_3237876647370796076[105] = 0.0;
   out_3237876647370796076[106] = 0.0;
   out_3237876647370796076[107] = 0.0;
   out_3237876647370796076[108] = 0.0;
   out_3237876647370796076[109] = 0.0;
   out_3237876647370796076[110] = 0.0;
   out_3237876647370796076[111] = 0.0;
   out_3237876647370796076[112] = 0.0;
   out_3237876647370796076[113] = 0.0;
   out_3237876647370796076[114] = 1.0;
   out_3237876647370796076[115] = 0.0;
   out_3237876647370796076[116] = 0.0;
   out_3237876647370796076[117] = 0.0;
   out_3237876647370796076[118] = 0.0;
   out_3237876647370796076[119] = 0.0;
   out_3237876647370796076[120] = 0.0;
   out_3237876647370796076[121] = 0.0;
   out_3237876647370796076[122] = 0.0;
   out_3237876647370796076[123] = 0.0;
   out_3237876647370796076[124] = 0.0;
   out_3237876647370796076[125] = 0.0;
   out_3237876647370796076[126] = 0.0;
   out_3237876647370796076[127] = 0.0;
   out_3237876647370796076[128] = 0.0;
   out_3237876647370796076[129] = 0.0;
   out_3237876647370796076[130] = 0.0;
   out_3237876647370796076[131] = 0.0;
   out_3237876647370796076[132] = 0.0;
   out_3237876647370796076[133] = 1.0;
   out_3237876647370796076[134] = 0.0;
   out_3237876647370796076[135] = 0.0;
   out_3237876647370796076[136] = 0.0;
   out_3237876647370796076[137] = 0.0;
   out_3237876647370796076[138] = 0.0;
   out_3237876647370796076[139] = 0.0;
   out_3237876647370796076[140] = 0.0;
   out_3237876647370796076[141] = 0.0;
   out_3237876647370796076[142] = 0.0;
   out_3237876647370796076[143] = 0.0;
   out_3237876647370796076[144] = 0.0;
   out_3237876647370796076[145] = 0.0;
   out_3237876647370796076[146] = 0.0;
   out_3237876647370796076[147] = 0.0;
   out_3237876647370796076[148] = 0.0;
   out_3237876647370796076[149] = 0.0;
   out_3237876647370796076[150] = 0.0;
   out_3237876647370796076[151] = 0.0;
   out_3237876647370796076[152] = 1.0;
   out_3237876647370796076[153] = 0.0;
   out_3237876647370796076[154] = 0.0;
   out_3237876647370796076[155] = 0.0;
   out_3237876647370796076[156] = 0.0;
   out_3237876647370796076[157] = 0.0;
   out_3237876647370796076[158] = 0.0;
   out_3237876647370796076[159] = 0.0;
   out_3237876647370796076[160] = 0.0;
   out_3237876647370796076[161] = 0.0;
   out_3237876647370796076[162] = 0.0;
   out_3237876647370796076[163] = 0.0;
   out_3237876647370796076[164] = 0.0;
   out_3237876647370796076[165] = 0.0;
   out_3237876647370796076[166] = 0.0;
   out_3237876647370796076[167] = 0.0;
   out_3237876647370796076[168] = 0.0;
   out_3237876647370796076[169] = 0.0;
   out_3237876647370796076[170] = 0.0;
   out_3237876647370796076[171] = 1.0;
   out_3237876647370796076[172] = 0.0;
   out_3237876647370796076[173] = 0.0;
   out_3237876647370796076[174] = 0.0;
   out_3237876647370796076[175] = 0.0;
   out_3237876647370796076[176] = 0.0;
   out_3237876647370796076[177] = 0.0;
   out_3237876647370796076[178] = 0.0;
   out_3237876647370796076[179] = 0.0;
   out_3237876647370796076[180] = 0.0;
   out_3237876647370796076[181] = 0.0;
   out_3237876647370796076[182] = 0.0;
   out_3237876647370796076[183] = 0.0;
   out_3237876647370796076[184] = 0.0;
   out_3237876647370796076[185] = 0.0;
   out_3237876647370796076[186] = 0.0;
   out_3237876647370796076[187] = 0.0;
   out_3237876647370796076[188] = 0.0;
   out_3237876647370796076[189] = 0.0;
   out_3237876647370796076[190] = 1.0;
   out_3237876647370796076[191] = 0.0;
   out_3237876647370796076[192] = 0.0;
   out_3237876647370796076[193] = 0.0;
   out_3237876647370796076[194] = 0.0;
   out_3237876647370796076[195] = 0.0;
   out_3237876647370796076[196] = 0.0;
   out_3237876647370796076[197] = 0.0;
   out_3237876647370796076[198] = 0.0;
   out_3237876647370796076[199] = 0.0;
   out_3237876647370796076[200] = 0.0;
   out_3237876647370796076[201] = 0.0;
   out_3237876647370796076[202] = 0.0;
   out_3237876647370796076[203] = 0.0;
   out_3237876647370796076[204] = 0.0;
   out_3237876647370796076[205] = 0.0;
   out_3237876647370796076[206] = 0.0;
   out_3237876647370796076[207] = 0.0;
   out_3237876647370796076[208] = 0.0;
   out_3237876647370796076[209] = 1.0;
   out_3237876647370796076[210] = 0.0;
   out_3237876647370796076[211] = 0.0;
   out_3237876647370796076[212] = 0.0;
   out_3237876647370796076[213] = 0.0;
   out_3237876647370796076[214] = 0.0;
   out_3237876647370796076[215] = 0.0;
   out_3237876647370796076[216] = 0.0;
   out_3237876647370796076[217] = 0.0;
   out_3237876647370796076[218] = 0.0;
   out_3237876647370796076[219] = 0.0;
   out_3237876647370796076[220] = 0.0;
   out_3237876647370796076[221] = 0.0;
   out_3237876647370796076[222] = 0.0;
   out_3237876647370796076[223] = 0.0;
   out_3237876647370796076[224] = 0.0;
   out_3237876647370796076[225] = 0.0;
   out_3237876647370796076[226] = 0.0;
   out_3237876647370796076[227] = 0.0;
   out_3237876647370796076[228] = 1.0;
   out_3237876647370796076[229] = 0.0;
   out_3237876647370796076[230] = 0.0;
   out_3237876647370796076[231] = 0.0;
   out_3237876647370796076[232] = 0.0;
   out_3237876647370796076[233] = 0.0;
   out_3237876647370796076[234] = 0.0;
   out_3237876647370796076[235] = 0.0;
   out_3237876647370796076[236] = 0.0;
   out_3237876647370796076[237] = 0.0;
   out_3237876647370796076[238] = 0.0;
   out_3237876647370796076[239] = 0.0;
   out_3237876647370796076[240] = 0.0;
   out_3237876647370796076[241] = 0.0;
   out_3237876647370796076[242] = 0.0;
   out_3237876647370796076[243] = 0.0;
   out_3237876647370796076[244] = 0.0;
   out_3237876647370796076[245] = 0.0;
   out_3237876647370796076[246] = 0.0;
   out_3237876647370796076[247] = 1.0;
   out_3237876647370796076[248] = 0.0;
   out_3237876647370796076[249] = 0.0;
   out_3237876647370796076[250] = 0.0;
   out_3237876647370796076[251] = 0.0;
   out_3237876647370796076[252] = 0.0;
   out_3237876647370796076[253] = 0.0;
   out_3237876647370796076[254] = 0.0;
   out_3237876647370796076[255] = 0.0;
   out_3237876647370796076[256] = 0.0;
   out_3237876647370796076[257] = 0.0;
   out_3237876647370796076[258] = 0.0;
   out_3237876647370796076[259] = 0.0;
   out_3237876647370796076[260] = 0.0;
   out_3237876647370796076[261] = 0.0;
   out_3237876647370796076[262] = 0.0;
   out_3237876647370796076[263] = 0.0;
   out_3237876647370796076[264] = 0.0;
   out_3237876647370796076[265] = 0.0;
   out_3237876647370796076[266] = 1.0;
   out_3237876647370796076[267] = 0.0;
   out_3237876647370796076[268] = 0.0;
   out_3237876647370796076[269] = 0.0;
   out_3237876647370796076[270] = 0.0;
   out_3237876647370796076[271] = 0.0;
   out_3237876647370796076[272] = 0.0;
   out_3237876647370796076[273] = 0.0;
   out_3237876647370796076[274] = 0.0;
   out_3237876647370796076[275] = 0.0;
   out_3237876647370796076[276] = 0.0;
   out_3237876647370796076[277] = 0.0;
   out_3237876647370796076[278] = 0.0;
   out_3237876647370796076[279] = 0.0;
   out_3237876647370796076[280] = 0.0;
   out_3237876647370796076[281] = 0.0;
   out_3237876647370796076[282] = 0.0;
   out_3237876647370796076[283] = 0.0;
   out_3237876647370796076[284] = 0.0;
   out_3237876647370796076[285] = 1.0;
   out_3237876647370796076[286] = 0.0;
   out_3237876647370796076[287] = 0.0;
   out_3237876647370796076[288] = 0.0;
   out_3237876647370796076[289] = 0.0;
   out_3237876647370796076[290] = 0.0;
   out_3237876647370796076[291] = 0.0;
   out_3237876647370796076[292] = 0.0;
   out_3237876647370796076[293] = 0.0;
   out_3237876647370796076[294] = 0.0;
   out_3237876647370796076[295] = 0.0;
   out_3237876647370796076[296] = 0.0;
   out_3237876647370796076[297] = 0.0;
   out_3237876647370796076[298] = 0.0;
   out_3237876647370796076[299] = 0.0;
   out_3237876647370796076[300] = 0.0;
   out_3237876647370796076[301] = 0.0;
   out_3237876647370796076[302] = 0.0;
   out_3237876647370796076[303] = 0.0;
   out_3237876647370796076[304] = 1.0;
   out_3237876647370796076[305] = 0.0;
   out_3237876647370796076[306] = 0.0;
   out_3237876647370796076[307] = 0.0;
   out_3237876647370796076[308] = 0.0;
   out_3237876647370796076[309] = 0.0;
   out_3237876647370796076[310] = 0.0;
   out_3237876647370796076[311] = 0.0;
   out_3237876647370796076[312] = 0.0;
   out_3237876647370796076[313] = 0.0;
   out_3237876647370796076[314] = 0.0;
   out_3237876647370796076[315] = 0.0;
   out_3237876647370796076[316] = 0.0;
   out_3237876647370796076[317] = 0.0;
   out_3237876647370796076[318] = 0.0;
   out_3237876647370796076[319] = 0.0;
   out_3237876647370796076[320] = 0.0;
   out_3237876647370796076[321] = 0.0;
   out_3237876647370796076[322] = 0.0;
   out_3237876647370796076[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5341964247148914054) {
   out_5341964247148914054[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5341964247148914054[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5341964247148914054[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5341964247148914054[3] = dt*state[12] + state[3];
   out_5341964247148914054[4] = dt*state[13] + state[4];
   out_5341964247148914054[5] = dt*state[14] + state[5];
   out_5341964247148914054[6] = state[6];
   out_5341964247148914054[7] = state[7];
   out_5341964247148914054[8] = state[8];
   out_5341964247148914054[9] = state[9];
   out_5341964247148914054[10] = state[10];
   out_5341964247148914054[11] = state[11];
   out_5341964247148914054[12] = state[12];
   out_5341964247148914054[13] = state[13];
   out_5341964247148914054[14] = state[14];
   out_5341964247148914054[15] = state[15];
   out_5341964247148914054[16] = state[16];
   out_5341964247148914054[17] = state[17];
}
void F_fun(double *state, double dt, double *out_6962674312365040456) {
   out_6962674312365040456[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6962674312365040456[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6962674312365040456[2] = 0;
   out_6962674312365040456[3] = 0;
   out_6962674312365040456[4] = 0;
   out_6962674312365040456[5] = 0;
   out_6962674312365040456[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6962674312365040456[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6962674312365040456[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_6962674312365040456[9] = 0;
   out_6962674312365040456[10] = 0;
   out_6962674312365040456[11] = 0;
   out_6962674312365040456[12] = 0;
   out_6962674312365040456[13] = 0;
   out_6962674312365040456[14] = 0;
   out_6962674312365040456[15] = 0;
   out_6962674312365040456[16] = 0;
   out_6962674312365040456[17] = 0;
   out_6962674312365040456[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6962674312365040456[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6962674312365040456[20] = 0;
   out_6962674312365040456[21] = 0;
   out_6962674312365040456[22] = 0;
   out_6962674312365040456[23] = 0;
   out_6962674312365040456[24] = 0;
   out_6962674312365040456[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6962674312365040456[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_6962674312365040456[27] = 0;
   out_6962674312365040456[28] = 0;
   out_6962674312365040456[29] = 0;
   out_6962674312365040456[30] = 0;
   out_6962674312365040456[31] = 0;
   out_6962674312365040456[32] = 0;
   out_6962674312365040456[33] = 0;
   out_6962674312365040456[34] = 0;
   out_6962674312365040456[35] = 0;
   out_6962674312365040456[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6962674312365040456[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6962674312365040456[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6962674312365040456[39] = 0;
   out_6962674312365040456[40] = 0;
   out_6962674312365040456[41] = 0;
   out_6962674312365040456[42] = 0;
   out_6962674312365040456[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6962674312365040456[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_6962674312365040456[45] = 0;
   out_6962674312365040456[46] = 0;
   out_6962674312365040456[47] = 0;
   out_6962674312365040456[48] = 0;
   out_6962674312365040456[49] = 0;
   out_6962674312365040456[50] = 0;
   out_6962674312365040456[51] = 0;
   out_6962674312365040456[52] = 0;
   out_6962674312365040456[53] = 0;
   out_6962674312365040456[54] = 0;
   out_6962674312365040456[55] = 0;
   out_6962674312365040456[56] = 0;
   out_6962674312365040456[57] = 1;
   out_6962674312365040456[58] = 0;
   out_6962674312365040456[59] = 0;
   out_6962674312365040456[60] = 0;
   out_6962674312365040456[61] = 0;
   out_6962674312365040456[62] = 0;
   out_6962674312365040456[63] = 0;
   out_6962674312365040456[64] = 0;
   out_6962674312365040456[65] = 0;
   out_6962674312365040456[66] = dt;
   out_6962674312365040456[67] = 0;
   out_6962674312365040456[68] = 0;
   out_6962674312365040456[69] = 0;
   out_6962674312365040456[70] = 0;
   out_6962674312365040456[71] = 0;
   out_6962674312365040456[72] = 0;
   out_6962674312365040456[73] = 0;
   out_6962674312365040456[74] = 0;
   out_6962674312365040456[75] = 0;
   out_6962674312365040456[76] = 1;
   out_6962674312365040456[77] = 0;
   out_6962674312365040456[78] = 0;
   out_6962674312365040456[79] = 0;
   out_6962674312365040456[80] = 0;
   out_6962674312365040456[81] = 0;
   out_6962674312365040456[82] = 0;
   out_6962674312365040456[83] = 0;
   out_6962674312365040456[84] = 0;
   out_6962674312365040456[85] = dt;
   out_6962674312365040456[86] = 0;
   out_6962674312365040456[87] = 0;
   out_6962674312365040456[88] = 0;
   out_6962674312365040456[89] = 0;
   out_6962674312365040456[90] = 0;
   out_6962674312365040456[91] = 0;
   out_6962674312365040456[92] = 0;
   out_6962674312365040456[93] = 0;
   out_6962674312365040456[94] = 0;
   out_6962674312365040456[95] = 1;
   out_6962674312365040456[96] = 0;
   out_6962674312365040456[97] = 0;
   out_6962674312365040456[98] = 0;
   out_6962674312365040456[99] = 0;
   out_6962674312365040456[100] = 0;
   out_6962674312365040456[101] = 0;
   out_6962674312365040456[102] = 0;
   out_6962674312365040456[103] = 0;
   out_6962674312365040456[104] = dt;
   out_6962674312365040456[105] = 0;
   out_6962674312365040456[106] = 0;
   out_6962674312365040456[107] = 0;
   out_6962674312365040456[108] = 0;
   out_6962674312365040456[109] = 0;
   out_6962674312365040456[110] = 0;
   out_6962674312365040456[111] = 0;
   out_6962674312365040456[112] = 0;
   out_6962674312365040456[113] = 0;
   out_6962674312365040456[114] = 1;
   out_6962674312365040456[115] = 0;
   out_6962674312365040456[116] = 0;
   out_6962674312365040456[117] = 0;
   out_6962674312365040456[118] = 0;
   out_6962674312365040456[119] = 0;
   out_6962674312365040456[120] = 0;
   out_6962674312365040456[121] = 0;
   out_6962674312365040456[122] = 0;
   out_6962674312365040456[123] = 0;
   out_6962674312365040456[124] = 0;
   out_6962674312365040456[125] = 0;
   out_6962674312365040456[126] = 0;
   out_6962674312365040456[127] = 0;
   out_6962674312365040456[128] = 0;
   out_6962674312365040456[129] = 0;
   out_6962674312365040456[130] = 0;
   out_6962674312365040456[131] = 0;
   out_6962674312365040456[132] = 0;
   out_6962674312365040456[133] = 1;
   out_6962674312365040456[134] = 0;
   out_6962674312365040456[135] = 0;
   out_6962674312365040456[136] = 0;
   out_6962674312365040456[137] = 0;
   out_6962674312365040456[138] = 0;
   out_6962674312365040456[139] = 0;
   out_6962674312365040456[140] = 0;
   out_6962674312365040456[141] = 0;
   out_6962674312365040456[142] = 0;
   out_6962674312365040456[143] = 0;
   out_6962674312365040456[144] = 0;
   out_6962674312365040456[145] = 0;
   out_6962674312365040456[146] = 0;
   out_6962674312365040456[147] = 0;
   out_6962674312365040456[148] = 0;
   out_6962674312365040456[149] = 0;
   out_6962674312365040456[150] = 0;
   out_6962674312365040456[151] = 0;
   out_6962674312365040456[152] = 1;
   out_6962674312365040456[153] = 0;
   out_6962674312365040456[154] = 0;
   out_6962674312365040456[155] = 0;
   out_6962674312365040456[156] = 0;
   out_6962674312365040456[157] = 0;
   out_6962674312365040456[158] = 0;
   out_6962674312365040456[159] = 0;
   out_6962674312365040456[160] = 0;
   out_6962674312365040456[161] = 0;
   out_6962674312365040456[162] = 0;
   out_6962674312365040456[163] = 0;
   out_6962674312365040456[164] = 0;
   out_6962674312365040456[165] = 0;
   out_6962674312365040456[166] = 0;
   out_6962674312365040456[167] = 0;
   out_6962674312365040456[168] = 0;
   out_6962674312365040456[169] = 0;
   out_6962674312365040456[170] = 0;
   out_6962674312365040456[171] = 1;
   out_6962674312365040456[172] = 0;
   out_6962674312365040456[173] = 0;
   out_6962674312365040456[174] = 0;
   out_6962674312365040456[175] = 0;
   out_6962674312365040456[176] = 0;
   out_6962674312365040456[177] = 0;
   out_6962674312365040456[178] = 0;
   out_6962674312365040456[179] = 0;
   out_6962674312365040456[180] = 0;
   out_6962674312365040456[181] = 0;
   out_6962674312365040456[182] = 0;
   out_6962674312365040456[183] = 0;
   out_6962674312365040456[184] = 0;
   out_6962674312365040456[185] = 0;
   out_6962674312365040456[186] = 0;
   out_6962674312365040456[187] = 0;
   out_6962674312365040456[188] = 0;
   out_6962674312365040456[189] = 0;
   out_6962674312365040456[190] = 1;
   out_6962674312365040456[191] = 0;
   out_6962674312365040456[192] = 0;
   out_6962674312365040456[193] = 0;
   out_6962674312365040456[194] = 0;
   out_6962674312365040456[195] = 0;
   out_6962674312365040456[196] = 0;
   out_6962674312365040456[197] = 0;
   out_6962674312365040456[198] = 0;
   out_6962674312365040456[199] = 0;
   out_6962674312365040456[200] = 0;
   out_6962674312365040456[201] = 0;
   out_6962674312365040456[202] = 0;
   out_6962674312365040456[203] = 0;
   out_6962674312365040456[204] = 0;
   out_6962674312365040456[205] = 0;
   out_6962674312365040456[206] = 0;
   out_6962674312365040456[207] = 0;
   out_6962674312365040456[208] = 0;
   out_6962674312365040456[209] = 1;
   out_6962674312365040456[210] = 0;
   out_6962674312365040456[211] = 0;
   out_6962674312365040456[212] = 0;
   out_6962674312365040456[213] = 0;
   out_6962674312365040456[214] = 0;
   out_6962674312365040456[215] = 0;
   out_6962674312365040456[216] = 0;
   out_6962674312365040456[217] = 0;
   out_6962674312365040456[218] = 0;
   out_6962674312365040456[219] = 0;
   out_6962674312365040456[220] = 0;
   out_6962674312365040456[221] = 0;
   out_6962674312365040456[222] = 0;
   out_6962674312365040456[223] = 0;
   out_6962674312365040456[224] = 0;
   out_6962674312365040456[225] = 0;
   out_6962674312365040456[226] = 0;
   out_6962674312365040456[227] = 0;
   out_6962674312365040456[228] = 1;
   out_6962674312365040456[229] = 0;
   out_6962674312365040456[230] = 0;
   out_6962674312365040456[231] = 0;
   out_6962674312365040456[232] = 0;
   out_6962674312365040456[233] = 0;
   out_6962674312365040456[234] = 0;
   out_6962674312365040456[235] = 0;
   out_6962674312365040456[236] = 0;
   out_6962674312365040456[237] = 0;
   out_6962674312365040456[238] = 0;
   out_6962674312365040456[239] = 0;
   out_6962674312365040456[240] = 0;
   out_6962674312365040456[241] = 0;
   out_6962674312365040456[242] = 0;
   out_6962674312365040456[243] = 0;
   out_6962674312365040456[244] = 0;
   out_6962674312365040456[245] = 0;
   out_6962674312365040456[246] = 0;
   out_6962674312365040456[247] = 1;
   out_6962674312365040456[248] = 0;
   out_6962674312365040456[249] = 0;
   out_6962674312365040456[250] = 0;
   out_6962674312365040456[251] = 0;
   out_6962674312365040456[252] = 0;
   out_6962674312365040456[253] = 0;
   out_6962674312365040456[254] = 0;
   out_6962674312365040456[255] = 0;
   out_6962674312365040456[256] = 0;
   out_6962674312365040456[257] = 0;
   out_6962674312365040456[258] = 0;
   out_6962674312365040456[259] = 0;
   out_6962674312365040456[260] = 0;
   out_6962674312365040456[261] = 0;
   out_6962674312365040456[262] = 0;
   out_6962674312365040456[263] = 0;
   out_6962674312365040456[264] = 0;
   out_6962674312365040456[265] = 0;
   out_6962674312365040456[266] = 1;
   out_6962674312365040456[267] = 0;
   out_6962674312365040456[268] = 0;
   out_6962674312365040456[269] = 0;
   out_6962674312365040456[270] = 0;
   out_6962674312365040456[271] = 0;
   out_6962674312365040456[272] = 0;
   out_6962674312365040456[273] = 0;
   out_6962674312365040456[274] = 0;
   out_6962674312365040456[275] = 0;
   out_6962674312365040456[276] = 0;
   out_6962674312365040456[277] = 0;
   out_6962674312365040456[278] = 0;
   out_6962674312365040456[279] = 0;
   out_6962674312365040456[280] = 0;
   out_6962674312365040456[281] = 0;
   out_6962674312365040456[282] = 0;
   out_6962674312365040456[283] = 0;
   out_6962674312365040456[284] = 0;
   out_6962674312365040456[285] = 1;
   out_6962674312365040456[286] = 0;
   out_6962674312365040456[287] = 0;
   out_6962674312365040456[288] = 0;
   out_6962674312365040456[289] = 0;
   out_6962674312365040456[290] = 0;
   out_6962674312365040456[291] = 0;
   out_6962674312365040456[292] = 0;
   out_6962674312365040456[293] = 0;
   out_6962674312365040456[294] = 0;
   out_6962674312365040456[295] = 0;
   out_6962674312365040456[296] = 0;
   out_6962674312365040456[297] = 0;
   out_6962674312365040456[298] = 0;
   out_6962674312365040456[299] = 0;
   out_6962674312365040456[300] = 0;
   out_6962674312365040456[301] = 0;
   out_6962674312365040456[302] = 0;
   out_6962674312365040456[303] = 0;
   out_6962674312365040456[304] = 1;
   out_6962674312365040456[305] = 0;
   out_6962674312365040456[306] = 0;
   out_6962674312365040456[307] = 0;
   out_6962674312365040456[308] = 0;
   out_6962674312365040456[309] = 0;
   out_6962674312365040456[310] = 0;
   out_6962674312365040456[311] = 0;
   out_6962674312365040456[312] = 0;
   out_6962674312365040456[313] = 0;
   out_6962674312365040456[314] = 0;
   out_6962674312365040456[315] = 0;
   out_6962674312365040456[316] = 0;
   out_6962674312365040456[317] = 0;
   out_6962674312365040456[318] = 0;
   out_6962674312365040456[319] = 0;
   out_6962674312365040456[320] = 0;
   out_6962674312365040456[321] = 0;
   out_6962674312365040456[322] = 0;
   out_6962674312365040456[323] = 1;
}
void h_4(double *state, double *unused, double *out_802636545138050071) {
   out_802636545138050071[0] = state[6] + state[9];
   out_802636545138050071[1] = state[7] + state[10];
   out_802636545138050071[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_8390395032409876811) {
   out_8390395032409876811[0] = 0;
   out_8390395032409876811[1] = 0;
   out_8390395032409876811[2] = 0;
   out_8390395032409876811[3] = 0;
   out_8390395032409876811[4] = 0;
   out_8390395032409876811[5] = 0;
   out_8390395032409876811[6] = 1;
   out_8390395032409876811[7] = 0;
   out_8390395032409876811[8] = 0;
   out_8390395032409876811[9] = 1;
   out_8390395032409876811[10] = 0;
   out_8390395032409876811[11] = 0;
   out_8390395032409876811[12] = 0;
   out_8390395032409876811[13] = 0;
   out_8390395032409876811[14] = 0;
   out_8390395032409876811[15] = 0;
   out_8390395032409876811[16] = 0;
   out_8390395032409876811[17] = 0;
   out_8390395032409876811[18] = 0;
   out_8390395032409876811[19] = 0;
   out_8390395032409876811[20] = 0;
   out_8390395032409876811[21] = 0;
   out_8390395032409876811[22] = 0;
   out_8390395032409876811[23] = 0;
   out_8390395032409876811[24] = 0;
   out_8390395032409876811[25] = 1;
   out_8390395032409876811[26] = 0;
   out_8390395032409876811[27] = 0;
   out_8390395032409876811[28] = 1;
   out_8390395032409876811[29] = 0;
   out_8390395032409876811[30] = 0;
   out_8390395032409876811[31] = 0;
   out_8390395032409876811[32] = 0;
   out_8390395032409876811[33] = 0;
   out_8390395032409876811[34] = 0;
   out_8390395032409876811[35] = 0;
   out_8390395032409876811[36] = 0;
   out_8390395032409876811[37] = 0;
   out_8390395032409876811[38] = 0;
   out_8390395032409876811[39] = 0;
   out_8390395032409876811[40] = 0;
   out_8390395032409876811[41] = 0;
   out_8390395032409876811[42] = 0;
   out_8390395032409876811[43] = 0;
   out_8390395032409876811[44] = 1;
   out_8390395032409876811[45] = 0;
   out_8390395032409876811[46] = 0;
   out_8390395032409876811[47] = 1;
   out_8390395032409876811[48] = 0;
   out_8390395032409876811[49] = 0;
   out_8390395032409876811[50] = 0;
   out_8390395032409876811[51] = 0;
   out_8390395032409876811[52] = 0;
   out_8390395032409876811[53] = 0;
}
void h_10(double *state, double *unused, double *out_3869789647935931974) {
   out_3869789647935931974[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_3869789647935931974[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_3869789647935931974[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5760458429482568539) {
   out_5760458429482568539[0] = 0;
   out_5760458429482568539[1] = 9.8100000000000005*cos(state[1]);
   out_5760458429482568539[2] = 0;
   out_5760458429482568539[3] = 0;
   out_5760458429482568539[4] = -state[8];
   out_5760458429482568539[5] = state[7];
   out_5760458429482568539[6] = 0;
   out_5760458429482568539[7] = state[5];
   out_5760458429482568539[8] = -state[4];
   out_5760458429482568539[9] = 0;
   out_5760458429482568539[10] = 0;
   out_5760458429482568539[11] = 0;
   out_5760458429482568539[12] = 1;
   out_5760458429482568539[13] = 0;
   out_5760458429482568539[14] = 0;
   out_5760458429482568539[15] = 1;
   out_5760458429482568539[16] = 0;
   out_5760458429482568539[17] = 0;
   out_5760458429482568539[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5760458429482568539[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5760458429482568539[20] = 0;
   out_5760458429482568539[21] = state[8];
   out_5760458429482568539[22] = 0;
   out_5760458429482568539[23] = -state[6];
   out_5760458429482568539[24] = -state[5];
   out_5760458429482568539[25] = 0;
   out_5760458429482568539[26] = state[3];
   out_5760458429482568539[27] = 0;
   out_5760458429482568539[28] = 0;
   out_5760458429482568539[29] = 0;
   out_5760458429482568539[30] = 0;
   out_5760458429482568539[31] = 1;
   out_5760458429482568539[32] = 0;
   out_5760458429482568539[33] = 0;
   out_5760458429482568539[34] = 1;
   out_5760458429482568539[35] = 0;
   out_5760458429482568539[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5760458429482568539[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5760458429482568539[38] = 0;
   out_5760458429482568539[39] = -state[7];
   out_5760458429482568539[40] = state[6];
   out_5760458429482568539[41] = 0;
   out_5760458429482568539[42] = state[4];
   out_5760458429482568539[43] = -state[3];
   out_5760458429482568539[44] = 0;
   out_5760458429482568539[45] = 0;
   out_5760458429482568539[46] = 0;
   out_5760458429482568539[47] = 0;
   out_5760458429482568539[48] = 0;
   out_5760458429482568539[49] = 0;
   out_5760458429482568539[50] = 1;
   out_5760458429482568539[51] = 0;
   out_5760458429482568539[52] = 0;
   out_5760458429482568539[53] = 1;
}
void h_13(double *state, double *unused, double *out_9172956192586326054) {
   out_9172956192586326054[0] = state[3];
   out_9172956192586326054[1] = state[4];
   out_9172956192586326054[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4556639569107352787) {
   out_4556639569107352787[0] = 0;
   out_4556639569107352787[1] = 0;
   out_4556639569107352787[2] = 0;
   out_4556639569107352787[3] = 1;
   out_4556639569107352787[4] = 0;
   out_4556639569107352787[5] = 0;
   out_4556639569107352787[6] = 0;
   out_4556639569107352787[7] = 0;
   out_4556639569107352787[8] = 0;
   out_4556639569107352787[9] = 0;
   out_4556639569107352787[10] = 0;
   out_4556639569107352787[11] = 0;
   out_4556639569107352787[12] = 0;
   out_4556639569107352787[13] = 0;
   out_4556639569107352787[14] = 0;
   out_4556639569107352787[15] = 0;
   out_4556639569107352787[16] = 0;
   out_4556639569107352787[17] = 0;
   out_4556639569107352787[18] = 0;
   out_4556639569107352787[19] = 0;
   out_4556639569107352787[20] = 0;
   out_4556639569107352787[21] = 0;
   out_4556639569107352787[22] = 1;
   out_4556639569107352787[23] = 0;
   out_4556639569107352787[24] = 0;
   out_4556639569107352787[25] = 0;
   out_4556639569107352787[26] = 0;
   out_4556639569107352787[27] = 0;
   out_4556639569107352787[28] = 0;
   out_4556639569107352787[29] = 0;
   out_4556639569107352787[30] = 0;
   out_4556639569107352787[31] = 0;
   out_4556639569107352787[32] = 0;
   out_4556639569107352787[33] = 0;
   out_4556639569107352787[34] = 0;
   out_4556639569107352787[35] = 0;
   out_4556639569107352787[36] = 0;
   out_4556639569107352787[37] = 0;
   out_4556639569107352787[38] = 0;
   out_4556639569107352787[39] = 0;
   out_4556639569107352787[40] = 0;
   out_4556639569107352787[41] = 1;
   out_4556639569107352787[42] = 0;
   out_4556639569107352787[43] = 0;
   out_4556639569107352787[44] = 0;
   out_4556639569107352787[45] = 0;
   out_4556639569107352787[46] = 0;
   out_4556639569107352787[47] = 0;
   out_4556639569107352787[48] = 0;
   out_4556639569107352787[49] = 0;
   out_4556639569107352787[50] = 0;
   out_4556639569107352787[51] = 0;
   out_4556639569107352787[52] = 0;
   out_4556639569107352787[53] = 0;
}
void h_14(double *state, double *unused, double *out_6465394495488652181) {
   out_6465394495488652181[0] = state[6];
   out_6465394495488652181[1] = state[7];
   out_6465394495488652181[2] = state[8];
}
void H_14(double *state, double *unused, double *out_5307606600114504515) {
   out_5307606600114504515[0] = 0;
   out_5307606600114504515[1] = 0;
   out_5307606600114504515[2] = 0;
   out_5307606600114504515[3] = 0;
   out_5307606600114504515[4] = 0;
   out_5307606600114504515[5] = 0;
   out_5307606600114504515[6] = 1;
   out_5307606600114504515[7] = 0;
   out_5307606600114504515[8] = 0;
   out_5307606600114504515[9] = 0;
   out_5307606600114504515[10] = 0;
   out_5307606600114504515[11] = 0;
   out_5307606600114504515[12] = 0;
   out_5307606600114504515[13] = 0;
   out_5307606600114504515[14] = 0;
   out_5307606600114504515[15] = 0;
   out_5307606600114504515[16] = 0;
   out_5307606600114504515[17] = 0;
   out_5307606600114504515[18] = 0;
   out_5307606600114504515[19] = 0;
   out_5307606600114504515[20] = 0;
   out_5307606600114504515[21] = 0;
   out_5307606600114504515[22] = 0;
   out_5307606600114504515[23] = 0;
   out_5307606600114504515[24] = 0;
   out_5307606600114504515[25] = 1;
   out_5307606600114504515[26] = 0;
   out_5307606600114504515[27] = 0;
   out_5307606600114504515[28] = 0;
   out_5307606600114504515[29] = 0;
   out_5307606600114504515[30] = 0;
   out_5307606600114504515[31] = 0;
   out_5307606600114504515[32] = 0;
   out_5307606600114504515[33] = 0;
   out_5307606600114504515[34] = 0;
   out_5307606600114504515[35] = 0;
   out_5307606600114504515[36] = 0;
   out_5307606600114504515[37] = 0;
   out_5307606600114504515[38] = 0;
   out_5307606600114504515[39] = 0;
   out_5307606600114504515[40] = 0;
   out_5307606600114504515[41] = 0;
   out_5307606600114504515[42] = 0;
   out_5307606600114504515[43] = 0;
   out_5307606600114504515[44] = 1;
   out_5307606600114504515[45] = 0;
   out_5307606600114504515[46] = 0;
   out_5307606600114504515[47] = 0;
   out_5307606600114504515[48] = 0;
   out_5307606600114504515[49] = 0;
   out_5307606600114504515[50] = 0;
   out_5307606600114504515[51] = 0;
   out_5307606600114504515[52] = 0;
   out_5307606600114504515[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_5884898945529161916) {
  err_fun(nom_x, delta_x, out_5884898945529161916);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2695974985561036334) {
  inv_err_fun(nom_x, true_x, out_2695974985561036334);
}
void pose_H_mod_fun(double *state, double *out_3237876647370796076) {
  H_mod_fun(state, out_3237876647370796076);
}
void pose_f_fun(double *state, double dt, double *out_5341964247148914054) {
  f_fun(state,  dt, out_5341964247148914054);
}
void pose_F_fun(double *state, double dt, double *out_6962674312365040456) {
  F_fun(state,  dt, out_6962674312365040456);
}
void pose_h_4(double *state, double *unused, double *out_802636545138050071) {
  h_4(state, unused, out_802636545138050071);
}
void pose_H_4(double *state, double *unused, double *out_8390395032409876811) {
  H_4(state, unused, out_8390395032409876811);
}
void pose_h_10(double *state, double *unused, double *out_3869789647935931974) {
  h_10(state, unused, out_3869789647935931974);
}
void pose_H_10(double *state, double *unused, double *out_5760458429482568539) {
  H_10(state, unused, out_5760458429482568539);
}
void pose_h_13(double *state, double *unused, double *out_9172956192586326054) {
  h_13(state, unused, out_9172956192586326054);
}
void pose_H_13(double *state, double *unused, double *out_4556639569107352787) {
  H_13(state, unused, out_4556639569107352787);
}
void pose_h_14(double *state, double *unused, double *out_6465394495488652181) {
  h_14(state, unused, out_6465394495488652181);
}
void pose_H_14(double *state, double *unused, double *out_5307606600114504515) {
  H_14(state, unused, out_5307606600114504515);
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
