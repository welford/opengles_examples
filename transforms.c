#include "transforms.h"
//-------------------------------------------------
// Transforms
//
//
//-------------------------------------------------
void TransformInitialise(STransform* const pTransform, unsigned int depth)
{
	MatrixStackInitialise(&pTransform->m_transformStack, depth);
	TransformIdentity(pTransform);
}
void TransformDestroy(STransform* const pTransform)
{
	MatrixStackDestroy(&pTransform->m_transformStack);
}

void TransformResizeStack(STransform* const pTransform, unsigned int depth)
{
	MatrixStackResize(&pTransform->m_transformStack, depth);
}

void TransformPush(STransform*const pTransform)
{
	MatrixStackPush(&pTransform->m_transformStack);
}
void TransformPop(STransform*const pTransform)
{
	MatrixStackPop(&pTransform->m_transformStack); 
}
SMatrix44* TransformGetCurrentMatrix(STransform*const pTransform)
{
	return MatrixStackTop(&pTransform->m_transformStack);
}
void TransformIdentity(STransform*const pTransform)
{
	Mtx44Identity(MatrixStackTop(&pTransform->m_transformStack));
}

void TransformTranslate(STransform*const pTransform, const float x,const float y,const float z)
{
	Mtx44Translate(MatrixStackTop(&pTransform->m_transformStack), x, y, z);
}

void TransformScale(STransform*const pTransform, const float x,const float y,const float z)
{
	Mtx44Scale(MatrixStackTop(&pTransform->m_transformStack), x, y, z);
}

void TransformMultiply(STransform*const pTransform, const SMatrix44* const mtx)
{
	Mtx44MultiplyMtx44(MatrixStackTop(&pTransform->m_transformStack), MatrixStackTop(&pTransform->m_transformStack), mtx);
}

void CreateProjectionMatrix(SMatrix44* const mat, float l,float r,float b,float t,float n,float f)
{
	SMatrix44 proj = {
		//col 1
		(2*n)/(r-l),
		0,
		0,
		0,
		//col 2
		0,
		(2*n)/(t-b),
		0,
		0,
		//col 3
		(r+l)/(r-l),
		(t+b)/(t-b),
		-(f+n)/(f-n),
		-1,
		//col 4
		0,
		0,
		-(2*f*n)/(f-n),
		0
	};
	*mat = proj;
}

void CreateInverseProjectionMatrix(SMatrix44* const mat, float l,float r,float b,float t,float n,float f)
{
	SMatrix44 proj = {
		//col 1
		(r-l)/(2*n),
		0,
		0,
		0,
		//col 2
		0,
		(t-b)/(2*n),
		0,
		0,
		//col 3
		0,
		0,
		0,
		-(f-n)/(2*f*n),
		//col4
		(r+l)/(2*n),
		(t+b)/(2*n),
		-1,
		(f+n)/(2*f*n)
	};
	*mat = proj;
}

void CreateOrthoMatrix(SMatrix44* const mat, float l,float r,float b,float t,float n,float f)
{
	SMatrix44 proj = {
		//col 1
		2/(r-l),
		0,
		0,
		0,
		//col 2
		0,
		2/(t-b),
		0,
		0,
		//col 3
		0,
		0,
		-2/(f-n),
		0,
		//col 4
		-(r+l)/(r-l),
		-(t+b)/(t-b),
		-(f+n)/(f-n),
		1
	};
	*mat = proj;
}