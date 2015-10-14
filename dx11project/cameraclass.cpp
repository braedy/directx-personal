#include "cameraclass.h"

CameraClass::CameraClass(){
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other){
}


CameraClass::~CameraClass(){
}

void CameraClass::SetPosition(float x, float y, float z){
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}


void CameraClass::SetRotation(float x, float y, float z){
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

XMFLOAT3 CameraClass::GetPosition(){
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 CameraClass::GetRotation(){
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render(){
	XMFLOAT3 f3_lookAt;
	XMVECTOR up, position, lookAt;
	float /*yaw, pitch, roll,*/ radians;
	XMMATRIX rotationMatrix;

	// set "up vector"
	up = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.f));

	// setup camera's world position
	position = XMLoadFloat3(&XMFLOAT3(m_positionX, m_positionY, m_positionZ));

	//BIGCHANGES
	/*
	// set default lookAt
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// set yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// create rotation matrix from yaw, pitch, roll values
	rotationMatrix = XMMatrixRotationRollPitchYaw(yaw, pitch, roll);

	// load float3s into xmvector
	v_up = XMLoadFloat3(&up);
	v_position = XMLoadFloat3(&position);
	v_lookAt = XMLoadFloat3(&lookAt);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	v_lookAt = XMVector3TransformCoord(v_lookAt, rotationMatrix);
	v_up = XMVector3TransformCoord(v_up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	v_lookAt = v_position + v_lookAt;*/
	//calculate rotation
	radians = m_rotationY * 0.0174532925f;
	//set up camera viewpoint
	f3_lookAt.x = sinf(radians) + m_positionX;
	f3_lookAt.y = m_positionY;
	f3_lookAt.z = cosf(radians) + m_positionZ;
	//store lookAt
	lookAt = XMLoadFloat3(&f3_lookAt);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix){
	viewMatrix = m_viewMatrix;
}