/* Generated by: /Users/sfegan/GitHub/fftw3/genfft/gen_r2cb.native -n 24 -sign 1 -standalone -fma -generic-arith -compact -name dft_codelet_r2cb_24 */

/*
 * This function contains 104 FP additions, 44 FP multiplications,
 * (or, 60 additions, 0 multiplications, 44 fused multiply/add),
 * 41 stack variables, 4 constants, and 48 memory accesses
 */
void dft_codelet_r2cb_24(R * R0, R * R1, R * Cr, R * Ci, stride rs, stride csr, stride csi, INT v, INT ivs, INT ovs)
{
  DK(KP707106781, +0.707106781186547524400844362104849039284835938);
  DK(KP1_414213562, +1.414213562373095048801688724209698078569671875);
  DK(KP1_732050807, +1.732050807568877293527446341505872366942805254);
  DK(KP2_000000000, +2.000000000000000000000000000000000000000000000);
  {
    INT i;
    for (i = v; i > 0; i = i - 1, R0 = R0 + ovs, R1 = R1 + ovs, Cr = Cr + ivs, Ci = Ci + ivs, MAKE_VOLATILE_STRIDE(96, rs), MAKE_VOLATILE_STRIDE(96, csr), MAKE_VOLATILE_STRIDE(96, csi)) {
      E Tc, TZ, Tw, Tz, T1w, TY, T7, Tr, T1v, TO, TN, Tu, Ti, T1z, TG;
      E TS, T17, T1m, Tn, T1A, TL, TT, T1c, T1n;
      {
        E T8, T9, Ta, Tb;
        T8 = Cr[WS(csr, 6)];
        T9 = Cr[WS(csr, 10)];
        Ta = Cr[WS(csr, 2)];
        Tb = ADD(T9, Ta);
        Tc = ADD(T8, Tb);
        TZ = SUB(T9, Ta);
        Tw = FNMS(KP2_000000000, T8, Tb);
      }
      {
        E TX, Tx, Ty, TW;
        TX = Ci[WS(csi, 6)];
        Tx = Ci[WS(csi, 10)];
        Ty = Ci[WS(csi, 2)];
        TW = ADD(Tx, Ty);
        Tz = SUB(Tx, Ty);
        T1w = SUB(TX, TW);
        TY = FMA(KP2_000000000, TX, TW);
      }
      {
        E Tq, T3, Tp, Tt, T6, Ts;
        {
          E T1, T2, T4, T5;
          Tq = Ci[WS(csi, 8)];
          T1 = Cr[0];
          T2 = Cr[WS(csr, 8)];
          T3 = FMA(KP2_000000000, T2, T1);
          Tp = SUB(T1, T2);
          Tt = Ci[WS(csi, 4)];
          T4 = Cr[WS(csr, 12)];
          T5 = Cr[WS(csr, 4)];
          T6 = FMA(KP2_000000000, T5, T4);
          Ts = SUB(T4, T5);
        }
        T7 = ADD(T3, T6);
        Tr = FMA(KP1_732050807, Tq, Tp);
        T1v = SUB(T3, T6);
        TO = FMA(KP1_732050807, Tt, Ts);
        TN = FNMS(KP1_732050807, Tq, Tp);
        Tu = FNMS(KP1_732050807, Tt, Ts);
      }
      {
        E Te, T14, Th, T16, TF, T13, TC, T15;
        Te = Cr[WS(csr, 3)];
        T14 = Ci[WS(csi, 3)];
        {
          E Tf, Tg, TD, TE;
          Tf = Cr[WS(csr, 11)];
          Tg = Cr[WS(csr, 5)];
          Th = ADD(Tf, Tg);
          T16 = SUB(Tf, Tg);
          TD = Ci[WS(csi, 11)];
          TE = Ci[WS(csi, 5)];
          TF = ADD(TD, TE);
          T13 = SUB(TE, TD);
        }
        Ti = ADD(Te, Th);
        T1z = SUB(T14, T13);
        TC = FNMS(KP2_000000000, Te, Th);
        TG = FNMS(KP1_732050807, TF, TC);
        TS = FMA(KP1_732050807, TF, TC);
        T15 = FMA(KP2_000000000, T14, T13);
        T17 = FMA(KP1_732050807, T16, T15);
        T1m = FNMS(KP1_732050807, T16, T15);
      }
      {
        E Tj, T19, Tm, T1b, TK, T18, TH, T1a;
        Tj = Cr[WS(csr, 9)];
        T19 = Ci[WS(csi, 9)];
        {
          E Tk, Tl, TI, TJ;
          Tk = Cr[WS(csr, 1)];
          Tl = Cr[WS(csr, 7)];
          Tm = ADD(Tk, Tl);
          T1b = SUB(Tk, Tl);
          TI = Ci[WS(csi, 1)];
          TJ = Ci[WS(csi, 7)];
          TK = ADD(TI, TJ);
          T18 = SUB(TI, TJ);
        }
        Tn = ADD(Tj, Tm);
        T1A = ADD(T18, T19);
        TH = FNMS(KP2_000000000, Tj, Tm);
        TL = FMA(KP1_732050807, TK, TH);
        TT = FNMS(KP1_732050807, TK, TH);
        T1a = FNMS(KP2_000000000, T19, T18);
        T1c = FMA(KP1_732050807, T1b, T1a);
        T1n = FNMS(KP1_732050807, T1b, T1a);
      }
      {
        E Td, To, T1F, T1G;
        Td = FMA(KP2_000000000, Tc, T7);
        To = ADD(Ti, Tn);
        R0[WS(rs, 6)] = FNMS(KP2_000000000, To, Td);
        R0[0] = FMA(KP2_000000000, To, Td);
        T1F = FNMS(KP2_000000000, Tc, T7);
        T1G = SUB(T1z, T1A);
        R0[WS(rs, 9)] = FNMS(KP2_000000000, T1G, T1F);
        R0[WS(rs, 3)] = FMA(KP2_000000000, T1G, T1F);
        {
          E T1x, T1D, T1C, T1E, T1y, T1B;
          T1x = FNMS(KP2_000000000, T1w, T1v);
          T1D = FMA(KP2_000000000, T1w, T1v);
          T1y = SUB(Ti, Tn);
          T1B = ADD(T1z, T1A);
          T1C = SUB(T1y, T1B);
          T1E = ADD(T1y, T1B);
          R1[WS(rs, 10)] = FNMS(KP1_414213562, T1C, T1x);
          R1[WS(rs, 7)] = FMA(KP1_414213562, T1E, T1D);
          R1[WS(rs, 4)] = FMA(KP1_414213562, T1C, T1x);
          R1[WS(rs, 1)] = FNMS(KP1_414213562, T1E, T1D);
        }
      }
      {
        E TU, T1i, TR, T1h, TP, TQ;
        TU = ADD(TS, TT);
        T1i = ADD(T17, T1c);
        TP = ADD(TN, TO);
        TQ = FNMS(KP1_732050807, Tz, Tw);
        TR = SUB(TP, TQ);
        T1h = ADD(TP, TQ);
        R0[WS(rs, 2)] = ADD(TR, TU);
        R0[WS(rs, 11)] = ADD(T1h, T1i);
        R0[WS(rs, 8)] = SUB(TR, TU);
        R0[WS(rs, 5)] = SUB(T1h, T1i);
      }
      {
        E TM, T1u, TB, T1t, Tv, TA;
        TM = ADD(TG, TL);
        T1u = ADD(T1m, T1n);
        Tv = ADD(Tr, Tu);
        TA = FMA(KP1_732050807, Tz, Tw);
        TB = SUB(Tv, TA);
        T1t = ADD(Tv, TA);
        R0[WS(rs, 10)] = ADD(TB, TM);
        R0[WS(rs, 7)] = ADD(T1t, T1u);
        R0[WS(rs, 4)] = SUB(TB, TM);
        R0[WS(rs, 1)] = SUB(T1t, T1u);
      }
      {
        E T1l, T1r, T1q, T1s;
        {
          E T1j, T1k, T1o, T1p;
          T1j = SUB(Tr, Tu);
          T1k = FNMS(KP1_732050807, TZ, TY);
          T1l = SUB(T1j, T1k);
          T1r = ADD(T1j, T1k);
          T1o = SUB(T1m, T1n);
          T1p = SUB(TL, TG);
          T1q = SUB(T1o, T1p);
          T1s = ADD(T1p, T1o);
        }
        R1[WS(rs, 8)] = FNMS(KP707106781, T1q, T1l);
        R1[WS(rs, 11)] = FMA(KP707106781, T1s, T1r);
        R1[WS(rs, 2)] = FMA(KP707106781, T1q, T1l);
        R1[WS(rs, 5)] = FNMS(KP707106781, T1s, T1r);
      }
      {
        E T11, T1f, T1e, T1g;
        {
          E TV, T10, T12, T1d;
          TV = SUB(TN, TO);
          T10 = FMA(KP1_732050807, TZ, TY);
          T11 = SUB(TV, T10);
          T1f = ADD(TV, T10);
          T12 = SUB(TT, TS);
          T1d = SUB(T17, T1c);
          T1e = SUB(T12, T1d);
          T1g = ADD(T12, T1d);
        }
        R1[WS(rs, 6)] = FNMS(KP707106781, T1e, T11);
        R1[WS(rs, 3)] = FMA(KP707106781, T1g, T1f);
        R1[0] = FMA(KP707106781, T1e, T11);
        R1[WS(rs, 9)] = FNMS(KP707106781, T1g, T1f);
      }
    }
  }
}