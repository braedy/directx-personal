#include "positionclass.h"

PositionClass::PositionClass(){
	m_frameTime = 0.0f;
	m_rotationY = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
}

PositionClass::PositionClass(const PositionClass& other){
}

PositionClass::~PositionClass(){
}

void PositionClass::SetFrameTime(float time){
	m_frameTime = time;
}

void PositionClass::GetRotation(float& y){
	y = m_rotationY;
}

void PositionClass::TurnLeft(bool keydown)
{
	//if key pressed, increase camera turn speed (left)
	//else slow down the turn speed.
	if (keydown){
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if (m_leftTurnSpeed > (m_frameTime * 0.15f)){
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else{
		m_leftTurnSpeed -= m_frameTime* 0.005f;

		if (m_leftTurnSpeed < 0.0f){
			m_leftTurnSpeed = 0.0f;
		}
	}

	//update the rotation with turning speed
	m_rotationY -= m_leftTurnSpeed;
	if (m_rotationY < 0.0f){
		m_rotationY += 360.0f;
	}
}

void PositionClass::TurnRight(bool keydown)
{
	//if key pressed, increase camera turn speed (right)
	//else slow down the turn speed.
	if (keydown){
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if (m_rightTurnSpeed > (m_frameTime * 0.15f)){
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else{
		m_rightTurnSpeed -= m_frameTime* 0.005f;

		if (m_rightTurnSpeed < 0.0f){
			m_rightTurnSpeed = 0.0f;
		}
	}

	//update rotation with turning speed
	m_rotationY += m_rightTurnSpeed;
	if (m_rotationY > 360.0f){
		m_rotationY -= 360.0f;
	}
}

