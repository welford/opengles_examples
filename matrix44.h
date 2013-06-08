#ifndef _MATRIX44_H_
#define _MATRIX44_H_

#define PI_I_44 (3.14159265f)
#define TO_RAD_44 (PI_I_44/180.0f)

static float Determinant4f(const float m[16])
{
	return
	m[12]*m[9]*m[6]*m[3]-
	m[8]*m[13]*m[6]*m[3]-
	m[12]*m[5]*m[10]*m[3]+
	m[4]*m[13]*m[10]*m[3]+
	m[8]*m[5]*m[14]*m[3]-
	m[4]*m[9]*m[14]*m[3]-
	m[12]*m[9]*m[2]*m[7]+
	m[8]*m[13]*m[2]*m[7]+
	m[12]*m[1]*m[10]*m[7]-
	m[0]*m[13]*m[10]*m[7]-
	m[8]*m[1]*m[14]*m[7]+
	m[0]*m[9]*m[14]*m[7]+
	m[12]*m[5]*m[2]*m[11]-
	m[4]*m[13]*m[2]*m[11]-
	m[12]*m[1]*m[6]*m[11]+
	m[0]*m[13]*m[6]*m[11]+
	m[4]*m[1]*m[14]*m[11]-
	m[0]*m[5]*m[14]*m[11]-
	m[8]*m[5]*m[2]*m[15]+
	m[4]*m[9]*m[2]*m[15]+
	m[8]*m[1]*m[6]*m[15]-
	m[0]*m[9]*m[6]*m[15]-
	m[4]*m[1]*m[10]*m[15]+
	m[0]*m[5]*m[10]*m[15];
}

static int GenerateInverseMatrix4f(float i[16], const float m[16])
{
	float x=Determinant4f(m);
	if (x==0) return 0;

	i[0]= (-m[13]*m[10]*m[7] +m[9]*m[14]*m[7] +m[13]*m[6]*m[11]
	-m[5]*m[14]*m[11] -m[9]*m[6]*m[15] +m[5]*m[10]*m[15])/x;
	i[4]= ( m[12]*m[10]*m[7] -m[8]*m[14]*m[7] -m[12]*m[6]*m[11]
	+m[4]*m[14]*m[11] +m[8]*m[6]*m[15] -m[4]*m[10]*m[15])/x;
	i[8]= (-m[12]*m[9]* m[7] +m[8]*m[13]*m[7] +m[12]*m[5]*m[11]
	-m[4]*m[13]*m[11] -m[8]*m[5]*m[15] +m[4]*m[9]* m[15])/x;
	i[12]=( m[12]*m[9]* m[6] -m[8]*m[13]*m[6] -m[12]*m[5]*m[10]
	+m[4]*m[13]*m[10] +m[8]*m[5]*m[14] -m[4]*m[9]* m[14])/x;
	i[1]= ( m[13]*m[10]*m[3] -m[9]*m[14]*m[3] -m[13]*m[2]*m[11]
	+m[1]*m[14]*m[11] +m[9]*m[2]*m[15] -m[1]*m[10]*m[15])/x;
	i[5]= (-m[12]*m[10]*m[3] +m[8]*m[14]*m[3] +m[12]*m[2]*m[11]
	-m[0]*m[14]*m[11] -m[8]*m[2]*m[15] +m[0]*m[10]*m[15])/x;
	i[9]= ( m[12]*m[9]* m[3] -m[8]*m[13]*m[3] -m[12]*m[1]*m[11]
	+m[0]*m[13]*m[11] +m[8]*m[1]*m[15] -m[0]*m[9]* m[15])/x;
	i[13]=(-m[12]*m[9]* m[2] +m[8]*m[13]*m[2] +m[12]*m[1]*m[10]
	-m[0]*m[13]*m[10] -m[8]*m[1]*m[14] +m[0]*m[9]* m[14])/x;
	i[2]= (-m[13]*m[6]* m[3] +m[5]*m[14]*m[3] +m[13]*m[2]*m[7]
	-m[1]*m[14]*m[7] -m[5]*m[2]*m[15] +m[1]*m[6]* m[15])/x;
	i[6]= ( m[12]*m[6]* m[3] -m[4]*m[14]*m[3] -m[12]*m[2]*m[7]
	+m[0]*m[14]*m[7] +m[4]*m[2]*m[15] -m[0]*m[6]* m[15])/x;
	i[10]=(-m[12]*m[5]* m[3] +m[4]*m[13]*m[3] +m[12]*m[1]*m[7]
	-m[0]*m[13]*m[7] -m[4]*m[1]*m[15] +m[0]*m[5]* m[15])/x;
	i[14]=( m[12]*m[5]* m[2] -m[4]*m[13]*m[2] -m[12]*m[1]*m[6]
	+m[0]*m[13]*m[6] +m[4]*m[1]*m[14] -m[0]*m[5]* m[14])/x;
	i[3]= ( m[9]* m[6]* m[3] -m[5]*m[10]*m[3] -m[9]* m[2]*m[7]
	+m[1]*m[10]*m[7] +m[5]*m[2]*m[11] -m[1]*m[6]* m[11])/x;
	i[7]= (-m[8]* m[6]* m[3] +m[4]*m[10]*m[3] +m[8]* m[2]*m[7]
	-m[0]*m[10]*m[7] -m[4]*m[2]*m[11] +m[0]*m[6]* m[11])/x;
	i[11]=( m[8]* m[5]* m[3] -m[4]*m[9]* m[3] -m[8]* m[1]*m[7]
	+m[0]*m[9]* m[7] +m[4]*m[1]*m[11] -m[0]*m[5]* m[11])/x;
	i[15]=(-m[8]* m[5]* m[2] +m[4]*m[9]* m[2] +m[8]* m[1]*m[6]
	-m[0]*m[9]* m[6] -m[4]*m[1]*m[10] +m[0]*m[5]* m[10])/x;
	return 1;
} 


typedef struct _SVec3f
{
	union
	{
		float data[4];
		struct
		{
			float x,y,z,w;
		};
	};		
}SVec3f;

typedef struct _SVec4f
{
	union
	{
		float data[4];
		struct
		{
			float x,y,z,w;
		};
		struct
		{
			SVec3f v;
			float _w;
		};
	};	
	/*
	bool operator==(const Vec4df&rhs)
	{
		if(data[0] == rhs.data[0] && data[1] == rhs.data[1]
		&& data[2] == rhs.data[2] && data[3] == rhs.data[3])
			return true;

		return false;
	}
	Vec4df& operator+=(const Vec4df&rhs)
	{
		data[0] += rhs.data[0];
		data[1] += rhs.data[1];
		data[2] += rhs.data[2];
		data[3] += rhs.data[3];		
		return *this;
	}
	Vec4df& operator*=(const Vec4df&rhs)
	{
		data[0] *= rhs.data[0];
		data[1] *= rhs.data[1];
		data[2] *= rhs.data[2];
		data[3] *= rhs.data[3];
		return *this;		
	}
	
	Vec4df(){x=y=z=t=0;}
	Vec4df(float value){x=y=z=t=value;}
	Vec4df(float vx, float vy, float vz, float vt){x=vx;y=vy;z=vz;t=vt;}
	*/
}SVec4f;

typedef struct _SMatrix44
{
	union
	{
		float data[16];
		struct
		{
			float a[4][4];
		};
		struct
		{
			SVec4f x,y,z,t;
		};
		struct
		{
			SVec4f vec[4];
		};
		struct
		{
			float	m11,m12,m13,m14,
					m21,m22,m23,m24,
					m31,m32,m33,m34,
					m41,m42,m43,m44;
		};
	};	
	/*
	bool operator ==(const _Matrix44& rhs)const
	{
		return (data[0]==rhs.data[0] && data[2]==rhs.data[2] && data[3]==rhs.data[3] &&
				data[4]==rhs.data[4] && data[5]==rhs.data[5] && data[6]==rhs.data[6] &&
				data[7]==rhs.data[7] && data[8]==rhs.data[8] && data[9]==rhs.data[9] &&
				data[10]==rhs.data[10] && data[11]==rhs.data[11] && data[12]==rhs.data[12] &&
				data[13]==rhs.data[13] && data[14]==rhs.data[14] && data[15]==rhs.data[15]);
	}
	*/
}SMatrix44;

void Mtx44Identity(SMatrix44 * const);
void Mtx44FromVec3(SMatrix44 * const, const SVec3f* const x,const SVec3f* const y,const SVec3f* const z);
void Mtx44FromVec4(SMatrix44 * const, const SVec4f* const x,const SVec4f* const y,const SVec4f* const z,const SVec4f* const w);
const SMatrix44 * Mtx44MultiplyMtx44(SMatrix44 * const result, const SMatrix44 * const lhs,const SMatrix44 * const rhs);

void Mtx44Translate( SMatrix44 * const m, const float x, const float y, const float z );
void Mtx44Scale( SMatrix44 * const m, const float sx, const float sy, const float sz );
void Mtx44Rotate( SMatrix44 * const m, const float radians, const int x, const int y, const int z);

//void Mtx44MultiplyVec3(SMatrix44 * const r, const SMatrix44 * const l,const SMatrix44 * const r);

/*

class CMatrix44 : public Matrix44
{
public : 
	enum VEC
	{
		COL_X,COL_Y,COL_Z,COL_T,
		ROW_X,ROW_Y,ROW_Z,ROW_T,
	};
	
	// Functions
	CMatrix44( const int bIdentity = 1)
	{
		if ( bIdentity ) Identity();
	}

	CMatrix44(CVec3df& x,CVec3df& y,CVec3df& z)
	{
		data[ 0] = x.x;    data[ 1] = y.x;      data[ 2] = z.x;    data[ 3] = 0.0f;  
		data[ 4] = x.y;    data[ 5] = y.y;      data[ 6] = z.y;    data[ 7] = 0.0f;  
		data[ 8] = x.z;    data[ 9] = y.z;      data[10] = z.z;    data[11] = 0.0f;  
		data[12] = 0.0f;   data[13] = 0.0f;      data[14] = 0.0f;  data[15] = 1.0f;
	}

	void Identity( )
	{
		data[ 0] = 1.0f;    data[ 1] = 0.0f;      data[ 2] = 0.0f;    data[ 3] = 0.0f;  
		data[ 4] = 0.0f;    data[ 5] = 1.0f;      data[ 6] = 0.0f;    data[ 7] = 0.0f;  
		data[ 8] = 0.0f;    data[ 9] = 0.0f;      data[10] = 1.0f;    data[11] = 0.0f;  
		data[12] = 0.0f;    data[13] = 0.0f;      data[14] = 0.0f;    data[15] = 1.0f;
	}

	inline float operator() ( int col, int row) const
	{
		return this->data[(col*3)+row]; //IS THIS OK?
	}

	// Concatenate 2 matrices with the * operator
	inline CMatrix44 operator* (const Matrix44 &rhs) const
	{
		CMatrix44 result( 0 );
		for (int i=0;i<16;i+=4)
		{
			for (int j=0;j<4;j++)
			{
				result.data[i + j]	= data[ j +  0] * rhs.data[ 0 + i] 
									+ data[ j +  4] * rhs.data[ 1 + i]
									+ data[ j +  8] * rhs.data[ 2 + i] 
									+ data[ j + 12] * rhs.data[ 3 + i];
			}
		}
		return result;
	}

	// Use a matrix to transform a 3D point with the * operator
	inline CVec3df operator* (const CVec3df &Point ) const
	{
		float x = Point.x*data[0] + Point.y*data[4] + Point.z*data[8]  + data[12];
		float y = Point.x*data[1] + Point.y*data[5] + Point.z*data[9]  + data[13];
		float z = Point.x*data[2] + Point.y*data[6] + Point.z*data[10] + data[14]; 
		return CVec3df( x, y, z );
	}

	// Use a matrix to transform a 3D point with the * operator
	inline Vec4df operator* (const Vec4df &Point ) const
	{
		float x = Point.x*data[0] + Point.y*data[4] + Point.z*data[8]  + Point.t*data[12];
		float y = Point.x*data[1] + Point.y*data[5] + Point.z*data[9]  + Point.t*data[13];
		float z = Point.x*data[2] + Point.y*data[6] + Point.z*data[10] + Point.t*data[14]; 
		float t = Point.x*data[3] + Point.y*data[7] + Point.z*data[11] + Point.t*data[15]; 
		return Vec4df( x, y, z, t );
	}

	// Rotate the *this matrix fDegrees counter-clockwise around a single axis( either x, y, or z )
	void Rotate( float fDegrees, int x, int y, int z )
	{
		CMatrix44 Temp;
		if (x == 1) Temp.RotX( fDegrees );
		if (y == 1) Temp.RotY( fDegrees );
		if (z == 1) Temp.RotZ( fDegrees );
		*this = Temp * (*this);
	}

	// Rotate the *this matrix fDegrees counter-clockwise around a single axis( either x, y, or z )
	void SetRotationPart(float rot[9])
	{
		data[0] = rot[0];	data[1] = rot[1];	data[2] = rot[2];
		data[4] = rot[3];	data[5] = rot[4];	data[6] = rot[5];
		data[8] = rot[6];	data[9] = rot[7];	data[10] = rot[8];
	}

	void Scale( float sx, float sy, float sz )
	{
		int x;
		for (x = 0; x <  4; x++) data[x]*=sx;
		for (x = 4; x <  8; x++) data[x]*=sy;
		for (x = 8; x < 12; x++) data[x]*=sz;
	}

	//translate within matrix space
	void Translate( const Vec3df &Test )
	{
		for (int j=0;j<4;j++)
		{
		data[12+j] += Test.x * data[j] + Test.y * data[4+j] + Test.z * data[8+j]; 
		}	 
	}

	CVec3df GetTranslate( )
	{
		return CVec3df( data[12], data[13], data[14] );
	}

	//OpenGL is Coloumn-Major:
	//  |  X   Y   Z   T
 	// x| m0  m4  m8  m12 
	// y| m1  m5  m9  m13
	// z| m2  m6  m10 m14
	// t| m3  m7  m11 m15
	CVec3df  GetVec( VEC vec)
	{
		switch (vec)
		{
		default:
		case COL_X:
			return CVec3df( data[0], data[1], data[2] );
			break;
		case COL_Y:
			return CVec3df( data[4], data[5], data[6] );
			break;
		case COL_Z:
			return CVec3df( data[8], data[9], data[10] );
			break;
		case COL_T:
			return CVec3df( data[12], data[13], data[14] );
			break;
		case ROW_X:
			return CVec3df( data[0], data[4], data[8] );
			break;
		case ROW_Y:
			return CVec3df( data[1], data[5], data[9] );
			break;
		case ROW_Z:
			return CVec3df( data[2], data[6], data[10] );
			break;
		case ROW_T:
			return CVec3df( data[3], data[7], data[11] );
			break;
		}
	}

	Vec4df  GetVec4( VEC vec)
	{
		Vec4df temp;

		switch (vec)
		{
		default:
		case COL_X:
			return x;
			break;
		case COL_Y:
			return y;
			break;
		case COL_Z:
			return z;
			break;
		case COL_T:
			return t;
			break;
		case ROW_X:			
			temp.x = data[0]; temp.y = data[4]; temp.z = data[8]; temp.t = data[12];
			return temp;
			break;
		case ROW_Y:
			temp.x = data[1]; temp.y = data[5]; temp.z = data[9]; temp.t = data[13];
			return temp;
			break;
		case ROW_Z:
			temp.x = data[2]; temp.y = data[6]; temp.z = data[10]; temp.t = data[14];
			return temp;
			break;
		case ROW_T:
			temp.x = data[3]; temp.y = data[7]; temp.z = data[11]; temp.t = data[15];
			return temp;
			break;
		}
	}

	// Zero out the translation part of the matrix
	CMatrix44 RotationOnly()
	{
		CMatrix44 Temp = *this;
		
		Temp.data[12] = 0;
		Temp.data[13] = 0;
		Temp.data[14] = 0;

		Temp.data[3] = 0;
		Temp.data[7] = 0;
		Temp.data[11] = 0;
		
		return Temp;
	}
	

	// Create a rotation matrix for a counter-clockwise rotation of fDegrees around an arbitrary axis(x, y, z)
	void RotateMatrix( float fDegrees, float x, float y, float z)
	{
		Identity();
		float cosA = cosf(fDegrees*TO_RAD_44);
		float sinA = sinf(fDegrees*TO_RAD_44);

		float m = 1.0f - cosA;
		data[0] = cosA + x*x*m;
		data[5] = cosA + y*y*m;
		data[10]= cosA + z*z*m;

		float tmp1 = x*y*m;
		float tmp2 = z*sinA;
		data[4] = tmp1 + tmp2;
		data[1] = tmp1 - tmp2;

		tmp1 = x*z*m;
		tmp2 = y*sinA;
		data[8] = tmp1 - tmp2;
		data[2] = tmp1 + tmp2;

		tmp1 = y*z*m;
		tmp2 = x*sinA;
		data[9] = tmp1 + tmp2;
		data[6] = tmp1 - tmp2;
	}

	// Simple but not robust matrix inversion. (Doesn't work properly if there is a scaling or skewing transformation.)
	inline CMatrix44 InvertSimple()
	{
		CMatrix44 R(0);
		GenerateInverseMatrix4f(R.data,data);		
		return R;
	}

	// Invert for only a rotation, any translation is zeroed out
	CMatrix44 InvertRot( )
	{
		CMatrix44 R( 0 );
		R.data[0]  = data[0]; 	R.data[1]  = data[4];		R.data[2]  = data[8];	R.data[3]  = 0.0f;
		R.data[4]  = data[1];		R.data[5]  = data[5];		R.data[6]  = data[9];	R.data[7]  = 0.0f;
		R.data[8]  = data[2];		R.data[9]  = data[6];		R.data[10] = data[10];R.data[11] = 0.0f;
		R.data[12] = 0;			R.data[13] = 0;			R.data[14] = 0;		R.data[15] = 1.0f;
		return R;
	}
	//inverts the rotation matrix as a transform
	//i.e. transposes the rotation
	//inverts the translation
	CMatrix44 InvertTransform( )
	{
		CMatrix44 R( 0 );
		R.data[0]  = data[0]; 	R.data[1]  = data[4];	R.data[2]  = data[8];	R.data[3]  = 0.0f;
		R.data[4]  = data[1];	R.data[5]  = data[5];	R.data[6]  = data[9];	R.data[7]  = 0.0f;
		R.data[8]  = data[2];	R.data[9]  = data[6];	R.data[10] = data[10];	R.data[11] = 0.0f;
		R.data[12] = -R.data[12];
		R.data[13] = -R.data[13];
		R.data[14] = -R.data[14];
		R.data[15] = 1.0f;
		return R;
	}
private:
	// helpers for Rotate, all anticlockwise
	void RotX(float angle)
	{  
		data[5]  = cosf(angle*TO_RAD_44);
		data[6]  = sinf(angle*TO_RAD_44);
		data[9]  = -sinf(angle*TO_RAD_44);
		data[10] = cosf(angle*TO_RAD_44);
	}

	void RotY(float angle)
	{
		data[0]  =  cosf(angle*TO_RAD_44);
		data[2]  =  -sinf(angle*TO_RAD_44);
		data[8]  =  sinf(angle*TO_RAD_44);
		data[10] =  cosf(angle*TO_RAD_44);    
	}

	void RotZ(float angle)
	{
		data[0] =  cosf(angle*TO_RAD_44);
		data[1] =  sinf(angle*TO_RAD_44);
		data[4] =  -sinf(angle*TO_RAD_44);
		data[5] =  cosf(angle*TO_RAD_44);
	}
};
*/
#endif //_MATRIX44_H_	
