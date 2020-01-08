/* Generated by: /Users/sfegan/GitHub/fftw3/genfft/gen_r2cb.native -n 40 -sign 1 -standalone -fma -generic-arith -compact -name dft_codelet_r2cb_40 */

/*
 * This function contains 220 FP additions, 134 FP multiplications,
 * (or, 86 additions, 0 multiplications, 134 fused multiply/add),
 * 74 stack variables, 9 constants, and 80 memory accesses
 */
void dft_codelet_r2cb_40(R * R0, R * R1, R * Cr, R * Ci, stride rs, stride csr, stride csi, INT v, INT ivs, INT ovs)
{
  DK(KP1_902113032, +1.902113032590307144232878666758764286811397268);
  DK(KP951056516, +0.951056516295153572116439333379382143405698634);
  DK(KP1_414213562, +1.414213562373095048801688724209698078569671875);
  DK(KP559016994, +0.559016994374947424102293417182819058860154590);
  DK(KP250000000, +0.250000000000000000000000000000000000000000000);
  DK(KP1_118033988, +1.118033988749894848204586834365638117720309180);
  DK(KP500000000, +0.500000000000000000000000000000000000000000000);
  DK(KP2_000000000, +2.000000000000000000000000000000000000000000000);
  DK(KP618033988, +0.618033988749894848204586834365638117720309180);
  {
    INT i;
    for (i = v; i > 0; i = i - 1, R0 = R0 + ovs, R1 = R1 + ovs, Cr = Cr + ivs, Ci = Ci + ivs, MAKE_VOLATILE_STRIDE(160, rs), MAKE_VOLATILE_STRIDE(160, csr), MAKE_VOLATILE_STRIDE(160, csi)) {
      E TK, T1C, T5, TH, T1B, TR, T1F, Ta, TO, T1E, Tc, TV, T2V, T2b,
       Tj;
      E TU, T22, T13, T27, T1J, T25, T26, T1q, T3r, T1Q, T2w, T32, TD,
       T33, T2z;
      E T1j, T1P, Tu, T30, T2o, T18, T1M, T1f, T3q, T1N, T2l, T2Z;
      {
        E TI, TJ, TP, TQ;
        TI = Ci[WS(csi, 8)];
        TJ = Ci[WS(csi, 16)];
        TK = FMA(KP618033988, TJ, TI);
        T1C = FMS(KP618033988, TI, TJ);
        {
          E T1, T4, TG, T2, T3, TF;
          T1 = Cr[0];
          T2 = Cr[WS(csr, 8)];
          T3 = Cr[WS(csr, 16)];
          T4 = ADD(T2, T3);
          TG = SUB(T2, T3);
          T5 = FMA(KP2_000000000, T4, T1);
          TF = FNMS(KP500000000, T4, T1);
          TH = FMA(KP1_118033988, TG, TF);
          T1B = FNMS(KP1_118033988, TG, TF);
        }
        TP = Ci[WS(csi, 12)];
        TQ = Ci[WS(csi, 4)];
        TR = FMA(KP618033988, TQ, TP);
        T1F = FNMS(KP618033988, TP, TQ);
        {
          E T6, T9, TN, T7, T8, TM;
          T6 = Cr[WS(csr, 20)];
          T7 = Cr[WS(csr, 12)];
          T8 = Cr[WS(csr, 4)];
          T9 = ADD(T7, T8);
          TN = SUB(T7, T8);
          Ta = FMA(KP2_000000000, T9, T6);
          TM = FNMS(KP500000000, T9, T6);
          TO = FMA(KP1_118033988, TN, TM);
          T1E = FNMS(KP1_118033988, TN, TM);
        }
        {
          E Ti, T2a, Tf, T29;
          Tc = Cr[WS(csr, 10)];
          {
            E Tg, Th, Td, Te;
            Tg = Cr[WS(csr, 14)];
            Th = Cr[WS(csr, 6)];
            Ti = ADD(Tg, Th);
            T2a = SUB(Tg, Th);
            Td = Cr[WS(csr, 18)];
            Te = Cr[WS(csr, 2)];
            Tf = ADD(Td, Te);
            T29 = SUB(Td, Te);
          }
          TV = SUB(Tf, Ti);
          T2V = FNMS(KP618033988, T29, T2a);
          T2b = FMA(KP618033988, T2a, T29);
          Tj = ADD(Tf, Ti);
          TU = FNMS(KP250000000, Tj, Tc);
        }
      }
      {
        E T12, T24, TZ, T23;
        T22 = Ci[WS(csi, 10)];
        {
          E T10, T11, TX, TY;
          T10 = Ci[WS(csi, 6)];
          T11 = Ci[WS(csi, 14)];
          T12 = SUB(T10, T11);
          T24 = ADD(T11, T10);
          TX = Ci[WS(csi, 18)];
          TY = Ci[WS(csi, 2)];
          TZ = SUB(TX, TY);
          T23 = ADD(TX, TY);
        }
        T13 = FMA(KP618033988, T12, TZ);
        T27 = ADD(T23, T24);
        T1J = FNMS(KP618033988, TZ, T12);
        T25 = SUB(T23, T24);
        T26 = FNMS(KP250000000, T25, T22);
      }
      {
        E T2q, T2r, T2s, T1m, T2t, T1p, T2u, T2v;
        T2q = Ci[WS(csi, 15)];
        {
          E T1k, T1l, T1n, T1o;
          T1k = Ci[WS(csi, 7)];
          T1l = Ci[WS(csi, 17)];
          T2r = SUB(T1l, T1k);
          T1n = Ci[WS(csi, 1)];
          T1o = Ci[WS(csi, 9)];
          T2s = ADD(T1n, T1o);
          T1m = ADD(T1k, T1l);
          T2t = ADD(T2r, T2s);
          T1p = SUB(T1n, T1o);
        }
        T1q = FNMS(KP618033988, T1p, T1m);
        T3r = SUB(T2t, T2q);
        T1Q = FMA(KP618033988, T1m, T1p);
        T2u = FMA(KP250000000, T2t, T2q);
        T2v = SUB(T2r, T2s);
        T2w = FNMS(KP559016994, T2v, T2u);
        T32 = FMA(KP559016994, T2v, T2u);
      }
      {
        E Tv, Ty, TB, TC, T2y, T2x, T1h, T1i;
        Tv = Cr[WS(csr, 15)];
        {
          E Tw, Tx, Tz, TA;
          Tw = Cr[WS(csr, 7)];
          Tx = Cr[WS(csr, 17)];
          Ty = ADD(Tw, Tx);
          Tz = Cr[WS(csr, 1)];
          TA = Cr[WS(csr, 9)];
          TB = ADD(Tz, TA);
          TC = ADD(Ty, TB);
          T2y = SUB(Tz, TA);
          T2x = SUB(Tw, Tx);
        }
        TD = ADD(Tv, TC);
        T33 = FNMS(KP618033988, T2x, T2y);
        T2z = FMA(KP618033988, T2y, T2x);
        T1h = FNMS(KP250000000, TC, Tv);
        T1i = SUB(Ty, TB);
        T1j = FMA(KP559016994, T1i, T1h);
        T1P = FNMS(KP559016994, T1i, T1h);
      }
      {
        E Tm, Tp, Ts, Tt, T2n, T2m, T16, T17;
        Tm = Cr[WS(csr, 5)];
        {
          E Tn, To, Tq, Tr;
          Tn = Cr[WS(csr, 13)];
          To = Cr[WS(csr, 3)];
          Tp = ADD(Tn, To);
          Tq = Cr[WS(csr, 19)];
          Tr = Cr[WS(csr, 11)];
          Ts = ADD(Tq, Tr);
          Tt = ADD(Tp, Ts);
          T2n = SUB(Tq, Tr);
          T2m = SUB(Tn, To);
        }
        Tu = ADD(Tm, Tt);
        T30 = FNMS(KP618033988, T2m, T2n);
        T2o = FMA(KP618033988, T2n, T2m);
        T16 = FNMS(KP250000000, Tt, Tm);
        T17 = SUB(Tp, Ts);
        T18 = FMA(KP559016994, T17, T16);
        T1M = FNMS(KP559016994, T17, T16);
      }
      {
        E T2f, T2g, T2h, T1b, T2i, T1e, T2j, T2k;
        T2f = Ci[WS(csi, 5)];
        {
          E T19, T1a, T1c, T1d;
          T19 = Ci[WS(csi, 13)];
          T1a = Ci[WS(csi, 3)];
          T2g = SUB(T19, T1a);
          T1c = Ci[WS(csi, 11)];
          T1d = Ci[WS(csi, 19)];
          T2h = ADD(T1c, T1d);
          T1b = ADD(T19, T1a);
          T2i = SUB(T2g, T2h);
          T1e = SUB(T1c, T1d);
        }
        T1f = FMA(KP618033988, T1e, T1b);
        T3q = ADD(T2i, T2f);
        T1N = FNMS(KP618033988, T1b, T1e);
        T2j = FNMS(KP250000000, T2i, T2f);
        T2k = ADD(T2g, T2h);
        T2l = FMA(KP559016994, T2k, T2j);
        T2Z = FNMS(KP559016994, T2k, T2j);
      }
      {
        E TE, T3y, Tl, T3x, Tb, Tk;
        TE = ADD(Tu, TD);
        T3y = ADD(T3q, T3r);
        Tb = ADD(T5, Ta);
        Tk = ADD(Tc, Tj);
        Tl = FMA(KP2_000000000, Tk, Tb);
        T3x = FNMS(KP2_000000000, Tk, Tb);
        R0[WS(rs, 10)] = FNMS(KP2_000000000, TE, Tl);
        R0[WS(rs, 15)] = FMA(KP2_000000000, T3y, T3x);
        R0[0] = FMA(KP2_000000000, TE, Tl);
        R0[WS(rs, 5)] = FNMS(KP2_000000000, T3y, T3x);
      }
      {
        E T3p, T3v, T3u, T3w;
        {
          E T3n, T3o, T3s, T3t;
          T3n = SUB(T5, Ta);
          T3o = ADD(T22, T25);
          T3p = FNMS(KP2_000000000, T3o, T3n);
          T3v = FMA(KP2_000000000, T3o, T3n);
          T3s = SUB(T3q, T3r);
          T3t = SUB(Tu, TD);
          T3u = SUB(T3s, T3t);
          T3w = ADD(T3t, T3s);
        }
        R1[WS(rs, 12)] = FNMS(KP1_414213562, T3u, T3p);
        R1[WS(rs, 7)] = FMA(KP1_414213562, T3w, T3v);
        R1[WS(rs, 2)] = FMA(KP1_414213562, T3u, T3p);
        R1[WS(rs, 17)] = FNMS(KP1_414213562, T3w, T3v);
      }
      {
        E T14, T1w, T2c, T2I, TT, T2H, T2N, T2S, T1s, T2K, T1A, T2e,
         T2B, T2G, T1v;
        E T21, TW, T28;
        TW = FMA(KP559016994, TV, TU);
        T14 = FMA(KP951056516, T13, TW);
        T1w = FNMS(KP951056516, T13, TW);
        T28 = FMA(KP559016994, T27, T26);
        T2c = FMA(KP951056516, T2b, T28);
        T2I = FNMS(KP951056516, T2b, T28);
        {
          E TL, TS, T2L, T2M;
          TL = FMA(KP1_902113032, TK, TH);
          TS = FNMS(KP1_902113032, TR, TO);
          TT = ADD(TL, TS);
          T2H = SUB(TL, TS);
          T2L = FNMS(KP951056516, T2o, T2l);
          T2M = FMA(KP951056516, T2z, T2w);
          T2N = ADD(T2L, T2M);
          T2S = SUB(T2L, T2M);
        }
        {
          E T1g, T1r, T1y, T1z;
          T1g = FMA(KP951056516, T1f, T18);
          T1r = FNMS(KP951056516, T1q, T1j);
          T1s = ADD(T1g, T1r);
          T2K = SUB(T1g, T1r);
          T1y = FNMS(KP951056516, T1f, T18);
          T1z = FMA(KP951056516, T1q, T1j);
          T1A = ADD(T1y, T1z);
          T2e = SUB(T1y, T1z);
        }
        {
          E T2p, T2A, T1t, T1u;
          T2p = FMA(KP951056516, T2o, T2l);
          T2A = FNMS(KP951056516, T2z, T2w);
          T2B = ADD(T2p, T2A);
          T2G = SUB(T2p, T2A);
          T1t = FNMS(KP1_902113032, TK, TH);
          T1u = FMA(KP1_902113032, TR, TO);
          T1v = ADD(T1t, T1u);
          T21 = SUB(T1t, T1u);
        }
        {
          E T15, T2F, T2R, T1x;
          T15 = FMA(KP2_000000000, T14, TT);
          R0[WS(rs, 2)] = FNMS(KP2_000000000, T1s, T15);
          R0[WS(rs, 12)] = FMA(KP2_000000000, T1s, T15);
          T2F = FNMS(KP2_000000000, T1w, T1v);
          R0[WS(rs, 13)] = FNMS(KP2_000000000, T2G, T2F);
          R0[WS(rs, 3)] = FMA(KP2_000000000, T2G, T2F);
          T2R = FNMS(KP2_000000000, T14, TT);
          R0[WS(rs, 17)] = FNMS(KP2_000000000, T2S, T2R);
          R0[WS(rs, 7)] = FMA(KP2_000000000, T2S, T2R);
          T1x = FMA(KP2_000000000, T1w, T1v);
          R0[WS(rs, 18)] = FNMS(KP2_000000000, T1A, T1x);
          R0[WS(rs, 8)] = FMA(KP2_000000000, T1A, T1x);
        }
        {
          E T2d, T2C, T2J, T2O;
          T2d = FNMS(KP2_000000000, T2c, T21);
          T2C = SUB(T2e, T2B);
          R1[WS(rs, 10)] = FNMS(KP1_414213562, T2C, T2d);
          R1[0] = FMA(KP1_414213562, T2C, T2d);
          T2J = FNMS(KP2_000000000, T2I, T2H);
          T2O = SUB(T2K, T2N);
          R1[WS(rs, 14)] = FNMS(KP1_414213562, T2O, T2J);
          R1[WS(rs, 4)] = FMA(KP1_414213562, T2O, T2J);
        }
        {
          E T2P, T2Q, T2D, T2E;
          T2P = FMA(KP2_000000000, T2I, T2H);
          T2Q = ADD(T2K, T2N);
          R1[WS(rs, 9)] = FNMS(KP1_414213562, T2Q, T2P);
          R1[WS(rs, 19)] = FMA(KP1_414213562, T2Q, T2P);
          T2D = FMA(KP2_000000000, T2c, T21);
          T2E = ADD(T2e, T2B);
          R1[WS(rs, 5)] = FNMS(KP1_414213562, T2E, T2D);
          R1[WS(rs, 15)] = FMA(KP1_414213562, T2E, T2D);
        }
      }
      {
        E T1K, T1W, T2W, T3c, T1H, T3b, T3h, T3m, T1S, T3e, T20, T2Y,
         T35, T3a, T1V;
        E T2T, T1I, T2U;
        T1I = FNMS(KP559016994, TV, TU);
        T1K = FNMS(KP951056516, T1J, T1I);
        T1W = FMA(KP951056516, T1J, T1I);
        T2U = FNMS(KP559016994, T27, T26);
        T2W = FNMS(KP951056516, T2V, T2U);
        T3c = FMA(KP951056516, T2V, T2U);
        {
          E T1D, T1G, T3f, T3g;
          T1D = FMA(KP1_902113032, T1C, T1B);
          T1G = FMA(KP1_902113032, T1F, T1E);
          T1H = ADD(T1D, T1G);
          T3b = SUB(T1D, T1G);
          T3f = FMA(KP951056516, T30, T2Z);
          T3g = FNMS(KP951056516, T33, T32);
          T3h = ADD(T3f, T3g);
          T3m = SUB(T3f, T3g);
        }
        {
          E T1O, T1R, T1Y, T1Z;
          T1O = FNMS(KP951056516, T1N, T1M);
          T1R = FNMS(KP951056516, T1Q, T1P);
          T1S = ADD(T1O, T1R);
          T3e = SUB(T1O, T1R);
          T1Y = FMA(KP951056516, T1N, T1M);
          T1Z = FMA(KP951056516, T1Q, T1P);
          T20 = ADD(T1Y, T1Z);
          T2Y = SUB(T1Y, T1Z);
        }
        {
          E T31, T34, T1T, T1U;
          T31 = FNMS(KP951056516, T30, T2Z);
          T34 = FMA(KP951056516, T33, T32);
          T35 = ADD(T31, T34);
          T3a = SUB(T31, T34);
          T1T = FNMS(KP1_902113032, T1C, T1B);
          T1U = FNMS(KP1_902113032, T1F, T1E);
          T1V = ADD(T1T, T1U);
          T2T = SUB(T1T, T1U);
        }
        {
          E T1L, T39, T3l, T1X;
          T1L = FMA(KP2_000000000, T1K, T1H);
          R0[WS(rs, 14)] = FNMS(KP2_000000000, T1S, T1L);
          R0[WS(rs, 4)] = FMA(KP2_000000000, T1S, T1L);
          T39 = FNMS(KP2_000000000, T1W, T1V);
          R0[WS(rs, 1)] = FNMS(KP2_000000000, T3a, T39);
          R0[WS(rs, 11)] = FMA(KP2_000000000, T3a, T39);
          T3l = FNMS(KP2_000000000, T1K, T1H);
          R0[WS(rs, 9)] = FNMS(KP2_000000000, T3m, T3l);
          R0[WS(rs, 19)] = FMA(KP2_000000000, T3m, T3l);
          T1X = FMA(KP2_000000000, T1W, T1V);
          R0[WS(rs, 6)] = FNMS(KP2_000000000, T20, T1X);
          R0[WS(rs, 16)] = FMA(KP2_000000000, T20, T1X);
        }
        {
          E T2X, T36, T3d, T3i;
          T2X = FNMS(KP2_000000000, T2W, T2T);
          T36 = SUB(T2Y, T35);
          R1[WS(rs, 18)] = FNMS(KP1_414213562, T36, T2X);
          R1[WS(rs, 8)] = FMA(KP1_414213562, T36, T2X);
          T3d = FMA(KP2_000000000, T3c, T3b);
          T3i = ADD(T3e, T3h);
          R1[WS(rs, 1)] = FNMS(KP1_414213562, T3i, T3d);
          R1[WS(rs, 11)] = FMA(KP1_414213562, T3i, T3d);
        }
        {
          E T3j, T3k, T37, T38;
          T3j = FNMS(KP2_000000000, T3c, T3b);
          T3k = SUB(T3h, T3e);
          R1[WS(rs, 16)] = FNMS(KP1_414213562, T3k, T3j);
          R1[WS(rs, 6)] = FMA(KP1_414213562, T3k, T3j);
          T37 = FMA(KP2_000000000, T2W, T2T);
          T38 = ADD(T2Y, T35);
          R1[WS(rs, 13)] = FNMS(KP1_414213562, T38, T37);
          R1[WS(rs, 3)] = FMA(KP1_414213562, T38, T37);
        }
      }
    }
  }
}