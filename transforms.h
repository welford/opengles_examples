#include "matrix_stack.h"

#ifndef _TRANSFORMS_H_
#define _TRANSFORMS_H_

typedef struct _STransform
{
	SMatrixStack m_transformStack;
}STransform;

void TransformInitialise(STransform* const, unsigned int depth);
void TransformDestroy(STransform* const);
void TransformResizeStack(STransform* const, unsigned int depth);
void TransformPush(STransform*const);
void TransformPop(STransform*const);
SMatrix44* TransformGetCurrentMatrix(STransform*const);

void TransformIdentity(STransform*const);
void TransformTranslate(STransform*const, const float x,const float y,const float z);
void TransformScale(STransform*const, const float x,const float y,const float z);
void TransformMultiply(STransform*const, const SMatrix44* const mtx);	

void CreateProjectionMatrix(SMatrix44* const mat, float l,float r,float b,float t,float n,float f);
void CreateInverseProjectionMatrix(SMatrix44* const mat, float l,float r,float b,float t,float n,float f);
void CreateOrthoMatrix(SMatrix44* const mat, float l,float r,float b,float t,float n,float f);

/*
class Transform
{
private:
	CMatrixStack m_transformStack;
public:
	Transform(unsigned int depth);
	Transform(void);
	virtual ~Transform();
	void ResizeStack(unsigned int depth);

	void Push();
	void Pop();
	CMatrix44 GetCurrentMatrix();
	
	void Identity(void);
	void Translate(const float x,const float y,const float z);
	void Translate(const Vec3df& vec);
	void Scale(const float x,const float y,const float z);
	void ApplyTransform(const CMatrix44& mtx);	
	
	static void CreateProjectionMatrix(Matrix44& mat, float l,float r,float b,float t,float n,float f);
	static void CreateInverseProjectionMatrix(Matrix44& mat, float l,float r,float b,float t,float n,float f);
	static void CreateOrthoMatrix(Matrix44& mat, float l,float r,float b,float t,float n,float f);
};
*/
#endif //_TRANSFORMS_H_