#include "Delta.h"

bool Delta::Create(Device& devi, Heap& cons, UINT idx)
{
	if (poly.Create(devi))		return true;
	if (buff.Create(devi, cons, sizeof(DeltaPoly::ConstBufferData), idx))		return true;

	return false;
}

void Delta::Set(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color)
{
	obj.Initialize(pos, color);
}

void Delta::Update(ComLis& lis)
{
	DeltaPoly::ConstBufferData delData
	{
		DirectX::XMMatrixTranspose(obj.GetWorld()),
		obj.GetColor()
	};
	UINT8* pDelData{};
	buff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pDelData));
	memcpy(pDelData, &delData, sizeof(delData));
	buff.GetBuf()->Unmap(0, nullptr);
	lis.GetList()->SetGraphicsRootDescriptorTable(1, buff.GetHand());

	poly.Draw(lis);
}