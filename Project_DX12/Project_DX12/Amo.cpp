#include "Amo.h"

void Amo::Summon(Device& devi, Heap& heap, Obj& sqrObj, ComLis& list)
{
	for(int i = 0; i < vacants.size(); i++)
	{
		if (vacants[i]) continue;

		vacants[i] = true;
		if (pols[i].Create(devi, true))		assert(false && "弾ポリゴン作成ー失敗ー");
		if (buffs[i].Create(devi, heap, sizeof(SquarePoly::ConstBufferData), 3 + i))	assert(false && "弾コンスタントバッファ作成ー失敗ー");
		objs[i].Initialize(sqrObj.GetPos(), { 0.0f, 0.0f, 1.0f, 0.5f });
		return;
	}
	vacants.push_back(true);
	pols.push_back(SquarePoly{});
	buffs.push_back(ConBuffer{});
	objs.push_back(Obj{});

	if (pols.back().Create(devi, true))		assert(false && "弾ポリゴン作成ー失敗ー");
	if (buffs.back().Create(devi, heap, sizeof(SquarePoly::ConstBufferData), 3 + (UINT)(buffs.size() - 1)))	assert(false && "弾コンスタントバッファ作成ー失敗ー");
	objs.back().Initialize(sqrObj.GetPos(), { 0.0f, 0.0f, 1.0f, 0.5f });
}

void Amo::Update(ComLis& list)
{
	for(int i = 0; i < vacants.size(); i++)
	{
		if (!vacants[i]) continue;

		objs[i].Amo();
		SquarePoly::ConstBufferData amoData
		{
			DirectX::XMMatrixTranspose(objs[i].GetWorld()),
			objs[i].GetColor()
		};
		UINT8* pAmoData{};
		buffs[i].GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pAmoData));
		memcpy(pAmoData, &amoData, sizeof(amoData));
		buffs[i].GetBuf()->Unmap(0, nullptr);
		list.GetList()->SetGraphicsRootDescriptorTable(1, buffs[i].GetHand());
		
		pols[i].Draw(list);
	}
}

void Amo::Check(Obj& targetObj)
{
	for(int i=0; i < vacants.size(); i++)
	{
		if (!vacants[i]) continue;

		DirectX::XMFLOAT3 amoPos = objs[i].GetPos();
		if (tPos.z - amoPos.z > 0.05f) continue;
		if (amoPos.y - 0.1f > tPos.y || amoPos.y + 0.1f < rPos.y) continue;

		if(amoPos.x <= 0 && amoPos.x +0.1f >= lPos.x)
		{
			DirectX::XMFLOAT2 dir = { amoPos.x - tPos.x, amoPos.y - tPos.y };
			DirectX::XMFLOAT2 edge = { lPos.x - tPos.x, lPos.y - tPos.y };
			{
				float mag = 1.0f / sqrtf(powf(edge.x, 2) + powf(edge.y, 2));
				edge.x *= mag;
				edge.y *= mag;
			}

			float dest = dir.x * edge.x + dir.y * edge.y ;

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
		else if(amoPos.x > 0 && amoPos.x -0.1f <= rPos.x)
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

/*
	pols.push_back(amoPol);
	buffs.push_back(amoBuff);
	objs.push_back(amoObj);

	if(pols[idx].Create(devi, true))		assert(false && "弾ポリゴン作成ー失敗ー");
	if (buffs[idx].Create(devi, heap, sizeof(SquarePoly::ConstBufferData), 3  + idx))		assert(false && "弾コンスタントバッファ作成ー失敗ー");
	objs[idx].Initialize(sqrObj.GetPos(), {0.0f, 0.0f, 1.0f, 0.5f});
	idx++;*/