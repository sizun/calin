/* Generated by: /Users/sfegan/GitHub/fftw3/genfft/gen_r2cb.native -n 16 -sign 1 -standalone -fma -generic-arith -compact -name dft_codelet_r2cb_16 */

/*
 * This function contains 58 FP additions, 32 FP multiplications,
 * (or, 26 additions, 0 multiplications, 32 fused multiply/add),
 * 31 stack variables, 4 constants, and 32 memory accesses
 */
void dft_codelet_r2cb_16(R * R0, R * R1, R * Cr, R * Ci, stride rs, stride csr, stride csi, INT v, INT ivs, INT ovs)
{
  DK(KP1_847759065, +1.847759065022573512256366378793576573644833252);
  DK(KP414213562, +0.414213562373095048801688724209698078569671875);
  DK(KP1_414213562, +1.414213562373095048801688724209698078569671875);
  DK(KP2_000000000, +2.000000000000000000000000000000000000000000000);
  {
    INT i;
    for (i = v; i > 0; i = i - 1, R0 = R0 + ovs, R1 = R1 + ovs, Cr = Cr + ivs, Ci = Ci + ivs, MAKE_VOLATILE_STRIDE(64, rs), MAKE_VOLATILE_STRIDE(64, csr), MAKE_VOLATILE_STRIDE(64, csi)) {
      E T5, TL, Tj, TD, T8, TM, To, TE, Tc, TP, Tf, TQ, Tu, Tz, TR;
      E TO, TH, TG;
      {
        E T4, Ti, T3, Th, T1, T2;
        T4 = Cr[WS(csr, 4)];
        Ti = Ci[WS(csi, 4)];
        T1 = Cr[0];
        T2 = Cr[WS(csr, 8)];
        T3 = ADD(T1, T2);
        Th = SUB(T1, T2);
        T5 = FMA(KP2_000000000, T4, T3);
        TL = FNMS(KP2_000000000, T4, T3);
        Tj = FNMS(KP2_000000000, Ti, Th);
        TD = FMA(KP2_000000000, Ti, Th);
      }
      {
        E T6, T7, Tk, Tl, Tm, Tn;
        T6 = Cr[WS(csr, 2)];
        T7 = Cr[WS(csr, 6)];
        Tk = SUB(T6, T7);
        Tl = Ci[WS(csi, 2)];
        Tm = Ci[WS(csi, 6)];
        Tn = ADD(Tl, Tm);
        T8 = ADD(T6, T7);
        TM = SUB(Tl, Tm);
        To = SUB(Tk, Tn);
        TE = ADD(Tk, Tn);
      }
      {
        E Tq, Ty, Tv, Tt;
        {
          E Ta, Tb, Tw, Tx;
          Ta = Cr[WS(csr, 1)];
          Tb = Cr[WS(csr, 7)];
          Tc = ADD(Ta, Tb);
          Tq = SUB(Ta, Tb);
          Tw = Ci[WS(csi, 1)];
          Tx = Ci[WS(csi, 7)];
          Ty = ADD(Tw, Tx);
          TP = SUB(Tw, Tx);
        }
        {
          E Td, Te, Tr, Ts;
          Td = Cr[WS(csr, 5)];
          Te = Cr[WS(csr, 3)];
          Tf = ADD(Td, Te);
          Tv = SUB(Td, Te);
          Tr = Ci[WS(csi, 5)];
          Ts = Ci[WS(csi, 3)];
          Tt = ADD(Tr, Ts);
          TQ = SUB(Tr, Ts);
        }
        Tu = SUB(Tq, Tt);
        Tz = ADD(Tv, Ty);
        TR = SUB(TP, TQ);
        TO = SUB(Tc, Tf);
        TH = ADD(Tq, Tt);
        TG = SUB(Ty, Tv);
      }
      {
        E T9, Tg, TT, TU;
        T9 = FMA(KP2_000000000, T8, T5);
        Tg = ADD(Tc, Tf);
        R0[WS(rs, 4)] = FNMS(KP2_000000000, Tg, T9);
        R0[0] = FMA(KP2_000000000, Tg, T9);
        TT = FMA(KP2_000000000, TM, TL);
        TU = ADD(TO, TR);
        R0[WS(rs, 3)] = FNMS(KP1_414213562, TU, TT);
        R0[WS(rs, 7)] = FMA(KP1_414213562, TU, TT);
      }
      {
        E TV, TW, Tp, TA;
        TV = FNMS(KP2_000000000, T8, T5);
        TW = ADD(TQ, TP);
        R0[WS(rs, 2)] = FNMS(KP2_000000000, TW, TV);
        R0[WS(rs, 6)] = FMA(KP2_000000000, TW, TV);
        Tp = FMA(KP1_414213562, To, Tj);
        TA = FNMS(KP414213562, Tz, Tu);
        R1[WS(rs, 4)] = FNMS(KP1_847759065, TA, Tp);
        R1[0] = FMA(KP1_847759065, TA, Tp);
      }
      {
        E TB, TC, TJ, TK;
        TB = FNMS(KP1_414213562, To, Tj);
        TC = FMA(KP414213562, Tu, Tz);
        R1[WS(rs, 2)] = FNMS(KP1_847759065, TC, TB);
        R1[WS(rs, 6)] = FMA(KP1_847759065, TC, TB);
        TJ = FMA(KP1_414213562, TE, TD);
        TK = FMA(KP414213562, TG, TH);
        R1[WS(rs, 3)] = FNMS(KP1_847759065, TK, TJ);
        R1[WS(rs, 7)] = FMA(KP1_847759065, TK, TJ);
      }
      {
        E TN, TS, TF, TI;
        TN = FNMS(KP2_000000000, TM, TL);
        TS = SUB(TO, TR);
        R0[WS(rs, 5)] = FNMS(KP1_414213562, TS, TN);
        R0[WS(rs, 1)] = FMA(KP1_414213562, TS, TN);
        TF = FNMS(KP1_414213562, TE, TD);
        TI = FNMS(KP414213562, TH, TG);
        R1[WS(rs, 1)] = FNMS(KP1_847759065, TI, TF);
        R1[WS(rs, 5)] = FMA(KP1_847759065, TI, TF);
      }
    }
  }
}
