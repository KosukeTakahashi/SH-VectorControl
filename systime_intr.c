#include "iodefine.h"
#include "systime_intr.h"

unsigned long _systime = 0;  // システムタイマ
unsigned long _time_div = 0; // タイマ分解能

/*
 * システムタイマ初期化
 * t [ms] で 1カウント
 * ↓ 参照
 * http://project12513.blog-fps.com/sh%E3%82%BD%E3%83%95%E3%83%88%E3%82%A6%E3%82%A7%E3%82%A2%E9%96%8B%E7%99%BA/%E3%82%BF%E3%82%A4%E3%83%9E%E3%83%BC%E5%89%B2%E8%BE%BC%E6%A9%9F%E8%83%BD%E3%82%92%E8%A9%A6%E3%81%99
 */
void init_CMT0_intr(int t)
{
    long int LN;

    //	CMT0の割り込みレベル（優先順位）を設定
    INTC.IPRJ.BIT._CMT0 = 0xA; // ※(0x0:最低～0xF:最高)

    STB.CR4.BIT._CMT = 0; // CMTモジュールのスタンバイ解除

    //設定中なのでカウンター一時停止
    CMT.CMSTR.BIT.STR0 = 0; // 0：カウント停止, 1：カウント開始

    CMT0.CMCSR.BIT.CMIE = 1; // コンペアマッチ割り込み許可　※1で割り込みを行う
    CMT0.CMCSR.BIT.CKS = 2;  // クロックセレクト[1,0]  10=>2：Pφ/128

    //CMCORの値を算出	Tミリ秒→LNカウント
    LN = 195312 * t / 1000; // 25MHz/128=195312Hz(1カウント5.12μs)
    if (LN < 1)
        LN = 1;
    else if (LN > 0xffff)
        LN = 0xffff; //335msに相当
    _time_div = LN;

    CMT0.CMCNT = 0;         // カウンタリセット
    CMT0.CMCOR = (int)LN;   // カウント目標値セット
    CMT.CMSTR.BIT.STR0 = 1; // CMT0カウント開始
}

void CMT0_INT_update_systime(void)
{
    _systime++;
}

unsigned long get_systime(void)
{
    return _systime;
}
