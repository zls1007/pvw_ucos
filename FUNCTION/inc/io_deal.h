#ifndef __IO_DEAL_H
#define __IO_DEAL_H
#include "includes.h"
#include "param.h"

typedef union
{
	u8 bit[4];
	float f;
} Float_Type;

void IOMsgAnalysis(CPU_INT08U buf[], u8 size);
void IOMsgSend(u8 d1, u8 d2);

#endif
