#include "ObjManager.h"
#include <cassert>

#include <vector>
#include <memory>

#include "DeltaPoly.h"

using namespace std;

class ObjTable
{
private:
	vector<unique_ptr<PolyTemp>> polygon{};	//ポリゴンの登録

public:
	//ポリゴンの作成
	template <typename T, typename... Args>
	void CreatePoly(Args&&... args)
	{
		auto poly = make_unique<T>(forward<decltype(args)>(args)...);
		polygon.emplace_back(move(poly));
	}

};

ObjTable table;

//オブジェクトの作成
void ObjManager::CreatePoly()
{

	table.CreatePoly<DeltaPoly>();	//三角形ポリゴンの作成
}

//バッファの作成
void ObjManager::CreateBuff()
{

}