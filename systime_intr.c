#include "iodefine.h"
#include "systime_intr.h"

unsigned long _systime = 0;  // �V�X�e���^�C�}
unsigned long _time_div = 0; // �^�C�}����\

/*
 * �V�X�e���^�C�}������
 * t [ms] �� 1�J�E���g
 * �� �Q��
 * http://project12513.blog-fps.com/sh%E3%82%BD%E3%83%95%E3%83%88%E3%82%A6%E3%82%A7%E3%82%A2%E9%96%8B%E7%99%BA/%E3%82%BF%E3%82%A4%E3%83%9E%E3%83%BC%E5%89%B2%E8%BE%BC%E6%A9%9F%E8%83%BD%E3%82%92%E8%A9%A6%E3%81%99
 */
void init_CMT0_intr(int t)
{
    long int LN;

    //	CMT0�̊��荞�݃��x���i�D�揇�ʁj��ݒ�
    INTC.IPRJ.BIT._CMT0 = 0xA; // ��(0x0:�Œ�`0xF:�ō�)

    STB.CR4.BIT._CMT = 0; // CMT���W���[���̃X�^���o�C����

    //�ݒ蒆�Ȃ̂ŃJ�E���^�[�ꎞ��~
    CMT.CMSTR.BIT.STR0 = 0; // 0�F�J�E���g��~, 1�F�J�E���g�J�n

    CMT0.CMCSR.BIT.CMIE = 1; // �R���y�A�}�b�`���荞�݋��@��1�Ŋ��荞�݂��s��
    CMT0.CMCSR.BIT.CKS = 2;  // �N���b�N�Z���N�g[1,0]  10=>2�FP��/128

    //CMCOR�̒l���Z�o	T�~���b��LN�J�E���g
    LN = 195312 * t / 1000; // 25MHz/128=195312Hz(1�J�E���g5.12��s)
    if (LN < 1)
        LN = 1;
    else if (LN > 0xffff)
        LN = 0xffff; //335ms�ɑ���
    _time_div = LN;

    CMT0.CMCNT = 0;         // �J�E���^���Z�b�g
    CMT0.CMCOR = (int)LN;   // �J�E���g�ڕW�l�Z�b�g
    CMT.CMSTR.BIT.STR0 = 1; // CMT0�J�E���g�J�n
}

void CMT0_INT_update_systime(void)
{
    _systime++;
}

unsigned long get_systime(void)
{
    return _systime;
}
