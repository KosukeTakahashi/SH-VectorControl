#ifndef CPWM_H
#define CPWM_H

void init_cpwm(int carrier_cycle, int deadtime, int init_tgr_u, int init_tgr_v, int init_tgr_w);
void start_cpwm(void);
void stop_cpwm(void);
void update_cpwm_duty(int tgr_u, int tgr_v, int tgr_w);

#endif