#include "Window.h"
#include "Fac_Ada.h"
#include "Device.h"
#include "ComQue.h"
#include "SwapC.h"
#include "Heap.h"
#include "Target.h"
#include "ComAll.h"
#include "ComLis.h"
#include "Fence.h"
#include "DeltaPoly.h"
#include "SquarePoly.h"
#include "RootSig.h"
#include "Shader.h"
#include "PipLine.h"
#include "Camera.h"
#include "ConBuffer.h"
#include "Obj.h"
#include "DepBuffer.h"

#include "Amo.h"


class Operations
{
private:
	pair<int, int> size = { 800,600 };
	string name = "移動 =>WASD-QE 発射 =>B 終了 =>Z";

	Window wind{};
	Fac_Ada f_a{};
	Device devi{};
	ComQue comQ{};
	SwapC swapC{};	
	Heap descHeap{};
	Target render{};
	ComAll comAll[2]{};
	ComLis comLis{};

	Fence fence{};

	RootSig rootSig{};
	Shader shader{};
	PipLine pipLine{};
	Heap consHeap{};

	Camera camera{};
	ConBuffer camBuff{};

	DeltaPoly delPol{};
	ConBuffer delBuff{};
	Obj delObj{};

	SquarePoly sqrPol{};
	ConBuffer sqrBuff{};
	Obj sqrObj{};

	Heap depsHeap{};
	DepBuffer depBuff{};

	Amo amo{};

public:
	Operations() = default;
	~Operations() = default;

	bool Initialize(HINSTANCE instance)
	{
		if (wind.Create(instance, name, size.first, size.second))			return false;
		if (f_a.Create())			return false;
		if (devi.Create(f_a))			return false;
		if (comQ.Create(devi))			return false;
		if (swapC.Create(wind, f_a, comQ, size.first, size.second))			return false;

		if (descHeap.Create(devi, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapC.GetDesc().BufferCount))			return false;
		if (render.Create(devi, swapC, descHeap))			return false;
		for (int i = 0; i < swapC.GetDesc().BufferCount; i++)
			if (comAll[i].Create(devi, D3D12_COMMAND_LIST_TYPE_DIRECT))				return false;
		if (comLis.Create(devi, comAll[0]))			return false;
		if (fence.Create(devi))			return false;

		if (rootSig.Create(devi))			return false;
		if (shader.Create(devi))			return false;
		if (pipLine.Create(devi, rootSig, shader))			return false;

		camera.Initialize(size.first, size.second);

		if (consHeap.Create(devi, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, swapC.GetDesc().BufferCount, true))			return false;
		if (camBuff.Create(devi, consHeap, sizeof(Camera::ConstBufferData), 0))			return false;

		if (delPol.Create(devi))			return false;
		if (delBuff.Create(devi, consHeap, sizeof(DeltaPoly::ConstBufferData), 1))			return false;
		delObj.Initialize({  0.0f,  0.5f, 5.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		if (sqrPol.Create(devi))			return false;
		if (sqrBuff.Create(devi, consHeap, sizeof(SquarePoly::ConstBufferData), 2))			return false;
		sqrObj.Initialize({  0.0f,  0.0f,  0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });

		if (depsHeap.Create(devi, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1))			return false;
		if (depBuff.Create(devi, depsHeap, size.first, size.second))			return false;

		return true;
	}

	void Loop()
	{
		bool test = false;

		MSG msg{};
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			camera.Updata();

			amo.Check(delObj);

			UINT idx = swapC.GetChain()->GetCurrentBackBufferIndex();

			fence.Check(idx);

			comAll[idx].Reset();
			comLis.Reset(comAll[idx]);

			comLis.Chenge(render, idx, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { render.GetHandle(devi, descHeap, idx) };
			D3D12_CPU_DESCRIPTOR_HANDLE depthHand = depBuff.GetHandle();
			comLis.GetList()->OMSetRenderTargets(1, handles, false, &depthHand);
			
			float backColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
			comLis.GetList()->ClearRenderTargetView(handles[0], backColor, 0, nullptr);
			comLis.GetList()->ClearDepthStencilView(depthHand, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
			comLis.GetList()->SetGraphicsRootSignature(rootSig.GetSign());
			comLis.SetVS(size.first, size.second);

			ID3D12DescriptorHeap* ppHeaps[] = { consHeap.GetHeap() };
			comLis.GetList()->SetDescriptorHeaps(1, ppHeaps);

			{	// カメラ
				Camera::ConstBufferData camData
				{
					DirectX::XMMatrixTranspose(camera.GetView()),
					DirectX::XMMatrixTranspose(camera.GetPro())
				};
				UINT8* pCamData{};
				camBuff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pCamData));
				memcpy(pCamData, &camData, sizeof(camData));
				camBuff.GetBuf()->Unmap(0, nullptr);
				comLis.GetList()->SetGraphicsRootDescriptorTable(0, camBuff.GetHand());
			}

			comLis.GetList()->SetPipelineState(pipLine.GetPip());

			{	// モデル△
				DeltaPoly::ConstBufferData delData
				{
					DirectX::XMMatrixTranspose(delObj.GetWorld()),
					delObj.GetColor()
				};
				UINT8* pDelData{};
				delBuff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pDelData));
				memcpy(pDelData, &delData, sizeof(delData));
				delBuff.GetBuf()->Unmap(0, nullptr);
				comLis.GetList()->SetGraphicsRootDescriptorTable(1, delBuff.GetHand());

				delPol.Draw(comLis);
			}
			{	// モデル□
				SquarePoly::ConstBufferData sqrData
				{
					DirectX::XMMatrixTranspose(sqrObj.GetWorld()),
					sqrObj.GetColor()
				};
				UINT8* pSqrData{};
				sqrBuff.GetBuf()->Map(0, nullptr, reinterpret_cast<void**>(&pSqrData));
				memcpy(pSqrData, &sqrData, sizeof(sqrData));
				sqrBuff.GetBuf()->Unmap(0, nullptr);
				comLis.GetList()->SetGraphicsRootDescriptorTable(1, sqrBuff.GetHand());

				sqrPol.Draw(comLis);
			}
			if (GetAsyncKeyState('B') && !test)
			{	// 弾発射
				amo.Summon(devi, consHeap, sqrObj, comLis);
				test = true;
			}
			if (!GetAsyncKeyState('B') && test) test = false;

			//delObj.Updata();
			sqrObj.Updata();
			amo.Updata(comLis);

			comLis.Chenge(render, idx, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			comLis.GetList()->Close();

			ID3D12CommandList* ppComLiss[] = { comLis.GetList() };
			comQ.GetQ()->ExecuteCommandLists(_countof(ppComLiss), ppComLiss);


			swapC.GetChain()->Present(1, 0);
			fence.SetNext(comQ, idx);

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (GetAsyncKeyState('Z')) break;
		}
	}

};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int nCmdShow)
{
	Operations op;

	if (op.Initialize(instance)) op.Loop();
	
	return 0;
}