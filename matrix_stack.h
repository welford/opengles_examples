#ifndef _MATRIX_STACK_H_
#define _MATRIX_STACK_H_

#include "matrix44.h"

typedef struct _SMatrixStack
{
	unsigned int	m_index;
	unsigned int	m_size;
	SMatrix44*		m_paStack;	
}SMatrixStack;

void MatrixStackInitialise(SMatrixStack* const pStack, unsigned int size);
void MatrixStackDestroy(SMatrixStack* const pStack);
void MatrixStackResize(SMatrixStack* const pStack, unsigned int size);
void MatrixStackPush(SMatrixStack* const pStack);
void MatrixStackPop(SMatrixStack* const pStack); 
SMatrix44* MatrixStackTop(SMatrixStack* const pStack);

#endif// _MATRIX_STACK_H_