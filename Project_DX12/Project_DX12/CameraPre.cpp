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
	auto hr = camBuff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pCamData));
	if (FAILED(hr)) {
		// リソースからデバイスを取り、削除理由を取得してログ
		ID3D12Device* dev = nullptr;
		if (camBuff.GetBuf() && SUCCEEDED(camBuff.GetBuf()->GetDevice(IID_PPV_ARGS(&dev)))) {
			HRESULT reason = dev->GetDeviceRemovedReason();
			char buf[256];
			sprintf_s(buf, "Map failed hr=0x%08X, GetDeviceRemovedReason=0x%08X\n", hr, reason);
			OutputDebugStringA(buf);
			dev->Release();
		} else {
			OutputDebugStringA("Map failed and cannot get device\n");
		}
		assert(false && "カメラバッファマップー失敗ー");
		return;
	}
	memcpy(pCamData, &camData, sizeof(camData));
	camBuff.GetBuf()->Unmap(0, nullptr);
	list.GetList()->SetGraphicsRootDescriptorTable(0, camBuff.GetHand());
}