#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include <idtLoader.h>

void _syscallHandler(void);

void _irq00Handler(void);

void _irq01Handler(void);

void _irq02Handler(void);

void _irq03Handler(void);

void _irq04Handler(void);

void _irq05Handler(void);

void _exception0Handler(void);

void _exception6Handler(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void _forceInt(void);

int _xchg(int *lock, int value);

int _xadd(int *lock, int value);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

//Termina la ejecución de la cpu.
void haltcpu(void);

void saveInitRegs(uint64_t rip);

#endif /* INTERRUPS_H_ */