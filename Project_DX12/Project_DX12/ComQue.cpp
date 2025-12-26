#include "ComQue.h"

ComQue::~ComQue()
{
	if (commandQ)
	{
		commandQ->Release();
		commandQ = nullptr;
	}
}

bool ComQue::Create(Device& devi)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	auto hr = devi.GetDev()->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQ));
	if (FAILED(hr))
	{
		assert(false && "コマンドキュー作成ー失敗ー");
		return true;
	}
	commandQ->SetName(L"Main_Command_Queue");

	return false;
}