#include <debug.h>
// Bag格子的概念，是个容器，可以是摆摊、物品栏、好友、召唤兽栏等
// 和Queue一个很大的区别在于，Bag有位置的概念，Queue位置概念太弱

/*
 mpBag = ([
 	iOwner: 12345678, // 宿主
	iBagId: 1, // 我是物品栏1 
	iMaxSize : 20,
	iSize : 0,
	Grids:
		([
		pos1 : ItemId,
		pos2 : ItemId,
		...
		pos20 : ItemId,
		]),
	
	// Id2Pos : ([ ]), // id 2 pos 对应表？不用了吧，等效率成问题再优化之。
 ]);
 */

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// // 分清楚frame\Bag\(grids)pos的概念
// // Frame是item\summon这样的大集合，例如：物品栏集合
// // Bag是Frame的子集，是单一包裹的集合 例如：转生物品栏和任务物品栏
// // grids\pos是Bag里位置 例如：任务物品栏第一格
// // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 这里是单纯对bag的数据操作，其他相关校验逻辑，应该由上层保证
//
//
//
//
//
//
//
//
// 请务必注意，iPos从1开始取值

int GetBagSize(mapping mpBag)
{
	return mpBag["iSize"];
}

// 有个占位符
int RecountSize(mapping mpBag)
{
	mpBag["iSize"] = sizeof(values(mpBag["Grids"]));
}

int GetMaxSize(mapping mpBag)
{
	return mpBag["iMaxSize"];
}

int IsBagFull(mapping mpBag)
{
	return GetBagSize(mpBag) >= GetMaxSize(mpBag);
}

int CountFreePos(mapping mpBag)
{
	return GetMaxSize(mpBag) - GetBagSize(mpBag);
}

// 请注意，绝对不要修改传出去的值，仅用来取值
// 没有必要的话，尽量使用 GetGridKeys\GetGridValues 来替代
mapping GetGrids(mapping mpBag)
{
	return mpBag["Grids"];
}

// 为了安全，不传回mapping
int * GetGridKeys(mapping mpBag)
{
	return keys(mpBag["Grids"]);
}

// 为了安全，不传回mapping
int * GetGridValues(mapping mpBag)
{
	return values(mpBag["Grids"]);
}

int GetEmptyPos(mapping mpBag)
{
	//debug_message( sprintf("GetMaxSize(mpBag) %d", GetMaxSize(mpBag)) );
	for ( int i=1; i <= GetMaxSize(mpBag); i ++ )
	{
		if ( undefinedp(mpBag["Grids"][i]) ) return i;
	}
	return -1;
}

int IsValidPos(mapping mpBag, int iPos)
{
	//return 1;
	return (iPos >= 1 && iPos <= mpBag["iMaxSize"]) ;
}

int IsEmptyPos(mapping mpBag, int iPos)
{
	// 非合法空格，也算非空
	if (!IsValidPos(mpBag, iPos)) 
		return 0;

	return undefinedp(mpBag["Grids"][iPos]);
}

int XSet( int iTarget, mapping mpBag, int iPos)
{
	assert( iPos );
	// assert( undefinedp(mpBag["Grids"][iPos]) );
	if ( undefinedp(mpBag["Grids"][iPos]) )
		mpBag["iSize"] += 1;

	mpBag["Grids"][iPos] = iTarget;

	return iPos;
}

varargs int AddToBag(int iTarget, mapping mpBag, int iPos)
{
	if ( iPos <= 0 ) 
	{
		iPos = GetEmptyPos(mpBag);
	}
	if (!IsEmptyPos(mpBag, iPos)) return 0;

	return XSet(iTarget, mpBag, iPos);
}

// false == 0
int GetIdByPos(mapping mpBag, int iPos)
{
	/*
	if (!IsValidPos(mpBag, iPos)) return 0;
	if (IsEmptyPos(mpBag, iPos) ) return 0;

	// assert(!IsEmptyPos(mpBag, iPos));
	*/
	return mpBag["Grids"][iPos];
}

// 管理num // false == 0
int GetPosById( mapping mpBag, int iTarget)
{
	// debug_message( sprintf("mpBag = %O, iTarget = %d", mpBag, iTarget) );
	foreach( int iPos, int iNum in mpBag["Grids"] )
	{
		if (iNum == iTarget)
			return iPos;
	}
	return 0;
}

int RemoveByPos(mapping mpBag, int iPos)
{
	if (!IsValidPos(mpBag, iPos)) return 0;
	if (IsEmptyPos(mpBag, iPos) ) return 0;

	// 删除物品
	map_delete(mpBag["Grids"], iPos);
	mpBag["iSize"] -= 1;

	// Import("TRACE_OBJ")->RemoveFromHost( iTarget, mpBag["iOwner"], iPos );

	return 1;
}

int BagMove(mapping mpBag, int iPosSrc, int iPosDst)
{
	int iTarget;

	iTarget = GetIdByPos(mpBag, iPosSrc);
	if ( !iTarget ||
	     !IsValidPos(mpBag, iPosDst) ||
		 !IsEmptyPos(mpBag, iPosDst) )
	{
		return 0;
	}
	RemoveByPos(mpBag, iPosSrc);
	return AddToBag(iTarget, mpBag, iPosDst);
}

// mapping实现Grids，所以不用预先分配大小
mapping CreateBag( int iOwner, int iBagId, int iMaxSize)
{
	mapping mpBag = ([
		"iOwner" : iOwner, // 宿主 
		"iBagId" : iBagId, // 第几个包裹
		"iMaxSize" : iMaxSize,
		"iSize" : 0,
		"Grids" : ([]),
	]);

	return mpBag ;
}

int ResizeBag(mapping mpBag, int iSize)
{
	//assert( iSize > GetBagSize(mpBag) );
	//assert( iSize > GetMaxSize(mpBag) );

	mpBag["iMaxSize"] = iSize;
	return iSize;
}

#ifdef __TEST__
void __test__()
{

}
#endif

