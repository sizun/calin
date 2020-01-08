/* Generated by: /Users/sfegan/GitHub/fftw3/genfft/gen_r2cf.native -n 24 -standalone -fma -generic-arith -compact -name dft_codelet_r2cf_24 */

/*
 * This function contains 104 FP additions, 34 FP multiplications,
 * (or, 72 additions, 2 multiplications, 32 fused multiply/add),
 * 46 stack variables, 3 constants, and 48 memory accesses
 */
void dft_codelet_r2cf_24(R * R0, R * R1, R * Cr, R * Ci, stride rs, stride csr, stride csi, INT v, INT ivs, INT ovs)
{
  DK(KP707106781, +0.707106781186547524400844362104849039284835938);
  DK(KP866025403, +0.866025403784438646763723170752936183471402627);
  DK(KP500000000, +0.500000000000000000000000000000000000000000000);
  {
    INT i;
    for (i = v; i > 0; i = i - 1, R0 = R0 + ivs, R1 = R1 + ivs, Cr = Cr + ovs, Ci = Ci + ovs, MAKE_VOLATILE_STRIDE(96, rs), MAKE_VOLATILE_STRIDE(96, csr), MAKE_VOLATILE_STRIDE(96, csi)) {
      E T5, Ta, T1e, TS, TR, TP, Tg, Tl, T1s, TV, TU, TM, T13, T1D,
       T14;
      E T1o, Tr, Tw, T1l, T10, T1C, T11, T1j, TC, TH, T1g, T1B, T1E;
      {
        E T1, T6, T4, TN, T9, TO;
        T1 = R0[0];
        T6 = R0[WS(rs, 6)];
        {
          E T2, T3, T7, T8;
          T2 = R0[WS(rs, 4)];
          T3 = R0[WS(rs, 8)];
          T4 = ADD(T2, T3);
          TN = SUB(T3, T2);
          T7 = R0[WS(rs, 10)];
          T8 = R0[WS(rs, 2)];
          T9 = ADD(T7, T8);
          TO = SUB(T8, T7);
        }
        T5 = ADD(T1, T4);
        Ta = ADD(T6, T9);
        T1e = SUB(TN, TO);
        TS = FNMS(KP500000000, T9, T6);
        TR = FNMS(KP500000000, T4, T1);
        TP = ADD(TN, TO);
      }
      {
        E Tc, Th, Tf, TK, Tk, TL;
        Tc = R0[WS(rs, 3)];
        Th = R0[WS(rs, 9)];
        {
          E Td, Te, Ti, Tj;
          Td = R0[WS(rs, 7)];
          Te = R0[WS(rs, 11)];
          Tf = ADD(Td, Te);
          TK = SUB(Te, Td);
          Ti = R0[WS(rs, 1)];
          Tj = R0[WS(rs, 5)];
          Tk = ADD(Ti, Tj);
          TL = SUB(Tj, Ti);
        }
        Tg = ADD(Tc, Tf);
        Tl = ADD(Th, Tk);
        T1s = SUB(TK, TL);
        TV = FNMS(KP500000000, Tk, Th);
        TU = FNMS(KP500000000, Tf, Tc);
        TM = ADD(TK, TL);
      }
      {
        E Tn, Ts, Tq, T1m, Tv, T1n;
        Tn = R1[WS(rs, 10)];
        Ts = R1[WS(rs, 4)];
        {
          E To, Tp, Tt, Tu;
          To = R1[WS(rs, 2)];
          Tp = R1[WS(rs, 6)];
          Tq = ADD(To, Tp);
          T1m = SUB(Tp, To);
          Tt = R1[WS(rs, 8)];
          Tu = R1[0];
          Tv = ADD(Tt, Tu);
          T1n = SUB(Tu, Tt);
        }
        T13 = ADD(Tn, Tq);
        T1D = ADD(T1m, T1n);
        T14 = ADD(Ts, Tv);
        T1o = SUB(T1m, T1n);
        Tr = FNMS(KP500000000, Tq, Tn);
        Tw = FNMS(KP500000000, Tv, Ts);
        T1l = SUB(Tr, Tw);
      }
      {
        E Ty, TD, TB, T1i, TG, T1h;
        Ty = R1[WS(rs, 1)];
        TD = R1[WS(rs, 7)];
        {
          E Tz, TA, TE, TF;
          Tz = R1[WS(rs, 5)];
          TA = R1[WS(rs, 9)];
          TB = ADD(Tz, TA);
          T1i = SUB(Tz, TA);
          TE = R1[WS(rs, 11)];
          TF = R1[WS(rs, 3)];
          TG = ADD(TE, TF);
          T1h = SUB(TE, TF);
        }
        T10 = ADD(Ty, TB);
        T1C = ADD(T1i, T1h);
        T11 = ADD(TD, TG);
        T1j = SUB(T1h, T1i);
        TC = FNMS(KP500000000, TB, Ty);
        TH = FNMS(KP500000000, TG, TD);
        T1g = SUB(TC, TH);
      }
      T1B = ADD(TP, TM);
      T1E = SUB(T1C, T1D);
      Ci[WS(csi, 4)] = MUL(KP866025403, ADD(T1B, T1E));
      Ci[WS(csi, 8)] = MUL(KP866025403, SUB(T1E, T1B));
      {
        E TZ, T17, T16, T18, T12, T15;
        TZ = SUB(T5, Ta);
        T17 = SUB(Tg, Tl);
        T12 = SUB(T10, T11);
        T15 = SUB(T13, T14);
        T16 = ADD(T12, T15);
        T18 = SUB(T15, T12);
        Cr[WS(csr, 3)] = FNMS(KP707106781, T16, TZ);
        Ci[WS(csi, 3)] = FMA(KP707106781, T18, T17);
        Cr[WS(csr, 9)] = FMA(KP707106781, T16, TZ);
        Ci[WS(csi, 9)] = NEG(FNMS(KP707106781, T18, T17));
      }
      {
        E Tb, Tm, T1b, T19, T1a, T1c;
        Tb = ADD(T5, Ta);
        Tm = ADD(Tg, Tl);
        T1b = ADD(Tb, Tm);
        T19 = ADD(T10, T11);
        T1a = ADD(T13, T14);
        T1c = ADD(T19, T1a);
        Cr[WS(csr, 6)] = SUB(Tb, Tm);
        Ci[WS(csi, 6)] = SUB(T19, T1a);
        Cr[WS(csr, 12)] = SUB(T1b, T1c);
        Cr[0] = ADD(T1b, T1c);
      }
      {
        E TQ, T1G, TJ, TY, TX, T1F;
        TQ = SUB(TM, TP);
        T1G = ADD(T1C, T1D);
        {
          E Tx, TI, TT, TW;
          Tx = ADD(Tr, Tw);
          TI = ADD(TC, TH);
          TJ = SUB(Tx, TI);
          TY = ADD(TI, Tx);
          TT = ADD(TR, TS);
          TW = ADD(TU, TV);
          TX = ADD(TT, TW);
          T1F = SUB(TT, TW);
        }
        Ci[WS(csi, 2)] = FMA(KP866025403, TQ, TJ);
        Cr[WS(csr, 2)] = FMA(KP866025403, T1G, T1F);
        Ci[WS(csi, 10)] = FNMS(KP866025403, TQ, TJ);
        Cr[WS(csr, 10)] = FNMS(KP866025403, T1G, T1F);
        Cr[WS(csr, 8)] = ADD(TX, TY);
        Cr[WS(csr, 4)] = SUB(TX, TY);
      }
      {
        E T1f, T1v, T1t, T1z, T1q, T1u, T1y, T1A, T1d, T1r;
        T1d = SUB(TU, TV);
        T1f = FMA(KP866025403, T1e, T1d);
        T1v = FNMS(KP866025403, T1e, T1d);
        T1r = SUB(TR, TS);
        T1t = FMA(KP866025403, T1s, T1r);
        T1z = FNMS(KP866025403, T1s, T1r);
        {
          E T1k, T1p, T1w, T1x;
          T1k = FMA(KP866025403, T1j, T1g);
          T1p = FNMS(KP866025403, T1o, T1l);
          T1q = SUB(T1k, T1p);
          T1u = ADD(T1k, T1p);
          T1w = FMA(KP866025403, T1o, T1l);
          T1x = FNMS(KP866025403, T1j, T1g);
          T1y = SUB(T1w, T1x);
          T1A = ADD(T1x, T1w);
        }
        Ci[WS(csi, 5)] = NEG(FNMS(KP707106781, T1q, T1f));
        Cr[WS(csr, 5)] = FNMS(KP707106781, T1A, T1z);
        Ci[WS(csi, 7)] = FMA(KP707106781, T1q, T1f);
        Cr[WS(csr, 7)] = FMA(KP707106781, T1A, T1z);
        Cr[WS(csr, 11)] = FNMS(KP707106781, T1u, T1t);
        Ci[WS(csi, 11)] = FMA(KP707106781, T1y, T1v);
        Cr[WS(csr, 1)] = FMA(KP707106781, T1u, T1t);
        Ci[WS(csi, 1)] = NEG(FNMS(KP707106781, T1y, T1v));
      }
    }
  }
}