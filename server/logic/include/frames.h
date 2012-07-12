
#ifndef __FRAMS_H__
#define __FRAMS_H__

// 每个包裹之间的间隔
#define BAG_SECTION 100
#define CLIENT_BAG_SIZE 36
#define NORMAL_BASE_BAG_SIZE (18)
#define EQUIP_BAG_SIZE 6

// 最大包裹页上限
#define TOTAL_ITEM_BAGS 4
#define MAGIC_NUMBER_OK 99999


#define ITEM_BAG_STARTID   1
#define ITEM_BAG_REGION  100

#define ITEM_BAGID(region) (ITEM_BAG_STARTID + ITEM_BAG_REGION * region)

#define ITEM_BAG_NORMAL    ITEM_BAGID(0)
#define ITEM_BAG_TASK      ITEM_BAGID(1)
#define ITEM_BAG_EQUIP     ITEM_BAGID(2)
#define ITEM_BAG_STORAGE   ITEM_BAGID(3)
#define ITEM_BAG_BAOSHI    ITEM_BAGID(4)

#define SUMMON_BAG_NORMAL 1
#define SUMMON_BAG_FIGHT  101    

#define ITEM_BAGS ({ITEM_BAG_NORMAL, ITEM_BAG_TASK, ITEM_BAG_EQUIP, ITEM_BAG_STORAGE, ITEM_BAG_BAOSHI })

#define CLIENT_ITEM_BAGS  ({ITEM_BAG_NORMAL, ITEM_BAG_TASK, ITEM_BAG_EQUIP, ITEM_BAG_STORAGE, ITEM_BAG_BAOSHI })

#define SUMMON_BAGS ({SUMMON_BAG_NORMAL, SUMMON_BAG_FIGHT,})

/*
int isItemBag(int iBagId)
{
	return iBagId == ITEM_BAG_NORMAL;
}
*/

int isNormalItemBag(int iBagId)
{
	return iBagId == ITEM_BAG_NORMAL;
}

int isEquipBag(int bagId)
{
	return (bagId >= ITEM_BAG_EQUIP && bagId < (ITEM_BAG_EQUIP + ITEM_BAG_REGION));
}

int getItemBagDefaultBaseSize(int bagId)
{
	if (isNormalItemBag(bagId)) {
		return NORMAL_BASE_BAG_SIZE;
	}
	else if (isEquipBag(bagId)) {
		return EQUIP_BAG_SIZE;
	}
	return CLIENT_BAG_SIZE;
}

int getItemBagDefaultMaxSize(int bagId)
{
	if (isNormalItemBag(bagId)) {
		return CLIENT_BAG_SIZE;
	}
	else if (isEquipBag(bagId)) {
		return EQUIP_BAG_SIZE;
	}
	return CLIENT_BAG_SIZE;
}

// 装备的cType[0..(size-1)] 相等，表示同一个物品
int IsSameType(int iBagId1, int iBagId2)
{
	return 0;
}

int IsFightBag(int iBagId)
{
	return 1;
}

#endif
