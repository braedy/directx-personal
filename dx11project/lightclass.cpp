#include "lightclass.h"

LightClass::LightClass(){
}

LightClass::LightClass(const LightClass& other){
}

LightClass::~LightClass(){
}

void LightClass::SetAmbientColour(float red, float green, float blue, float alpha){
	m_ambientColour = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetDiffuseColour(float red, float green, float blue, float alpha){
	m_diffuseColour = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetPosition(float x, float y, float z){
	m_position = XMFLOAT3(x, y, z);
}

void LightClass::SetDirection(float x, float y, float z){
	m_direction = XMFLOAT3(x, y, z);
}

void LightClass::SetSpecularColour(float red, float green, float blue, float alpha){
	m_specularColour = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetSpecularPower(float power){
	m_specularPower = power;
}

XMFLOAT4 LightClass::GetAmbientColour(){
	return m_ambientColour;
}

XMFLOAT4 LightClass::GetDiffuseColour(){
	return m_diffuseColour;
}

XMFLOAT3 LightClass::GetPosition(){
	return m_position;
}

XMFLOAT3 LightClass::GetDirection(){
	return m_direction;
}

XMFLOAT4 LightClass::GetSpecularColour(){
	return m_specularColour;
}

float LightClass::GetSpecularPower(){
	return m_specularPower;
}