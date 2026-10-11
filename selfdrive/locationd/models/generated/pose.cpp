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
void err_fun(double *nom_x, double *delta_x, double *out_1794537510029647999) {
   out_1794537510029647999[0] = delta_x[0] + nom_x[0];
   out_1794537510029647999[1] = delta_x[1] + nom_x[1];
   out_1794537510029647999[2] = delta_x[2] + nom_x[2];
   out_1794537510029647999[3] = delta_x[3] + nom_x[3];
   out_1794537510029647999[4] = delta_x[4] + nom_x[4];
   out_1794537510029647999[5] = delta_x[5] + nom_x[5];
   out_1794537510029647999[6] = delta_x[6] + nom_x[6];
   out_1794537510029647999[7] = delta_x[7] + nom_x[7];
   out_1794537510029647999[8] = delta_x[8] + nom_x[8];
   out_1794537510029647999[9] = delta_x[9] + nom_x[9];
   out_1794537510029647999[10] = delta_x[10] + nom_x[10];
   out_1794537510029647999[11] = delta_x[11] + nom_x[11];
   out_1794537510029647999[12] = delta_x[12] + nom_x[12];
   out_1794537510029647999[13] = delta_x[13] + nom_x[13];
   out_1794537510029647999[14] = delta_x[14] + nom_x[14];
   out_1794537510029647999[15] = delta_x[15] + nom_x[15];
   out_1794537510029647999[16] = delta_x[16] + nom_x[16];
   out_1794537510029647999[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1818321109360324212) {
   out_1818321109360324212[0] = -nom_x[0] + true_x[0];
   out_1818321109360324212[1] = -nom_x[1] + true_x[1];
   out_1818321109360324212[2] = -nom_x[2] + true_x[2];
   out_1818321109360324212[3] = -nom_x[3] + true_x[3];
   out_1818321109360324212[4] = -nom_x[4] + true_x[4];
   out_1818321109360324212[5] = -nom_x[5] + true_x[5];
   out_1818321109360324212[6] = -nom_x[6] + true_x[6];
   out_1818321109360324212[7] = -nom_x[7] + true_x[7];
   out_1818321109360324212[8] = -nom_x[8] + true_x[8];
   out_1818321109360324212[9] = -nom_x[9] + true_x[9];
   out_1818321109360324212[10] = -nom_x[10] + true_x[10];
   out_1818321109360324212[11] = -nom_x[11] + true_x[11];
   out_1818321109360324212[12] = -nom_x[12] + true_x[12];
   out_1818321109360324212[13] = -nom_x[13] + true_x[13];
   out_1818321109360324212[14] = -nom_x[14] + true_x[14];
   out_1818321109360324212[15] = -nom_x[15] + true_x[15];
   out_1818321109360324212[16] = -nom_x[16] + true_x[16];
   out_1818321109360324212[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4142439847419469755) {
   out_4142439847419469755[0] = 1.0;
   out_4142439847419469755[1] = 0.0;
   out_4142439847419469755[2] = 0.0;
   out_4142439847419469755[3] = 0.0;
   out_4142439847419469755[4] = 0.0;
   out_4142439847419469755[5] = 0.0;
   out_4142439847419469755[6] = 0.0;
   out_4142439847419469755[7] = 0.0;
   out_4142439847419469755[8] = 0.0;
   out_4142439847419469755[9] = 0.0;
   out_4142439847419469755[10] = 0.0;
   out_4142439847419469755[11] = 0.0;
   out_4142439847419469755[12] = 0.0;
   out_4142439847419469755[13] = 0.0;
   out_4142439847419469755[14] = 0.0;
   out_4142439847419469755[15] = 0.0;
   out_4142439847419469755[16] = 0.0;
   out_4142439847419469755[17] = 0.0;
   out_4142439847419469755[18] = 0.0;
   out_4142439847419469755[19] = 1.0;
   out_4142439847419469755[20] = 0.0;
   out_4142439847419469755[21] = 0.0;
   out_4142439847419469755[22] = 0.0;
   out_4142439847419469755[23] = 0.0;
   out_4142439847419469755[24] = 0.0;
   out_4142439847419469755[25] = 0.0;
   out_4142439847419469755[26] = 0.0;
   out_4142439847419469755[27] = 0.0;
   out_4142439847419469755[28] = 0.0;
   out_4142439847419469755[29] = 0.0;
   out_4142439847419469755[30] = 0.0;
   out_4142439847419469755[31] = 0.0;
   out_4142439847419469755[32] = 0.0;
   out_4142439847419469755[33] = 0.0;
   out_4142439847419469755[34] = 0.0;
   out_4142439847419469755[35] = 0.0;
   out_4142439847419469755[36] = 0.0;
   out_4142439847419469755[37] = 0.0;
   out_4142439847419469755[38] = 1.0;
   out_4142439847419469755[39] = 0.0;
   out_4142439847419469755[40] = 0.0;
   out_4142439847419469755[41] = 0.0;
   out_4142439847419469755[42] = 0.0;
   out_4142439847419469755[43] = 0.0;
   out_4142439847419469755[44] = 0.0;
   out_4142439847419469755[45] = 0.0;
   out_4142439847419469755[46] = 0.0;
   out_4142439847419469755[47] = 0.0;
   out_4142439847419469755[48] = 0.0;
   out_4142439847419469755[49] = 0.0;
   out_4142439847419469755[50] = 0.0;
   out_4142439847419469755[51] = 0.0;
   out_4142439847419469755[52] = 0.0;
   out_4142439847419469755[53] = 0.0;
   out_4142439847419469755[54] = 0.0;
   out_4142439847419469755[55] = 0.0;
   out_4142439847419469755[56] = 0.0;
   out_4142439847419469755[57] = 1.0;
   out_4142439847419469755[58] = 0.0;
   out_4142439847419469755[59] = 0.0;
   out_4142439847419469755[60] = 0.0;
   out_4142439847419469755[61] = 0.0;
   out_4142439847419469755[62] = 0.0;
   out_4142439847419469755[63] = 0.0;
   out_4142439847419469755[64] = 0.0;
   out_4142439847419469755[65] = 0.0;
   out_4142439847419469755[66] = 0.0;
   out_4142439847419469755[67] = 0.0;
   out_4142439847419469755[68] = 0.0;
   out_4142439847419469755[69] = 0.0;
   out_4142439847419469755[70] = 0.0;
   out_4142439847419469755[71] = 0.0;
   out_4142439847419469755[72] = 0.0;
   out_4142439847419469755[73] = 0.0;
   out_4142439847419469755[74] = 0.0;
   out_4142439847419469755[75] = 0.0;
   out_4142439847419469755[76] = 1.0;
   out_4142439847419469755[77] = 0.0;
   out_4142439847419469755[78] = 0.0;
   out_4142439847419469755[79] = 0.0;
   out_4142439847419469755[80] = 0.0;
   out_4142439847419469755[81] = 0.0;
   out_4142439847419469755[82] = 0.0;
   out_4142439847419469755[83] = 0.0;
   out_4142439847419469755[84] = 0.0;
   out_4142439847419469755[85] = 0.0;
   out_4142439847419469755[86] = 0.0;
   out_4142439847419469755[87] = 0.0;
   out_4142439847419469755[88] = 0.0;
   out_4142439847419469755[89] = 0.0;
   out_4142439847419469755[90] = 0.0;
   out_4142439847419469755[91] = 0.0;
   out_4142439847419469755[92] = 0.0;
   out_4142439847419469755[93] = 0.0;
   out_4142439847419469755[94] = 0.0;
   out_4142439847419469755[95] = 1.0;
   out_4142439847419469755[96] = 0.0;
   out_4142439847419469755[97] = 0.0;
   out_4142439847419469755[98] = 0.0;
   out_4142439847419469755[99] = 0.0;
   out_4142439847419469755[100] = 0.0;
   out_4142439847419469755[101] = 0.0;
   out_4142439847419469755[102] = 0.0;
   out_4142439847419469755[103] = 0.0;
   out_4142439847419469755[104] = 0.0;
   out_4142439847419469755[105] = 0.0;
   out_4142439847419469755[106] = 0.0;
   out_4142439847419469755[107] = 0.0;
   out_4142439847419469755[108] = 0.0;
   out_4142439847419469755[109] = 0.0;
   out_4142439847419469755[110] = 0.0;
   out_4142439847419469755[111] = 0.0;
   out_4142439847419469755[112] = 0.0;
   out_4142439847419469755[113] = 0.0;
   out_4142439847419469755[114] = 1.0;
   out_4142439847419469755[115] = 0.0;
   out_4142439847419469755[116] = 0.0;
   out_4142439847419469755[117] = 0.0;
   out_4142439847419469755[118] = 0.0;
   out_4142439847419469755[119] = 0.0;
   out_4142439847419469755[120] = 0.0;
   out_4142439847419469755[121] = 0.0;
   out_4142439847419469755[122] = 0.0;
   out_4142439847419469755[123] = 0.0;
   out_4142439847419469755[124] = 0.0;
   out_4142439847419469755[125] = 0.0;
   out_4142439847419469755[126] = 0.0;
   out_4142439847419469755[127] = 0.0;
   out_4142439847419469755[128] = 0.0;
   out_4142439847419469755[129] = 0.0;
   out_4142439847419469755[130] = 0.0;
   out_4142439847419469755[131] = 0.0;
   out_4142439847419469755[132] = 0.0;
   out_4142439847419469755[133] = 1.0;
   out_4142439847419469755[134] = 0.0;
   out_4142439847419469755[135] = 0.0;
   out_4142439847419469755[136] = 0.0;
   out_4142439847419469755[137] = 0.0;
   out_4142439847419469755[138] = 0.0;
   out_4142439847419469755[139] = 0.0;
   out_4142439847419469755[140] = 0.0;
   out_4142439847419469755[141] = 0.0;
   out_4142439847419469755[142] = 0.0;
   out_4142439847419469755[143] = 0.0;
   out_4142439847419469755[144] = 0.0;
   out_4142439847419469755[145] = 0.0;
   out_4142439847419469755[146] = 0.0;
   out_4142439847419469755[147] = 0.0;
   out_4142439847419469755[148] = 0.0;
   out_4142439847419469755[149] = 0.0;
   out_4142439847419469755[150] = 0.0;
   out_4142439847419469755[151] = 0.0;
   out_4142439847419469755[152] = 1.0;
   out_4142439847419469755[153] = 0.0;
   out_4142439847419469755[154] = 0.0;
   out_4142439847419469755[155] = 0.0;
   out_4142439847419469755[156] = 0.0;
   out_4142439847419469755[157] = 0.0;
   out_4142439847419469755[158] = 0.0;
   out_4142439847419469755[159] = 0.0;
   out_4142439847419469755[160] = 0.0;
   out_4142439847419469755[161] = 0.0;
   out_4142439847419469755[162] = 0.0;
   out_4142439847419469755[163] = 0.0;
   out_4142439847419469755[164] = 0.0;
   out_4142439847419469755[165] = 0.0;
   out_4142439847419469755[166] = 0.0;
   out_4142439847419469755[167] = 0.0;
   out_4142439847419469755[168] = 0.0;
   out_4142439847419469755[169] = 0.0;
   out_4142439847419469755[170] = 0.0;
   out_4142439847419469755[171] = 1.0;
   out_4142439847419469755[172] = 0.0;
   out_4142439847419469755[173] = 0.0;
   out_4142439847419469755[174] = 0.0;
   out_4142439847419469755[175] = 0.0;
   out_4142439847419469755[176] = 0.0;
   out_4142439847419469755[177] = 0.0;
   out_4142439847419469755[178] = 0.0;
   out_4142439847419469755[179] = 0.0;
   out_4142439847419469755[180] = 0.0;
   out_4142439847419469755[181] = 0.0;
   out_4142439847419469755[182] = 0.0;
   out_4142439847419469755[183] = 0.0;
   out_4142439847419469755[184] = 0.0;
   out_4142439847419469755[185] = 0.0;
   out_4142439847419469755[186] = 0.0;
   out_4142439847419469755[187] = 0.0;
   out_4142439847419469755[188] = 0.0;
   out_4142439847419469755[189] = 0.0;
   out_4142439847419469755[190] = 1.0;
   out_4142439847419469755[191] = 0.0;
   out_4142439847419469755[192] = 0.0;
   out_4142439847419469755[193] = 0.0;
   out_4142439847419469755[194] = 0.0;
   out_4142439847419469755[195] = 0.0;
   out_4142439847419469755[196] = 0.0;
   out_4142439847419469755[197] = 0.0;
   out_4142439847419469755[198] = 0.0;
   out_4142439847419469755[199] = 0.0;
   out_4142439847419469755[200] = 0.0;
   out_4142439847419469755[201] = 0.0;
   out_4142439847419469755[202] = 0.0;
   out_4142439847419469755[203] = 0.0;
   out_4142439847419469755[204] = 0.0;
   out_4142439847419469755[205] = 0.0;
   out_4142439847419469755[206] = 0.0;
   out_4142439847419469755[207] = 0.0;
   out_4142439847419469755[208] = 0.0;
   out_4142439847419469755[209] = 1.0;
   out_4142439847419469755[210] = 0.0;
   out_4142439847419469755[211] = 0.0;
   out_4142439847419469755[212] = 0.0;
   out_4142439847419469755[213] = 0.0;
   out_4142439847419469755[214] = 0.0;
   out_4142439847419469755[215] = 0.0;
   out_4142439847419469755[216] = 0.0;
   out_4142439847419469755[217] = 0.0;
   out_4142439847419469755[218] = 0.0;
   out_4142439847419469755[219] = 0.0;
   out_4142439847419469755[220] = 0.0;
   out_4142439847419469755[221] = 0.0;
   out_4142439847419469755[222] = 0.0;
   out_4142439847419469755[223] = 0.0;
   out_4142439847419469755[224] = 0.0;
   out_4142439847419469755[225] = 0.0;
   out_4142439847419469755[226] = 0.0;
   out_4142439847419469755[227] = 0.0;
   out_4142439847419469755[228] = 1.0;
   out_4142439847419469755[229] = 0.0;
   out_4142439847419469755[230] = 0.0;
   out_4142439847419469755[231] = 0.0;
   out_4142439847419469755[232] = 0.0;
   out_4142439847419469755[233] = 0.0;
   out_4142439847419469755[234] = 0.0;
   out_4142439847419469755[235] = 0.0;
   out_4142439847419469755[236] = 0.0;
   out_4142439847419469755[237] = 0.0;
   out_4142439847419469755[238] = 0.0;
   out_4142439847419469755[239] = 0.0;
   out_4142439847419469755[240] = 0.0;
   out_4142439847419469755[241] = 0.0;
   out_4142439847419469755[242] = 0.0;
   out_4142439847419469755[243] = 0.0;
   out_4142439847419469755[244] = 0.0;
   out_4142439847419469755[245] = 0.0;
   out_4142439847419469755[246] = 0.0;
   out_4142439847419469755[247] = 1.0;
   out_4142439847419469755[248] = 0.0;
   out_4142439847419469755[249] = 0.0;
   out_4142439847419469755[250] = 0.0;
   out_4142439847419469755[251] = 0.0;
   out_4142439847419469755[252] = 0.0;
   out_4142439847419469755[253] = 0.0;
   out_4142439847419469755[254] = 0.0;
   out_4142439847419469755[255] = 0.0;
   out_4142439847419469755[256] = 0.0;
   out_4142439847419469755[257] = 0.0;
   out_4142439847419469755[258] = 0.0;
   out_4142439847419469755[259] = 0.0;
   out_4142439847419469755[260] = 0.0;
   out_4142439847419469755[261] = 0.0;
   out_4142439847419469755[262] = 0.0;
   out_4142439847419469755[263] = 0.0;
   out_4142439847419469755[264] = 0.0;
   out_4142439847419469755[265] = 0.0;
   out_4142439847419469755[266] = 1.0;
   out_4142439847419469755[267] = 0.0;
   out_4142439847419469755[268] = 0.0;
   out_4142439847419469755[269] = 0.0;
   out_4142439847419469755[270] = 0.0;
   out_4142439847419469755[271] = 0.0;
   out_4142439847419469755[272] = 0.0;
   out_4142439847419469755[273] = 0.0;
   out_4142439847419469755[274] = 0.0;
   out_4142439847419469755[275] = 0.0;
   out_4142439847419469755[276] = 0.0;
   out_4142439847419469755[277] = 0.0;
   out_4142439847419469755[278] = 0.0;
   out_4142439847419469755[279] = 0.0;
   out_4142439847419469755[280] = 0.0;
   out_4142439847419469755[281] = 0.0;
   out_4142439847419469755[282] = 0.0;
   out_4142439847419469755[283] = 0.0;
   out_4142439847419469755[284] = 0.0;
   out_4142439847419469755[285] = 1.0;
   out_4142439847419469755[286] = 0.0;
   out_4142439847419469755[287] = 0.0;
   out_4142439847419469755[288] = 0.0;
   out_4142439847419469755[289] = 0.0;
   out_4142439847419469755[290] = 0.0;
   out_4142439847419469755[291] = 0.0;
   out_4142439847419469755[292] = 0.0;
   out_4142439847419469755[293] = 0.0;
   out_4142439847419469755[294] = 0.0;
   out_4142439847419469755[295] = 0.0;
   out_4142439847419469755[296] = 0.0;
   out_4142439847419469755[297] = 0.0;
   out_4142439847419469755[298] = 0.0;
   out_4142439847419469755[299] = 0.0;
   out_4142439847419469755[300] = 0.0;
   out_4142439847419469755[301] = 0.0;
   out_4142439847419469755[302] = 0.0;
   out_4142439847419469755[303] = 0.0;
   out_4142439847419469755[304] = 1.0;
   out_4142439847419469755[305] = 0.0;
   out_4142439847419469755[306] = 0.0;
   out_4142439847419469755[307] = 0.0;
   out_4142439847419469755[308] = 0.0;
   out_4142439847419469755[309] = 0.0;
   out_4142439847419469755[310] = 0.0;
   out_4142439847419469755[311] = 0.0;
   out_4142439847419469755[312] = 0.0;
   out_4142439847419469755[313] = 0.0;
   out_4142439847419469755[314] = 0.0;
   out_4142439847419469755[315] = 0.0;
   out_4142439847419469755[316] = 0.0;
   out_4142439847419469755[317] = 0.0;
   out_4142439847419469755[318] = 0.0;
   out_4142439847419469755[319] = 0.0;
   out_4142439847419469755[320] = 0.0;
   out_4142439847419469755[321] = 0.0;
   out_4142439847419469755[322] = 0.0;
   out_4142439847419469755[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_274046506931821425) {
   out_274046506931821425[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_274046506931821425[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_274046506931821425[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_274046506931821425[3] = dt*state[12] + state[3];
   out_274046506931821425[4] = dt*state[13] + state[4];
   out_274046506931821425[5] = dt*state[14] + state[5];
   out_274046506931821425[6] = state[6];
   out_274046506931821425[7] = state[7];
   out_274046506931821425[8] = state[8];
   out_274046506931821425[9] = state[9];
   out_274046506931821425[10] = state[10];
   out_274046506931821425[11] = state[11];
   out_274046506931821425[12] = state[12];
   out_274046506931821425[13] = state[13];
   out_274046506931821425[14] = state[14];
   out_274046506931821425[15] = state[15];
   out_274046506931821425[16] = state[16];
   out_274046506931821425[17] = state[17];
}
void F_fun(double *state, double dt, double *out_5107362200909236418) {
   out_5107362200909236418[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5107362200909236418[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5107362200909236418[2] = 0;
   out_5107362200909236418[3] = 0;
   out_5107362200909236418[4] = 0;
   out_5107362200909236418[5] = 0;
   out_5107362200909236418[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5107362200909236418[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5107362200909236418[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_5107362200909236418[9] = 0;
   out_5107362200909236418[10] = 0;
   out_5107362200909236418[11] = 0;
   out_5107362200909236418[12] = 0;
   out_5107362200909236418[13] = 0;
   out_5107362200909236418[14] = 0;
   out_5107362200909236418[15] = 0;
   out_5107362200909236418[16] = 0;
   out_5107362200909236418[17] = 0;
   out_5107362200909236418[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5107362200909236418[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5107362200909236418[20] = 0;
   out_5107362200909236418[21] = 0;
   out_5107362200909236418[22] = 0;
   out_5107362200909236418[23] = 0;
   out_5107362200909236418[24] = 0;
   out_5107362200909236418[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5107362200909236418[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_5107362200909236418[27] = 0;
   out_5107362200909236418[28] = 0;
   out_5107362200909236418[29] = 0;
   out_5107362200909236418[30] = 0;
   out_5107362200909236418[31] = 0;
   out_5107362200909236418[32] = 0;
   out_5107362200909236418[33] = 0;
   out_5107362200909236418[34] = 0;
   out_5107362200909236418[35] = 0;
   out_5107362200909236418[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5107362200909236418[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5107362200909236418[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5107362200909236418[39] = 0;
   out_5107362200909236418[40] = 0;
   out_5107362200909236418[41] = 0;
   out_5107362200909236418[42] = 0;
   out_5107362200909236418[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5107362200909236418[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_5107362200909236418[45] = 0;
   out_5107362200909236418[46] = 0;
   out_5107362200909236418[47] = 0;
   out_5107362200909236418[48] = 0;
   out_5107362200909236418[49] = 0;
   out_5107362200909236418[50] = 0;
   out_5107362200909236418[51] = 0;
   out_5107362200909236418[52] = 0;
   out_5107362200909236418[53] = 0;
   out_5107362200909236418[54] = 0;
   out_5107362200909236418[55] = 0;
   out_5107362200909236418[56] = 0;
   out_5107362200909236418[57] = 1;
   out_5107362200909236418[58] = 0;
   out_5107362200909236418[59] = 0;
   out_5107362200909236418[60] = 0;
   out_5107362200909236418[61] = 0;
   out_5107362200909236418[62] = 0;
   out_5107362200909236418[63] = 0;
   out_5107362200909236418[64] = 0;
   out_5107362200909236418[65] = 0;
   out_5107362200909236418[66] = dt;
   out_5107362200909236418[67] = 0;
   out_5107362200909236418[68] = 0;
   out_5107362200909236418[69] = 0;
   out_5107362200909236418[70] = 0;
   out_5107362200909236418[71] = 0;
   out_5107362200909236418[72] = 0;
   out_5107362200909236418[73] = 0;
   out_5107362200909236418[74] = 0;
   out_5107362200909236418[75] = 0;
   out_5107362200909236418[76] = 1;
   out_5107362200909236418[77] = 0;
   out_5107362200909236418[78] = 0;
   out_5107362200909236418[79] = 0;
   out_5107362200909236418[80] = 0;
   out_5107362200909236418[81] = 0;
   out_5107362200909236418[82] = 0;
   out_5107362200909236418[83] = 0;
   out_5107362200909236418[84] = 0;
   out_5107362200909236418[85] = dt;
   out_5107362200909236418[86] = 0;
   out_5107362200909236418[87] = 0;
   out_5107362200909236418[88] = 0;
   out_5107362200909236418[89] = 0;
   out_5107362200909236418[90] = 0;
   out_5107362200909236418[91] = 0;
   out_5107362200909236418[92] = 0;
   out_5107362200909236418[93] = 0;
   out_5107362200909236418[94] = 0;
   out_5107362200909236418[95] = 1;
   out_5107362200909236418[96] = 0;
   out_5107362200909236418[97] = 0;
   out_5107362200909236418[98] = 0;
   out_5107362200909236418[99] = 0;
   out_5107362200909236418[100] = 0;
   out_5107362200909236418[101] = 0;
   out_5107362200909236418[102] = 0;
   out_5107362200909236418[103] = 0;
   out_5107362200909236418[104] = dt;
   out_5107362200909236418[105] = 0;
   out_5107362200909236418[106] = 0;
   out_5107362200909236418[107] = 0;
   out_5107362200909236418[108] = 0;
   out_5107362200909236418[109] = 0;
   out_5107362200909236418[110] = 0;
   out_5107362200909236418[111] = 0;
   out_5107362200909236418[112] = 0;
   out_5107362200909236418[113] = 0;
   out_5107362200909236418[114] = 1;
   out_5107362200909236418[115] = 0;
   out_5107362200909236418[116] = 0;
   out_5107362200909236418[117] = 0;
   out_5107362200909236418[118] = 0;
   out_5107362200909236418[119] = 0;
   out_5107362200909236418[120] = 0;
   out_5107362200909236418[121] = 0;
   out_5107362200909236418[122] = 0;
   out_5107362200909236418[123] = 0;
   out_5107362200909236418[124] = 0;
   out_5107362200909236418[125] = 0;
   out_5107362200909236418[126] = 0;
   out_5107362200909236418[127] = 0;
   out_5107362200909236418[128] = 0;
   out_5107362200909236418[129] = 0;
   out_5107362200909236418[130] = 0;
   out_5107362200909236418[131] = 0;
   out_5107362200909236418[132] = 0;
   out_5107362200909236418[133] = 1;
   out_5107362200909236418[134] = 0;
   out_5107362200909236418[135] = 0;
   out_5107362200909236418[136] = 0;
   out_5107362200909236418[137] = 0;
   out_5107362200909236418[138] = 0;
   out_5107362200909236418[139] = 0;
   out_5107362200909236418[140] = 0;
   out_5107362200909236418[141] = 0;
   out_5107362200909236418[142] = 0;
   out_5107362200909236418[143] = 0;
   out_5107362200909236418[144] = 0;
   out_5107362200909236418[145] = 0;
   out_5107362200909236418[146] = 0;
   out_5107362200909236418[147] = 0;
   out_5107362200909236418[148] = 0;
   out_5107362200909236418[149] = 0;
   out_5107362200909236418[150] = 0;
   out_5107362200909236418[151] = 0;
   out_5107362200909236418[152] = 1;
   out_5107362200909236418[153] = 0;
   out_5107362200909236418[154] = 0;
   out_5107362200909236418[155] = 0;
   out_5107362200909236418[156] = 0;
   out_5107362200909236418[157] = 0;
   out_5107362200909236418[158] = 0;
   out_5107362200909236418[159] = 0;
   out_5107362200909236418[160] = 0;
   out_5107362200909236418[161] = 0;
   out_5107362200909236418[162] = 0;
   out_5107362200909236418[163] = 0;
   out_5107362200909236418[164] = 0;
   out_5107362200909236418[165] = 0;
   out_5107362200909236418[166] = 0;
   out_5107362200909236418[167] = 0;
   out_5107362200909236418[168] = 0;
   out_5107362200909236418[169] = 0;
   out_5107362200909236418[170] = 0;
   out_5107362200909236418[171] = 1;
   out_5107362200909236418[172] = 0;
   out_5107362200909236418[173] = 0;
   out_5107362200909236418[174] = 0;
   out_5107362200909236418[175] = 0;
   out_5107362200909236418[176] = 0;
   out_5107362200909236418[177] = 0;
   out_5107362200909236418[178] = 0;
   out_5107362200909236418[179] = 0;
   out_5107362200909236418[180] = 0;
   out_5107362200909236418[181] = 0;
   out_5107362200909236418[182] = 0;
   out_5107362200909236418[183] = 0;
   out_5107362200909236418[184] = 0;
   out_5107362200909236418[185] = 0;
   out_5107362200909236418[186] = 0;
   out_5107362200909236418[187] = 0;
   out_5107362200909236418[188] = 0;
   out_5107362200909236418[189] = 0;
   out_5107362200909236418[190] = 1;
   out_5107362200909236418[191] = 0;
   out_5107362200909236418[192] = 0;
   out_5107362200909236418[193] = 0;
   out_5107362200909236418[194] = 0;
   out_5107362200909236418[195] = 0;
   out_5107362200909236418[196] = 0;
   out_5107362200909236418[197] = 0;
   out_5107362200909236418[198] = 0;
   out_5107362200909236418[199] = 0;
   out_5107362200909236418[200] = 0;
   out_5107362200909236418[201] = 0;
   out_5107362200909236418[202] = 0;
   out_5107362200909236418[203] = 0;
   out_5107362200909236418[204] = 0;
   out_5107362200909236418[205] = 0;
   out_5107362200909236418[206] = 0;
   out_5107362200909236418[207] = 0;
   out_5107362200909236418[208] = 0;
   out_5107362200909236418[209] = 1;
   out_5107362200909236418[210] = 0;
   out_5107362200909236418[211] = 0;
   out_5107362200909236418[212] = 0;
   out_5107362200909236418[213] = 0;
   out_5107362200909236418[214] = 0;
   out_5107362200909236418[215] = 0;
   out_5107362200909236418[216] = 0;
   out_5107362200909236418[217] = 0;
   out_5107362200909236418[218] = 0;
   out_5107362200909236418[219] = 0;
   out_5107362200909236418[220] = 0;
   out_5107362200909236418[221] = 0;
   out_5107362200909236418[222] = 0;
   out_5107362200909236418[223] = 0;
   out_5107362200909236418[224] = 0;
   out_5107362200909236418[225] = 0;
   out_5107362200909236418[226] = 0;
   out_5107362200909236418[227] = 0;
   out_5107362200909236418[228] = 1;
   out_5107362200909236418[229] = 0;
   out_5107362200909236418[230] = 0;
   out_5107362200909236418[231] = 0;
   out_5107362200909236418[232] = 0;
   out_5107362200909236418[233] = 0;
   out_5107362200909236418[234] = 0;
   out_5107362200909236418[235] = 0;
   out_5107362200909236418[236] = 0;
   out_5107362200909236418[237] = 0;
   out_5107362200909236418[238] = 0;
   out_5107362200909236418[239] = 0;
   out_5107362200909236418[240] = 0;
   out_5107362200909236418[241] = 0;
   out_5107362200909236418[242] = 0;
   out_5107362200909236418[243] = 0;
   out_5107362200909236418[244] = 0;
   out_5107362200909236418[245] = 0;
   out_5107362200909236418[246] = 0;
   out_5107362200909236418[247] = 1;
   out_5107362200909236418[248] = 0;
   out_5107362200909236418[249] = 0;
   out_5107362200909236418[250] = 0;
   out_5107362200909236418[251] = 0;
   out_5107362200909236418[252] = 0;
   out_5107362200909236418[253] = 0;
   out_5107362200909236418[254] = 0;
   out_5107362200909236418[255] = 0;
   out_5107362200909236418[256] = 0;
   out_5107362200909236418[257] = 0;
   out_5107362200909236418[258] = 0;
   out_5107362200909236418[259] = 0;
   out_5107362200909236418[260] = 0;
   out_5107362200909236418[261] = 0;
   out_5107362200909236418[262] = 0;
   out_5107362200909236418[263] = 0;
   out_5107362200909236418[264] = 0;
   out_5107362200909236418[265] = 0;
   out_5107362200909236418[266] = 1;
   out_5107362200909236418[267] = 0;
   out_5107362200909236418[268] = 0;
   out_5107362200909236418[269] = 0;
   out_5107362200909236418[270] = 0;
   out_5107362200909236418[271] = 0;
   out_5107362200909236418[272] = 0;
   out_5107362200909236418[273] = 0;
   out_5107362200909236418[274] = 0;
   out_5107362200909236418[275] = 0;
   out_5107362200909236418[276] = 0;
   out_5107362200909236418[277] = 0;
   out_5107362200909236418[278] = 0;
   out_5107362200909236418[279] = 0;
   out_5107362200909236418[280] = 0;
   out_5107362200909236418[281] = 0;
   out_5107362200909236418[282] = 0;
   out_5107362200909236418[283] = 0;
   out_5107362200909236418[284] = 0;
   out_5107362200909236418[285] = 1;
   out_5107362200909236418[286] = 0;
   out_5107362200909236418[287] = 0;
   out_5107362200909236418[288] = 0;
   out_5107362200909236418[289] = 0;
   out_5107362200909236418[290] = 0;
   out_5107362200909236418[291] = 0;
   out_5107362200909236418[292] = 0;
   out_5107362200909236418[293] = 0;
   out_5107362200909236418[294] = 0;
   out_5107362200909236418[295] = 0;
   out_5107362200909236418[296] = 0;
   out_5107362200909236418[297] = 0;
   out_5107362200909236418[298] = 0;
   out_5107362200909236418[299] = 0;
   out_5107362200909236418[300] = 0;
   out_5107362200909236418[301] = 0;
   out_5107362200909236418[302] = 0;
   out_5107362200909236418[303] = 0;
   out_5107362200909236418[304] = 1;
   out_5107362200909236418[305] = 0;
   out_5107362200909236418[306] = 0;
   out_5107362200909236418[307] = 0;
   out_5107362200909236418[308] = 0;
   out_5107362200909236418[309] = 0;
   out_5107362200909236418[310] = 0;
   out_5107362200909236418[311] = 0;
   out_5107362200909236418[312] = 0;
   out_5107362200909236418[313] = 0;
   out_5107362200909236418[314] = 0;
   out_5107362200909236418[315] = 0;
   out_5107362200909236418[316] = 0;
   out_5107362200909236418[317] = 0;
   out_5107362200909236418[318] = 0;
   out_5107362200909236418[319] = 0;
   out_5107362200909236418[320] = 0;
   out_5107362200909236418[321] = 0;
   out_5107362200909236418[322] = 0;
   out_5107362200909236418[323] = 1;
}
void h_4(double *state, double *unused, double *out_2986239765915263589) {
   out_2986239765915263589[0] = state[6] + state[9];
   out_2986239765915263589[1] = state[7] + state[10];
   out_2986239765915263589[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_7914247057263377327) {
   out_7914247057263377327[0] = 0;
   out_7914247057263377327[1] = 0;
   out_7914247057263377327[2] = 0;
   out_7914247057263377327[3] = 0;
   out_7914247057263377327[4] = 0;
   out_7914247057263377327[5] = 0;
   out_7914247057263377327[6] = 1;
   out_7914247057263377327[7] = 0;
   out_7914247057263377327[8] = 0;
   out_7914247057263377327[9] = 1;
   out_7914247057263377327[10] = 0;
   out_7914247057263377327[11] = 0;
   out_7914247057263377327[12] = 0;
   out_7914247057263377327[13] = 0;
   out_7914247057263377327[14] = 0;
   out_7914247057263377327[15] = 0;
   out_7914247057263377327[16] = 0;
   out_7914247057263377327[17] = 0;
   out_7914247057263377327[18] = 0;
   out_7914247057263377327[19] = 0;
   out_7914247057263377327[20] = 0;
   out_7914247057263377327[21] = 0;
   out_7914247057263377327[22] = 0;
   out_7914247057263377327[23] = 0;
   out_7914247057263377327[24] = 0;
   out_7914247057263377327[25] = 1;
   out_7914247057263377327[26] = 0;
   out_7914247057263377327[27] = 0;
   out_7914247057263377327[28] = 1;
   out_7914247057263377327[29] = 0;
   out_7914247057263377327[30] = 0;
   out_7914247057263377327[31] = 0;
   out_7914247057263377327[32] = 0;
   out_7914247057263377327[33] = 0;
   out_7914247057263377327[34] = 0;
   out_7914247057263377327[35] = 0;
   out_7914247057263377327[36] = 0;
   out_7914247057263377327[37] = 0;
   out_7914247057263377327[38] = 0;
   out_7914247057263377327[39] = 0;
   out_7914247057263377327[40] = 0;
   out_7914247057263377327[41] = 0;
   out_7914247057263377327[42] = 0;
   out_7914247057263377327[43] = 0;
   out_7914247057263377327[44] = 1;
   out_7914247057263377327[45] = 0;
   out_7914247057263377327[46] = 0;
   out_7914247057263377327[47] = 1;
   out_7914247057263377327[48] = 0;
   out_7914247057263377327[49] = 0;
   out_7914247057263377327[50] = 0;
   out_7914247057263377327[51] = 0;
   out_7914247057263377327[52] = 0;
   out_7914247057263377327[53] = 0;
}
void h_10(double *state, double *unused, double *out_7953871030082380114) {
   out_7953871030082380114[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_7953871030082380114[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_7953871030082380114[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_4746611735982339664) {
   out_4746611735982339664[0] = 0;
   out_4746611735982339664[1] = 9.8100000000000005*cos(state[1]);
   out_4746611735982339664[2] = 0;
   out_4746611735982339664[3] = 0;
   out_4746611735982339664[4] = -state[8];
   out_4746611735982339664[5] = state[7];
   out_4746611735982339664[6] = 0;
   out_4746611735982339664[7] = state[5];
   out_4746611735982339664[8] = -state[4];
   out_4746611735982339664[9] = 0;
   out_4746611735982339664[10] = 0;
   out_4746611735982339664[11] = 0;
   out_4746611735982339664[12] = 1;
   out_4746611735982339664[13] = 0;
   out_4746611735982339664[14] = 0;
   out_4746611735982339664[15] = 1;
   out_4746611735982339664[16] = 0;
   out_4746611735982339664[17] = 0;
   out_4746611735982339664[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_4746611735982339664[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_4746611735982339664[20] = 0;
   out_4746611735982339664[21] = state[8];
   out_4746611735982339664[22] = 0;
   out_4746611735982339664[23] = -state[6];
   out_4746611735982339664[24] = -state[5];
   out_4746611735982339664[25] = 0;
   out_4746611735982339664[26] = state[3];
   out_4746611735982339664[27] = 0;
   out_4746611735982339664[28] = 0;
   out_4746611735982339664[29] = 0;
   out_4746611735982339664[30] = 0;
   out_4746611735982339664[31] = 1;
   out_4746611735982339664[32] = 0;
   out_4746611735982339664[33] = 0;
   out_4746611735982339664[34] = 1;
   out_4746611735982339664[35] = 0;
   out_4746611735982339664[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_4746611735982339664[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_4746611735982339664[38] = 0;
   out_4746611735982339664[39] = -state[7];
   out_4746611735982339664[40] = state[6];
   out_4746611735982339664[41] = 0;
   out_4746611735982339664[42] = state[4];
   out_4746611735982339664[43] = -state[3];
   out_4746611735982339664[44] = 0;
   out_4746611735982339664[45] = 0;
   out_4746611735982339664[46] = 0;
   out_4746611735982339664[47] = 0;
   out_4746611735982339664[48] = 0;
   out_4746611735982339664[49] = 0;
   out_4746611735982339664[50] = 1;
   out_4746611735982339664[51] = 0;
   out_4746611735982339664[52] = 0;
   out_4746611735982339664[53] = 1;
}
void h_13(double *state, double *unused, double *out_6031258319996835090) {
   out_6031258319996835090[0] = state[3];
   out_6031258319996835090[1] = state[4];
   out_6031258319996835090[2] = state[5];
}
void H_13(double *state, double *unused, double *out_7320223191113841488) {
   out_7320223191113841488[0] = 0;
   out_7320223191113841488[1] = 0;
   out_7320223191113841488[2] = 0;
   out_7320223191113841488[3] = 1;
   out_7320223191113841488[4] = 0;
   out_7320223191113841488[5] = 0;
   out_7320223191113841488[6] = 0;
   out_7320223191113841488[7] = 0;
   out_7320223191113841488[8] = 0;
   out_7320223191113841488[9] = 0;
   out_7320223191113841488[10] = 0;
   out_7320223191113841488[11] = 0;
   out_7320223191113841488[12] = 0;
   out_7320223191113841488[13] = 0;
   out_7320223191113841488[14] = 0;
   out_7320223191113841488[15] = 0;
   out_7320223191113841488[16] = 0;
   out_7320223191113841488[17] = 0;
   out_7320223191113841488[18] = 0;
   out_7320223191113841488[19] = 0;
   out_7320223191113841488[20] = 0;
   out_7320223191113841488[21] = 0;
   out_7320223191113841488[22] = 1;
   out_7320223191113841488[23] = 0;
   out_7320223191113841488[24] = 0;
   out_7320223191113841488[25] = 0;
   out_7320223191113841488[26] = 0;
   out_7320223191113841488[27] = 0;
   out_7320223191113841488[28] = 0;
   out_7320223191113841488[29] = 0;
   out_7320223191113841488[30] = 0;
   out_7320223191113841488[31] = 0;
   out_7320223191113841488[32] = 0;
   out_7320223191113841488[33] = 0;
   out_7320223191113841488[34] = 0;
   out_7320223191113841488[35] = 0;
   out_7320223191113841488[36] = 0;
   out_7320223191113841488[37] = 0;
   out_7320223191113841488[38] = 0;
   out_7320223191113841488[39] = 0;
   out_7320223191113841488[40] = 0;
   out_7320223191113841488[41] = 1;
   out_7320223191113841488[42] = 0;
   out_7320223191113841488[43] = 0;
   out_7320223191113841488[44] = 0;
   out_7320223191113841488[45] = 0;
   out_7320223191113841488[46] = 0;
   out_7320223191113841488[47] = 0;
   out_7320223191113841488[48] = 0;
   out_7320223191113841488[49] = 0;
   out_7320223191113841488[50] = 0;
   out_7320223191113841488[51] = 0;
   out_7320223191113841488[52] = 0;
   out_7320223191113841488[53] = 0;
}
void h_14(double *state, double *unused, double *out_9013923068017280655) {
   out_9013923068017280655[0] = state[6];
   out_9013923068017280655[1] = state[7];
   out_9013923068017280655[2] = state[8];
}
void H_14(double *state, double *unused, double *out_6569256160106689760) {
   out_6569256160106689760[0] = 0;
   out_6569256160106689760[1] = 0;
   out_6569256160106689760[2] = 0;
   out_6569256160106689760[3] = 0;
   out_6569256160106689760[4] = 0;
   out_6569256160106689760[5] = 0;
   out_6569256160106689760[6] = 1;
   out_6569256160106689760[7] = 0;
   out_6569256160106689760[8] = 0;
   out_6569256160106689760[9] = 0;
   out_6569256160106689760[10] = 0;
   out_6569256160106689760[11] = 0;
   out_6569256160106689760[12] = 0;
   out_6569256160106689760[13] = 0;
   out_6569256160106689760[14] = 0;
   out_6569256160106689760[15] = 0;
   out_6569256160106689760[16] = 0;
   out_6569256160106689760[17] = 0;
   out_6569256160106689760[18] = 0;
   out_6569256160106689760[19] = 0;
   out_6569256160106689760[20] = 0;
   out_6569256160106689760[21] = 0;
   out_6569256160106689760[22] = 0;
   out_6569256160106689760[23] = 0;
   out_6569256160106689760[24] = 0;
   out_6569256160106689760[25] = 1;
   out_6569256160106689760[26] = 0;
   out_6569256160106689760[27] = 0;
   out_6569256160106689760[28] = 0;
   out_6569256160106689760[29] = 0;
   out_6569256160106689760[30] = 0;
   out_6569256160106689760[31] = 0;
   out_6569256160106689760[32] = 0;
   out_6569256160106689760[33] = 0;
   out_6569256160106689760[34] = 0;
   out_6569256160106689760[35] = 0;
   out_6569256160106689760[36] = 0;
   out_6569256160106689760[37] = 0;
   out_6569256160106689760[38] = 0;
   out_6569256160106689760[39] = 0;
   out_6569256160106689760[40] = 0;
   out_6569256160106689760[41] = 0;
   out_6569256160106689760[42] = 0;
   out_6569256160106689760[43] = 0;
   out_6569256160106689760[44] = 1;
   out_6569256160106689760[45] = 0;
   out_6569256160106689760[46] = 0;
   out_6569256160106689760[47] = 0;
   out_6569256160106689760[48] = 0;
   out_6569256160106689760[49] = 0;
   out_6569256160106689760[50] = 0;
   out_6569256160106689760[51] = 0;
   out_6569256160106689760[52] = 0;
   out_6569256160106689760[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_1794537510029647999) {
  err_fun(nom_x, delta_x, out_1794537510029647999);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_1818321109360324212) {
  inv_err_fun(nom_x, true_x, out_1818321109360324212);
}
void pose_H_mod_fun(double *state, double *out_4142439847419469755) {
  H_mod_fun(state, out_4142439847419469755);
}
void pose_f_fun(double *state, double dt, double *out_274046506931821425) {
  f_fun(state,  dt, out_274046506931821425);
}
void pose_F_fun(double *state, double dt, double *out_5107362200909236418) {
  F_fun(state,  dt, out_5107362200909236418);
}
void pose_h_4(double *state, double *unused, double *out_2986239765915263589) {
  h_4(state, unused, out_2986239765915263589);
}
void pose_H_4(double *state, double *unused, double *out_7914247057263377327) {
  H_4(state, unused, out_7914247057263377327);
}
void pose_h_10(double *state, double *unused, double *out_7953871030082380114) {
  h_10(state, unused, out_7953871030082380114);
}
void pose_H_10(double *state, double *unused, double *out_4746611735982339664) {
  H_10(state, unused, out_4746611735982339664);
}
void pose_h_13(double *state, double *unused, double *out_6031258319996835090) {
  h_13(state, unused, out_6031258319996835090);
}
void pose_H_13(double *state, double *unused, double *out_7320223191113841488) {
  H_13(state, unused, out_7320223191113841488);
}
void pose_h_14(double *state, double *unused, double *out_9013923068017280655) {
  h_14(state, unused, out_9013923068017280655);
}
void pose_H_14(double *state, double *unused, double *out_6569256160106689760) {
  H_14(state, unused, out_6569256160106689760);
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
