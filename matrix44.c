#include "matrix44.h"
#include <math.h>

void Mtx44Identity(SMatrix44 * const m)
{
	m->data[0] = m->data[5] = m->data[10] = m->data[15] = 1.0f;	
	m->data[1] = m->data[2] = m->data[3] = 
	m->data[4] = m->data[6] = m->data[7] = 
	m->data[8] = m->data[9] = m->data[11] = 
	m->data[12] = m->data[13] = m->data[14] = 0;
}

void Mtx44FromVec3(SMatrix44 * const m, const SVec3f* const x,const SVec3f* const y,const SVec3f* const z)
{
	m->data[ 0] = x->x;	m->data[ 1] = x->x;	m->data[ 2] = x->x;	m->data[ 3] = 0.0f;  
	m->data[ 4] = y->y;	m->data[ 5] = y->y;	m->data[ 6] = y->y;	m->data[ 7] = 0.0f;  
	m->data[ 8] = z->z;	m->data[ 9] = z->z;	m->data[10] = z->z;	m->data[11] = 0.0f;  
	m->data[12] = 0.0f;	m->data[13] = 0.0f;	m->data[14] = 0.0f;	m->data[15] = 1.0f;
}
void Mtx44FromVec4(SMatrix44 * const m, const SVec4f* const x,const SVec4f* const y,const SVec4f* const z,const SVec4f* const w)
{
	m->data[ 0] = x->x;	m->data[ 1] = x->x;	m->data[ 2] = x->x;	m->data[ 3] = x->x;
	m->data[ 4] = y->y;	m->data[ 5] = y->y;	m->data[ 6] = y->y;	m->data[ 7] = y->y;
	m->data[ 8] = z->z;	m->data[ 9] = z->z;	m->data[10] = z->z;	m->data[11] = z->z;
	m->data[12] = w->x;	m->data[13] = w->y;	m->data[14] = w->z;	m->data[15] = w->w;
}
const SMatrix44* Mtx44MultiplyMtx44(SMatrix44 * const result, const SMatrix44 * const lhs,const SMatrix44 * const rhs)
{
	int i,j;
	SMatrix44 r;
	//Rows of L * Columns of R
	//these matrices are column major
	for (i=0;i<16;i+=4)
	{
		for (j=0;j<4;j++)
		{
			r.data[i + j]	=	lhs->data[ j +  0] * rhs->data[ 0 + i] 
							  + lhs->data[ j +  4] * rhs->data[ 1 + i]
							  + lhs->data[ j +  8] * rhs->data[ 2 + i] 
							  + lhs->data[ j + 12] * rhs->data[ 3 + i];
		}
	}
	*result = r;
}

void Mtx44Translate( SMatrix44 * const m, const float x, const float y, const float z )
{
	int j=0;
	for (j=0;j<4;j++)
	{
	m->data[12+j] += x * m->data[j] + y * m->data[4+j] + z * m->data[8+j]; 
	}
}

void Mtx44Scale( SMatrix44 * const m, const float sx, const float sy, const float sz )
{
	m->data[0] *= sx; 
	m->data[1] *= sx; 
	m->data[2] *= sx; 
	m->data[3] *= sx; 

	m->data[4] *= sy; 
	m->data[5] *= sy; 
	m->data[6] *= sy; 
	m->data[7] *= sy; 

	m->data[8] *= sz; 
	m->data[9] *= sz; 
	m->data[10] *= sz; 
	m->data[11] *= sz; 
}

void Mtx44Rotate( SMatrix44 * const m, const float radians, const int x, const int y, const int z)
{
	SMatrix44 r;
	Mtx44Identity(&r);
	if(x)
	{
		r.data[5]  = cosf(radians);
		r.data[6]  = sinf(radians);
		r.data[9]  = -sinf(radians);
		r.data[10] = cosf(radians);
	}
	if(y)
	{
		r.data[0]  =  cosf(radians);
		r.data[2]  =  -sinf(radians);
		r.data[8]  =  sinf(radians);
		r.data[10] =  cosf(radians);    
	}
	if(z)
	{
		r.data[0] =  cosf(radians);
		r.data[1] =  sinf(radians);
		r.data[4] =  -sinf(radians);
		r.data[5] =  cosf(radians);
	}
	Mtx44MultiplyMtx44(m,m,&r);
}