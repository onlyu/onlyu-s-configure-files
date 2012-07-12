// 函数：转换为非负数
int to_uint( int iNumber ) { return iNumber > 0 ? iNumber : 0; }

// 函数：转换为自然数
int to_nint( int iNumber ) { return iNumber > 0 ? iNumber : 1; }

// 函数：转换为绝对值
int abs( int iNumber ) { return iNumber < 0 ? - iNumber : iNumber; }
float fabs(float x)
{
	return x<0 ? -x : x;
}

int min(int a, int b) { return (a < b) ? a : b;}

int max(int a, int b) { return (a > b) ? a : b;}

/***************************************************************************
*	将总数量随机分配成N个组，结果放进一个数组中。
*	例如：整组开材料包，叠加总数量按五行随机分配为5组

*	数组中每个元素的值取得过程：
*	1.每个元素初始为随机生成的一个0-期望值（平均数）之间的数
*	2.每个元素加上剩下的 总数量/分组 （平均数）
*	3.余数随机加给数组中的元素
****************************************************************************/
int* rand_group_to_array( int iAmount, int iGroupNum )
{
	int iAverage;
	int iLeftDiv, iLeftMod;	
	int *arrRet;	
	int i;

	if( iAmount<=0 || iGroupNum <=0 )
	{
		return ({});
	}
	
	arrRet = allocate( iGroupNum );
	iAverage = iAmount / iGroupNum;
	
	//1.先为每个分量随机分配一个值	
	for( i=0; i<iGroupNum; i++ )
	{
		int iRandom = random(iAverage + 1);
		arrRet[i] = iRandom;
		iAmount -= iRandom;
	}
	iLeftDiv = iAmount / iGroupNum;		//商(平均数)
	iLeftMod = iAmount % iGroupNum;		//余数
	
	//2.每个元素加上剩下的 总数量/分组	
	for( i=0; i<iGroupNum; i++ )
	{
		arrRet[i] += iLeftDiv;
	}
	
	//3.余数随机加给数组中的元素
	for( i=0; i<iLeftMod; i++ )
	{
		arrRet[random(iGroupNum)] += 1;
	}

	return arrRet;
}
