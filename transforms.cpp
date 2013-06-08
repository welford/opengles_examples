#include "transforms.h"
//-------------------------------------------------
// Transforms
//
//
//-------------------------------------------------

Transform::Transform(unsigned int depth): m_transformStack(depth)
{
	m_transformStack.TopOfStack()->Identity();
}

Transform::Transform( ): m_transformStack(1)
{
	m_transformStack.TopOfStack()->Identity();
}

Transform::~Transform()
{
	
}

void Transform::ResizeStack(unsigned int depth)
{
	m_transformStack.Resize(depth);
}

void Transform::Push()
{ 
	m_transformStack.Push();	
}

void Transform::Pop()
{ 
	m_transformStack.Pop();	
}

CMatrix44 Transform::GetCurrentMatrix()
{
	return *m_transformStack.TopOfStack();
}

void Transform::Identity(void)
{
	m_transformStack.TopOfStack()->Identity();
}

void  Transform::Translate(const float x,const float y,const float z)
{
	m_transformStack.TopOfStack()->Translate(CVec3df(x,y,z));
}

void  Transform::Translate(const Vec3df& vec)
{
	m_transformStack.TopOfStack()->Translate(vec);
}

void  Transform::Scale(const float x,const float y,const float z)
{
	m_transformStack.TopOfStack()->Scale(x,y,z);		
}

void Transform::ApplyTransform(const CMatrix44& matrix)
{
	*m_transformStack.TopOfStack() = *m_transformStack.TopOfStack() * matrix;
}
/*

void Transform::CalcBasicShadowMtx(unsigned int idx, const CMatrix44& proj,const CMatrix44& modelview)
{
	if(idx >= m_nShadows)
		return;

	//for(unsigned int i=0; i< m_nShadows; i++)
	//{
		m_pShadowStack[idx].TopOfStack()->Identity();
		m_pShadowStack[idx].TopOfStack()->Translate(CVec3df(0.5));
		m_pShadowStack[idx].TopOfStack()->Scale(0.5f, 0.5f, 0.5f);
		(*m_pShadowStack[idx].TopOfStack()) = (*m_pShadowStack[idx].TopOfStack()) * proj;
		(*m_pShadowStack[idx].TopOfStack()) = (*m_pShadowStack[idx].TopOfStack()) * modelview;	
	//}
}
//*/

void Transform::CreateProjectionMatrix(Matrix44& mat, float l,float r,float b,float t,float n,float f)
{
	Matrix44 proj = {
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
	mat = proj;
}

void Transform::CreateInverseProjectionMatrix(Matrix44& mat, float l,float r,float b,float t,float n,float f)
{
	Matrix44 proj = {
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
	mat = proj;
}

void Transform::CreateOrthoMatrix(Matrix44& mat, float l,float r,float b,float t,float n,float f)
{
	Matrix44 proj = {
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
	mat = proj;
}