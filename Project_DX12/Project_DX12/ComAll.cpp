#include "ComAll.h"

ComAll::~ComAll()
{
	if (allocator)
	{
		allocator->Release();
		allocator = nullptr;
	}
}

bool ComAll::Create(Device& devi, D3D12_COMMAND_LIST_TYPE setType)
{
	type = setType;

	auto hr = devi.GetDev()->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator));
	if (FAILED(hr))
	{
		assert(false && "コマンドアロケーター作成ー失敗ー");
		return true;
	}
	return false;
}