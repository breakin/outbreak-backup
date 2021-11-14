#ifndef __ETERNITY_VECTOR_INC__
#define __ETERNITY_VECTOR_INC__

#include "..\e3d_types.h"
#include "..\e3d_sysdef.h"
#include <math.h>

namespace Eternity {
	
	/**
	 * [eTernity 3D Engine]
	 * ====================
	 * @class	CVector3d
	 * @brief	3D Vector mathematics class
	 * @author	Peter Nordlander
	 * @date	2001-05-23
	 */
	
	// forw. declarations
	class CMatrix4x4;

	class CVector3d
	{		
	private:

		float32	m_x;
		float32 m_y;
		float32 m_z;
		float32 m_w;
		
	public:
		
		// constructors
		explicit CVector3d (float32 x=0,float32 y=0,float32 z=0) : m_x(x),m_y(y),m_z(z), m_w(1.0f){} ;
		
		void make(float32 x, float32 y, float32 z);
		void setX(float32 x) { m_x = x;}
		void setY(float32 y) { m_y = y;}
		void setZ(float32 z) { m_z = z;}
		void setW(float32 w) { m_w = w;}

		float32 getX() const { return m_x; }
		float32 getY() const { return m_y; }
		float32 getZ() const { return m_z; }
		float32 getW() const { return m_w; }

		// calculates and return length of vector
		float32 getLength() const;

		// calculates and return length of vector ^2
		float32 getLengthBy2() const;

		// returns a vector with the same direction, but with length 1.0
		CVector3d getNormalized() const;
		
		// normalizes and scales down vector to length = 1.0
		void normalize();

		// overloaded self modified operators
		void operator+= (const CVector3d &other);
		void operator-= (const CVector3d &other);
		void operator/= (const CVector3d &other);
		void operator%= (const CVector3d &other);
		
		// overloaded aritmethic operators
		float32	  operator* (const CVector3d &other) const;
		CVector3d operator- () const;
		CVector3d operator* (const float32 scalar) const;
		CVector3d operator% (const CVector3d &other) const;
		CVector3d operator/ (const CVector3d &other) const;
		CVector3d operator+ (const CVector3d &other) const;
		CVector3d operator- (const CVector3d &other) const;
	
		// friend functions
		friend CVector3d operator*(const CVector3d &vector, const CMatrix4x4 &matrix);
		friend CVector3d operator*(float32 scalar, const CVector3d &v);
		friend class CMatrix4x4;
	};


// ====================================================================================================

E3D_INLINE void CVector3d::make(float x, float y, float z) {

	m_x = x; m_y = y; m_z = z;
}

// ====================================================================================================

E3D_INLINE float32 CVector3d::getLength() const {

	return (sqrtf(m_x * m_x + m_y * m_y + m_z * m_z));
}

// ====================================================================================================

E3D_INLINE float32 CVector3d::getLengthBy2() const {

	return (m_x * m_x + m_y * m_y + m_z * m_z);
}

// ====================================================================================================

E3D_INLINE CVector3d CVector3d::getNormalized() const {

	CVector3d res(*this);
	res.normalize();
	return res;
}

// ====================================================================================================

E3D_INLINE void CVector3d::normalize() {

	float l = getLength();
	m_x /= l;
	m_y /= l;
	m_z /= l;
}

// ====================================================================================================

E3D_INLINE CVector3d CVector3d::operator-() const {

	return CVector3d(-m_x,-m_y,-m_z);
}

// ====================================================================================================

E3D_INLINE CVector3d CVector3d::operator*(const float32 scalar) const {
	
	return CVector3d(m_x * scalar, m_y * scalar, m_z * scalar);
}

// ====================================================================================================

E3D_INLINE CVector3d CVector3d::operator%(const CVector3d &other) const {

	return CVector3d (
		m_y * other.m_z - m_z * other.m_y, 
		m_z * other.m_x - m_x * other.m_z,
		m_x * other.m_y - m_y * other.m_x );
}

// ====================================================================================================

E3D_INLINE CVector3d CVector3d::operator/(const CVector3d &other) const {
	
	return CVector3d(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z);
}

// ====================================================================================================

E3D_INLINE CVector3d CVector3d::operator+(const CVector3d &other) const {

	return CVector3d(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

// ====================================================================================================

E3D_INLINE CVector3d CVector3d::operator-(const CVector3d &other) const {
	
	return CVector3d(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

// ====================================================================================================

E3D_INLINE void CVector3d::operator+=(const CVector3d &other) {

	m_x+=other.m_x;
	m_y+=other.m_y;
	m_z+=other.m_z;
}

// ====================================================================================================

E3D_INLINE void CVector3d::operator-=(const CVector3d &other) {

	m_x-=other.m_x;
	m_y-=other.m_y;
	m_z-=other.m_z;
}

// ====================================================================================================

E3D_INLINE void CVector3d::operator%=(const CVector3d &other) {

	make(m_y * other.m_z - m_z * other.m_y, 
		m_z * other.m_x - m_x * other.m_z, 
		m_x * other.m_y - m_x * other.m_y);
}

// ====================================================================================================

E3D_INLINE float32 CVector3d::operator*(const CVector3d &other) const {

	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

// ====================================================================================================

E3D_INLINE CVector3d operator*(float32 scalar, const CVector3d &v) {

	return CVector3d(v.m_x * scalar, v.m_y * scalar, v.m_z * scalar);
}

// ====================================================================================================

} // end namespace

#endif
