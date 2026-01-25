#include "CubePre.h"

void CubePre::Summon(Device& devi, Heap& heap, Obj& sqrObj, ComLis& list)
{
	for (int i = 0; i < vacants.size(); i++)
	{
		if (vacants[i]) continue;

		vacants[i] = true;
		if (pols[i].Create(devi))		assert(false && "弾ポリゴン作成ー失敗ー");
		if (buffs[i].Create(devi, heap, sizeof(CubePoly::ConstBufferData), 3 + i))	assert(false && "弾コンスタントバッファ作成ー失敗ー");
		objs[i].Initialize({ sqrObj.GetPos().x,sqrObj.GetPos().y,sqrObj.GetPos().z + 0.1f }, { 0.0f, 0.0f, 1.0f, 0.5f });
		return;
	}
	vacants.push_back(true);
	pols.push_back(CubePoly{});
	buffs.push_back(ConBuffer{});
	objs.push_back(Obj{});

	if (pols.back().Create(devi))		assert(false && "弾ポリゴン作成ー失敗ー");
	if (buffs.back().Create(devi, heap, sizeof(CubePoly::ConstBufferData), 3 + (UINT)(buffs.size() - 1)))	assert(false && "弾コンスタントバッファ作成ー失敗ー");
	objs.back().Initialize({ sqrObj.GetPos().x,sqrObj.GetPos().y,sqrObj.GetPos().z + 0.1f }, { 0.0f, 0.0f, 1.0f, 0.5f });
}

void CubePre::Update(ComLis& list)
{
	for (int i = 0; i < vacants.size(); i++)
	{
		if (!vacants[i]) continue;

		objs[i].Amo();
		CubePoly::ConstBufferData data
		{
			DirectX::XMMatrixTranspose(objs[i].GetWorld()),
			objs[i].GetColor()
		};
		UINT8* pData{};
		buffs[i].GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pData));
		memcpy(pData, &data, sizeof(data));
		buffs[i].GetBuf()->Unmap(0, nullptr);
		list.GetList()->SetGraphicsRootDescriptorTable(1, buffs[i].GetHand());

		pols[i].Draw(list);
	}
}

void CubePre::Check(Obj& targetObj)
{
	for (int i = 0; i < vacants.size(); i++)
	{
		if (!vacants[i]) continue;

		DirectX::XMFLOAT3 amoPos = objs[i].GetPos();
		if (tPos.z - amoPos.z > 0.05f) continue;
		if (amoPos.y - 0.1f > tPos.y || amoPos.y + 0.1f < rPos.y) continue;

		if (amoPos.x <= 0 && amoPos.x + 0.1f >= lPos.x)
		{
			DirectX::XMFLOAT2 dir = { amoPos.x - tPos.x, amoPos.y - tPos.y };
			DirectX::XMFLOAT2 edge = { lPos.x - tPos.x, lPos.y - tPos.y };
			{
				float mag = 1.0f / sqrtf(powf(edge.x, 2) + powf(edge.y, 2));
				edge.x *= mag;
				edge.y *= mag;
			}

			float dest = dir.x * edge.x + dir.y * edge.y;

			DirectX::XMFLOAT2 proj = { edge.x * dest + tPos.x, edge.y * dest + tPos.y };
			float end = sqrtf(pow(proj.x - tPos.x, 2) + pow(proj.y - lPos.y, 2));

			float topH = sqrt(powf(amoPos.x + 0.1f - tPos.x, 2) + powf(amoPos.y + 0.1f - lPos.y, 2));
			float undH = sqrt(powf(amoPos.x + 0.1f - tPos.x, 2) + powf(amoPos.y - 0.1f - lPos.y, 2));

			if (end >= topH || end >= undH)
			{
				targetObj.Hit();

				vacants[i] = false;
			}
		}
		else if (amoPos.x > 0 && amoPos.x - 0.1f <= rPos.x)
		{
			DirectX::XMFLOAT2 dir = { amoPos.x - tPos.x, amoPos.y - tPos.y };
			DirectX::XMFLOAT2 edge = { rPos.x - tPos.x, rPos.y - tPos.y };
			{
				float mag = 1.0f / sqrtf(powf(edge.x, 2) + powf(edge.y, 2));
				edge.x *= mag;
				edge.y *= mag;
			}

			float dest = dir.x * edge.x + dir.y * edge.y;

			DirectX::XMFLOAT2 proj = { edge.x * dest + tPos.x, edge.y * dest + tPos.y };
			float end = sqrtf(pow(proj.x - tPos.x, 2) + pow(proj.y - rPos.y, 2));

			float topH = sqrt(powf(amoPos.x - 0.1f - tPos.x, 2) + powf(amoPos.y + 0.1f - rPos.y, 2));
			float undH = sqrt(powf(amoPos.x - 0.1f - tPos.x, 2) + powf(amoPos.y - 0.1f - rPos.y, 2));

			if (end >= topH || end >= undH)
			{
				targetObj.Hit();

				vacants[i] = false;
			}
		}

		if (amoPos.z > 8.0f) vacants[i] = false;
	}
}