#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

#include <math.h>

class PositionClass
{
	float m_frameTime;
	float m_rotationY;
	float m_leftTurnSpeed, m_rightTurnSpeed;

public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTime(float);
	void GetRotation(float&);

	void TurnLeft(bool);
	void TurnRight(bool);
};

#endif