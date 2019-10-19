#include "iodefine.h"
#include "cpwm.h"

void init_cpwm(int carrier_cycle, int deadtime, int init_tgr_u, int init_tgr_v, int init_tgr_w)
{
    STB.CR4.BIT._MTU2 = 0;

    // [1] カウンタ動作停止
    MTU2.TSTR.BIT.CST3 = 0;
    MTU2.TSTR.BIT.CST4 = 0;

    // [2] カウンタクロック・カウンタクリア要因選択
    // 立ち上がりエッジでカウント
    // 内部クロック φ / 1 でカウント
    MTU23.TCR.BIT.CKEG = 0;
    MTU23.TCR.BIT.TPSC = 0;
    MTU24.TCR.BIT.CKEG = 0;
    MTU24.TCR.BIT.TPSC = 0;

    // [3] BLDC モータ制御設定
    // 無効
    MTU2.TGCR.BIT.BDC = 0;
    MTU2.TGCR.BIT.N = 1;
    MTU2.TGCR.BIT.P = 1;

    // [4] TCNT 設定
    MTU23.TCNT = deadtime;
    MTU23.TCNT = 0;

    // [5] チャネル間の同期設定
    // 設定なし

    // [6] TGR 初期設定
    MTU23.TGRB = init_tgr_u;
    MTU24.TGRA = init_tgr_v;
    MTU24.TGRB = init_tgr_w;
    MTU23.TGRD = init_tgr_u;
    MTU24.TGRC = init_tgr_v;
    MTU24.TGRD = init_tgr_w;

    // [7] デッドタイム生成の有無の設定
    // デフォルト(設定あり)

    // [8] デッドタイム・キャリア周期設定
    MTU2.TDDR = deadtime;          // デッドタイム
    MTU2.TCDR = carrier_cycle / 2; // キャリア周期
    MTU2.TCBR = carrier_cycle / 2; // キャリア周期バッファ
    MTU23.TGRA = carrier_cycle / 2 + deadtime;
    MTU23.TGRC = carrier_cycle / 2 + deadtime;

    // [9] PWM 波形出力許可・PWM 出力レベル設定
    MTU2.TOCR1.BIT.PSYE = 0; // PWM 同期出力禁止
    MTU2.TOCR1.BIT.OLSN = 0; // Active -> LOW; Up -> HIGH
    MTU2.TOCR1.BIT.OLSP = 0; // Active -> LOW; Up -> LOW

    // [10] 相補 PWM モード設定
    MTU23.TMDR.BIT.MD = 0xE; // 相補 PWM モード (谷転送)
    MTU23.TMDR.BIT.BFA = 1;
    MTU23.TMDR.BIT.BFB = 1; // バッファ動作 B; TGRB と TGRD はバッファ動作
    MTU23.TMDR.BIT.BFA = 1; // バッファ動作 A; TGRA と TGRC はバッファ動作

    // [11] 波形出力許可
    MTU2.TOER.BIT.OE3B = 1; // TIOC3B
    MTU2.TOER.BIT.OE3D = 1; // TIOC3D
    MTU2.TOER.BIT.OE4A = 1; // TIOC4A
    MTU2.TOER.BIT.OE4C = 1; // TIOC4C
    MTU2.TOER.BIT.OE4B = 1; // TIOC4B
    MTU2.TOER.BIT.OE4D = 1; // TIOC4D

    // [12] PFC 設定
    PFC.PECRL3.BIT.PE9MD = 1;  // TIOC3B 出力
    PFC.PECRL3.BIT.PE11MD = 1; // TIOC3D 出力
    PFC.PECRL4.BIT.PE12MD = 1; // TIOC4A 出力
    PFC.PECRL4.BIT.PE13MD = 1; // TIOC4B 出力
    PFC.PECRL4.BIT.PE14MD = 1; // TIOC4C 出力
    PFC.PECRL4.BIT.PE15MD = 1; // TIOC4D 出力

    PFC.PEIORL.BIT.B9 = 1;  // PE9  出力
    PFC.PEIORL.BIT.B11 = 1; // PE11 出力
    PFC.PEIORL.BIT.B12 = 1; // PE12 出力
    PFC.PEIORL.BIT.B13 = 1; // PE13 出力
    PFC.PEIORL.BIT.B14 = 1; // PE14 出力
    PFC.PEIORL.BIT.B15 = 1; // PE15 出力
}

void start_cpwm(void)
{
    // PWM 波形出力許可
    MTU2.TOER.BIT.OE3B = 1; // TIOC3B
    MTU2.TOER.BIT.OE3D = 1; // TIOC3D
    MTU2.TOER.BIT.OE4A = 1; // TIOC4A
    MTU2.TOER.BIT.OE4C = 1; // TIOC4C
    MTU2.TOER.BIT.OE4B = 1; // TIOC4B
    MTU2.TOER.BIT.OE4D = 1; // TIOC4D

    MTU2.TSTR.BIT.CST3 = 1;
    MTU2.TSTR.BIT.CST4 = 1;
}

void stop_cpwm(void)
{
    MTU2.TSTR.BIT.CST3 = 0;
    MTU2.TSTR.BIT.CST4 = 0;

    // PWM 波形出力禁止
    MTU2.TOER.BIT.OE3B = 0; // TIOC3B
    MTU2.TOER.BIT.OE3D = 0; // TIOC3D
    MTU2.TOER.BIT.OE4A = 0; // TIOC4A
    MTU2.TOER.BIT.OE4C = 0; // TIOC4C
    MTU2.TOER.BIT.OE4B = 0; // TIOC4B
    MTU2.TOER.BIT.OE4D = 0; // TIOC4D
}

void update_cpwm_duty(int tgr_u, int tgr_v, int tgr_w)
{
    MTU23.TGRD = tgr_u;
    MTU24.TGRC = tgr_v;
    MTU24.TGRD = tgr_w;
}