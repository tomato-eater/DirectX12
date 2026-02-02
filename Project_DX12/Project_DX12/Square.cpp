#include "Square.h"

bool Square::Create(Device& devi, Heap& cons, UINT idx)
{
	if (poly.Create(devi))		return true;
	if (buff.Create(devi, cons, sizeof(SquarePoly::ConstBufferData), idx))		return true;

	return false;
}

void Square::Set(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color)
{
	obj.Initialize(pos, color);
}

void Square::Update(ComLis& list)
{
	obj.Updata();		//ˆÚ“®ˆ—

	SquarePoly::ConstBufferData sqrData
	{
		DirectX::XMMatrixTranspose(obj.GetWorld()),
		obj.GetColor()
	};
	UINT8* pSqrData{};
	buff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pSqrData));
	memcpy(pSqrData, &sqrData, sizeof(sqrData));
	buff.GetBuf()->Unmap(0, nullptr);
	list.GetList()->SetGraphicsRootDescriptorTable(1, buff.GetHand());

	poly.Draw(list);
}