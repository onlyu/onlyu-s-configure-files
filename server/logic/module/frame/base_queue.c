#include <debug.h>
// 数组
// 和位置key弱相关，但符合数组特性，有具体位置概念
// Pos从1开始

/*
mapping mpQueue = ([
 	iOwner: 12345678, // 宿主
	iBagId: 1, // 我是物品栏1 
	iMaxSize : 20,
	iSize : 0,
	Grids:
		({
		ItemId,
		ItemId,
		...
		ItemId,
		}),

 ]);
 */

int GetBagSize(mapping mpBag)
{
	return mpBag["iSize"];
}

int RecountSize(mapping mpBag)
{
	mpBag["iSize"] = sizeof((mpBag["Grids"]));
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
mixed * GetGrids(mapping mpBag)
{
	return mpBag["Grids"];
}

// 顺序插入，放在第一或者最后的位置
// 返回插入的位置
private int AddToBag(int iTarget, mapping mpBag, int bHead)
{
	if ( IsBagFull(mpBag) )
	{
		return 0;
	}	

	//Import("TRACE_OBJ")->AddToHost( iTarget, mpBag["iOwner"], iPos );

	mpBag["iSize"] += 1;
	if ( bHead )
	{	
		mpBag["Grids"] = ({ iTarget }) + mpBag["Grids"];
		return 1;
	}	
	else
	{
		/*
		foreach ( int i in mpBag["Grids"] )
		{
		
		}	
		*/
		mpBag["Grids"] = mpBag["Grids"] + ({ iTarget });
		return mpBag["iSize"];
	}	
}

int PushHead(int iTarget, mapping mpBag)
{
	return AddToBag(iTarget, mpBag, 1);
}

int PushTail(int iTarget, mapping mpBag)
{
	return AddToBag(iTarget, mpBag, 0);
}

int GetPosById(mapping mpBag, int iTarget)
{
	// debug_message( sprintf("mpBag = %O, iTarget =%d", mpBag, iTarget) );
	return member_array(iTarget, mpBag["Grids"]) + 1; // pos 从1开始
}

int GetIdByPos(mapping mpBag, int iPos)
{
	iPos -= 1; // Pos从1开始

	//debug_message( sprintf("iPos %d, BagSize %d, Grids %O", iPos, GetBagSize(mpBag), mpBag["Grids"]) );
	if ( iPos >= GetBagSize(mpBag) ) return 0;

	return mpBag["Grids"][iPos];
}

void XSet(mapping mpBag, int iKey, int iValue)
{
	int i;

	iKey -= 1; // pos 从1 开始
	if ( sizeof(mpBag["Grids"]) > iKey )
	{
		mpBag["Grids"][iKey] = iValue;
		//debug_message( sprintf("AFTER XSet %d mpBag[grids] = %O", iKey, mpBag["Grids"]) );
		return;
	}

	for ( i = mpBag["iSize"]; i <= iKey; i ++ )
	{
		mpBag["Grids"] += ({ -1 });
		mpBag["iSize"] += 1;
	}	
	
	mpBag["Grids"][iKey] = iValue;

	// debug_message( sprintf("AFTER XSet %d mpBag[grids] = %O", iKey, mpBag["Grids"]) );
}

void RemoveById(mapping mpBag, int iId)
{
	mpBag["Grids"] -= ({ iId });
	mpBag["iSize"] -= 1;
}	

void RemoveByPos(mapping mpBag, int iPos)
{
	mixed mxTmp;
	iPos -= 1;

	mxTmp = mpBag["Grids"][iPos];
	RemoveById(mpBag, mxTmp);
}	

void SwapItems(mapping mpBag, int iPos1, int iPos2)
{
	mixed PosItem1, PosItem2;

	// 适应游戏逻辑，pos从1开始
	iPos1 -= 1;
	iPos2 -= 1;

	PosItem1 = mpBag["Grids"][iPos1];
	PosItem2 = mpBag["Grids"][iPos2];

	mpBag["Grids"][iPos1] = PosItem2;
	mpBag["Grids"][iPos2] = PosItem1;
}

mapping CreateBag( int iOwner, int iBagId, int iMaxSize)
{
	mapping mpBag = ([
		"iOwner" : iOwner, // 宿主 
		"iBagId" : iBagId, // 第几个包裹
		"iMaxSize" : iMaxSize,
		"iSize" : 0,
		"Grids" : ({}),
	]);

	return mpBag ;
}

int ResizeBag(mapping mpBag, int iSize)
{
	assert( iSize > GetBagSize(mpBag) );
	assert( iSize > GetMaxSize(mpBag) );

	mpBag["iMaxSize"] = iSize;
	return iSize;
}

#ifdef __TEST__
void __test__()
{

}
#endif

