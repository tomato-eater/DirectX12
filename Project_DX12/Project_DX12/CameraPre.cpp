#include "CameraPre.h"

//bool CameraPre::Create(int wi, int hi, Device& devi, Heap& cons, UINT idx)
//{ 
//	camera.Initialize(wi, hi); 
//	if (camBuff.Create(devi, cons, sizeof(Camera::ConstBufferData), idx))		return true;
//
//	return false;
//}

bool CameraPre::Create(int wi, int hi, Device& devi, PosPro& posPro, UINT idx)
{ 
	camera.Initialize(wi, hi); 
	if (camBuff.Create(devi, posPro, sizeof(Camera::ConstBufferData), idx))		return true;

	return false;
}

void CameraPre::Change(ComLis& list)
{
	Camera::ConstBufferData camData
	{
		DirectX::XMMatrixTranspose(camera.GetView()),
		DirectX::XMMatrixTranspose(camera.GetPro())
	};
	UINT8* pCamData{};
	auto hr = camBuff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pCamData));
	if(FAILED(hr)) {
		assert(false && "カメラバッファマップー失敗ー");
		return;
	}
	memcpy(pCamData, &camData, sizeof(camData));
	camBuff.GetBuf()->Unmap(0, nullptr);
	list.GetList()->SetGraphicsRootDescriptorTable(0, camBuff.GetHand());
}