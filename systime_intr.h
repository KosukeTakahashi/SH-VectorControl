#ifndef SYSTIME_INTR_H
#define SYSTIME_INTR_H

void init_CMT0_intr(int t);
void CMT0_INT_update_systime(void);
unsigned long get_systime(void);

#endif
