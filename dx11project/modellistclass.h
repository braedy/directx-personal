#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_

#include <DirectXMath.h>
#include <stdlib.h>
#include <time.h>

using namespace DirectX;

class ModelListClass{
	struct ModelInfoType{
		XMFLOAT4 colour;
		float posX, posY, posZ;
	};

	int m_modelCount;
	ModelInfoType* m_ModelInfoList;

public:
	ModelListClass();
	ModelListClass(const ModelListClass&);
	~ModelListClass();

	bool Initialize(int);
	void Shutdown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, XMFLOAT4&);
};

#endif