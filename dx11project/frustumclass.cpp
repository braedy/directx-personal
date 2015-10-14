#include "frustumclass.h"

FrustumClass::FrustumClass(){
}

FrustumClass::FrustumClass(const FrustumClass& other){
}

FrustumClass::~FrustumClass(){
}

void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX &projectionMatrix, XMMATRIX &viewMatrix)
{
	float zMinimum, r;
	XMMATRIX frustumMatrix;
	XMFLOAT4X4 projection, frustum;

	//access projection matrix
	XMStoreFloat4x4(&projection, projectionMatrix);	

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projection._43 / projection._33;
	r = screenDepth / (screenDepth - zMinimum);
	projection._33 = r;
	projection._43 = -r * zMinimum;

	//write back to projection matrix
	projectionMatrix = XMLoadFloat4x4(&projection);

	// Create the frustum matrix from the view matrix and updated projection matrix.
	frustumMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	// copy into accessible matrix
	XMStoreFloat4x4(&frustum, frustumMatrix);
	
	// Calculate near plane of frustum.
	m_planes[0].x = frustum._14 + frustum._13;
	m_planes[0].y = frustum._24 + frustum._23;
	m_planes[0].z = frustum._34 + frustum._33;
	m_planes[0].w = frustum._44 + frustum._43;
	//D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	// Calculate far plane of frustum.
	m_planes[1].x = frustum._14 - frustum._13;
	m_planes[1].y = frustum._24 - frustum._23;
	m_planes[1].z = frustum._34 - frustum._33;
	m_planes[1].w = frustum._44 - frustum._43;
	//D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	// Calculate left plane of frustum.
	m_planes[2].x = frustum._14 + frustum._11;
	m_planes[2].y = frustum._24 + frustum._21;
	m_planes[2].z = frustum._34 + frustum._31;
	m_planes[2].w = frustum._44 + frustum._41;
	//D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	// Calculate right plane of frustum.
	m_planes[3].x = frustum._14 - frustum._11;
	m_planes[3].y = frustum._24 - frustum._21;
	m_planes[3].z = frustum._34 - frustum._31;
	m_planes[3].w = frustum._44 - frustum._41;
	//D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	// Calculate top plane of frustum.
	m_planes[4].x = frustum._14 - frustum._12;
	m_planes[4].y = frustum._24 - frustum._22;
	m_planes[4].z = frustum._34 - frustum._32;
	m_planes[4].w = frustum._44 - frustum._42;
	//D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	// Calculate bottom plane of frustum.
	m_planes[5].x = frustum._14 + frustum._12;
	m_planes[5].y = frustum._24 + frustum._22;
	m_planes[5].z = frustum._34 + frustum._32;
	m_planes[5].w = frustum._44 + frustum._42;
	//D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);

	for (int i = 0; i < 6; i++){
		XMVECTOR plane = XMVectorZero();
		plane = XMLoadFloat4(&m_planes[i]);
		plane = XMPlaneNormalize(plane);
		XMStoreFloat4(&m_planes[i], plane);
	}
}

bool FrustumClass::CheckPoint(float x, float y, float z)
{
	int i;

	XMVECTOR point = XMLoadFloat3(&XMFLOAT3(x, y, z));
	//check if point is inside all six planes of the view frustum
	for (i = 0; i<6; i++)
	{
		XMVECTOR plane;
		plane = XMLoadFloat4(&m_planes[i]);
		if (XMVectorGetX(XMPlaneDotCoord(plane, point)) < 0.0f){
			return false;
		}
	}

	return true;
}

bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;
	XMVECTOR ftl, ftr, fbl, fbr, btl, btr, bbl, bbr;

	ftl = XMLoadFloat3(&XMFLOAT3((xCenter - radius), (yCenter - radius), (zCenter - radius))); //front top left
	ftr = XMLoadFloat3(&XMFLOAT3((xCenter + radius), (yCenter - radius), (zCenter - radius))); //front top right
	fbl = XMLoadFloat3(&XMFLOAT3((xCenter - radius), (yCenter + radius), (zCenter - radius))); //front bottom left
	fbr = XMLoadFloat3(&XMFLOAT3((xCenter + radius), (yCenter + radius), (zCenter - radius))); //front bottom right
	btl = XMLoadFloat3(&XMFLOAT3((xCenter - radius), (yCenter - radius), (zCenter + radius))); //back top left
	btr = XMLoadFloat3(&XMFLOAT3((xCenter + radius), (yCenter - radius), (zCenter + radius))); //back top right
	bbl = XMLoadFloat3(&XMFLOAT3((xCenter - radius), (yCenter + radius), (zCenter + radius))); //back bottom left
	bbr = XMLoadFloat3(&XMFLOAT3((xCenter + radius), (yCenter + radius), (zCenter + radius))); //back bottom right

	// Check if any one point of the cube is in the view frustum.
	for (i = 0; i<6; i++)
	{
		XMVECTOR plane;
		plane = XMLoadFloat4(&m_planes[i]);
		
		if (XMVectorGetX(XMPlaneDotCoord(plane, ftl)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, ftr)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, fbl)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, fbr)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, btl)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, btr)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, bbl)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, bbr)) >= 0.0f){
			continue;
		}

		return false;
	}

	return true;
}

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;
	XMVECTOR sphere;
	sphere = XMLoadFloat3(&XMFLOAT3(xCenter, yCenter, zCenter));

	// Check if the radius of the sphere is inside the view frustum.
	for (i = 0; i<6; i++){
		XMVECTOR plane;
		plane = XMLoadFloat4(&m_planes[i]);

		if (XMVectorGetX(XMPlaneDotCoord(plane, sphere)) < -radius){
			return false;
		}
	}

	return true;
}

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	int i;
	XMVECTOR ftl, ftr, fbl, fbr, btl, btr, bbl, bbr;

	ftl = XMLoadFloat3(&XMFLOAT3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))); //front top left
	ftr = XMLoadFloat3(&XMFLOAT3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))); //front top right
	fbl = XMLoadFloat3(&XMFLOAT3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))); //front bottom left
	fbr = XMLoadFloat3(&XMFLOAT3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize))); //front bottom right
	btl = XMLoadFloat3(&XMFLOAT3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize))); //back top left
	btr = XMLoadFloat3(&XMFLOAT3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))); //back top right
	bbl = XMLoadFloat3(&XMFLOAT3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))); //back bottom left
	bbr = XMLoadFloat3(&XMFLOAT3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))); //back bottom right


	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for (i = 0; i<6; i++)
	{
		XMVECTOR plane;
		plane = XMLoadFloat4(&m_planes[i]);

		if (XMVectorGetX(XMPlaneDotCoord(plane, ftl)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, ftr)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, fbl)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, fbr)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, btl)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, btr)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, bbl)) >= 0.0f){
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(plane, bbr)) >= 0.0f){
			continue;
		}

		return false;
	}

	return true;
}