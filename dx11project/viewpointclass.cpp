#include "viewpointclass.h"

ViewPointClass::ViewPointClass(){
}

ViewPointClass::ViewPointClass(const ViewPointClass& other){
}

ViewPointClass::~ViewPointClass(){
}

void ViewPointClass::SetPosition(float x, float y, float z){
	m_position = XMFLOAT3(x, y, z);
}

void ViewPointClass::SetLookAt(float x, float y, float z){
	m_lookAt = XMFLOAT3(x, y, z);
}

void ViewPointClass::SetProjectionParameters(float fieldOfView, float aspectRatio, float nearPlane, float farPlane){
	m_fieldOfView = fieldOfView;
	m_aspectRatio = aspectRatio;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
}

void ViewPointClass::GenerateViewMatrix(){
	XMVECTOR position, lookAt, up;
	//load into vector objects
	position = XMLoadFloat3(&m_position);
	lookAt = XMLoadFloat3(&m_lookAt);
	//set up vector
	up = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));

	//create view matrix
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void ViewPointClass::GenerateProjectionMatrix(){
	//create projection matrix for the viewpoint
	m_projectionMatrix = XMMatrixPerspectiveFovLH(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
}

void ViewPointClass::GetViewMatrix(XMMATRIX &viewMatrix){
	viewMatrix = m_viewMatrix;
}

void ViewPointClass::GetProjectionMatrix(XMMATRIX &projectionMatrix){
	projectionMatrix = m_projectionMatrix;
}