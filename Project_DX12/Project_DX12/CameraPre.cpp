#include "CameraPre.h"

bool CameraPre::Create(int wi, int hi, Device& devi, Heap& cons, UINT idx)
{ 
	camera.Initialize(wi, hi); 
	if (camBuff.Create(devi, cons, sizeof(Camera::ConstBufferData), idx))		return true;

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
	camBuff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pCamData));
	memcpy(pCamData, &camData, sizeof(camData));
	camBuff.GetBuf()->Unmap(0, nullptr);
	list.GetList()->SetGraphicsRootDescriptorTable(0, camBuff.GetHand());
}