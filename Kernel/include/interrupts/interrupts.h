#include <stdint.h>
#ifndef INTERRUPS_H_
#define INTERRUPS_H_


void _irq00Handler ( void );
void _irq01Handler ( void );
void _irq02Handler ( void );
void _irq03Handler ( void );
void _irq04Handler ( void );
void _irq05Handler ( void );

void _irq80Handler ( void );


void _exception0Handler ( void );
void _exception6Handler ( void );
void _exception13Handler ( void );

void _cli ( void );

void _sti ( void );

void _hlt ( void );

void pic_master_mask ( uint8_t mask );

void pic_slave_mask ( uint8_t mask );

void haltcpu ( void );

void timer_tick ( void );

void yield ( void );

#endif