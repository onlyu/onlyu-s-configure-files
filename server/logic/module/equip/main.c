#include <equip.h>
#include <item_key.h>
#include <debug.h>
#include <macros.h>
#include <frames.h>
#include <item.h>
#include <debug.h>
#include <baoshi.h>
#include <user_key.h>
#include "/rc/rpc/equip.h"

static mapping staticEquipData = ([]);
static mapping staticXilianData = ([]);
static mapping xilianAttrTable = ([]);

static mapping staticBaoshiData = ([]);
static mapping staticGradeBaoshiData = ([]);

static mapping staticLocateDesc = ([
		EQUIP_LOCATE_WEAPON : "武器",
		EQUIP_LOCATE_HAT : "帽子",
		EQUIP_LOCATE_CLOTHES : "衣服",
		EQUIP_LOCATE_BELT : "磐带",
		EQUIP_LOCATE_BRACELET : "玉佩",
		EQUIP_LOCATE_SHOES : "鞋子",
	]);

static object _log;

void sendXilianInfo(int uid, int item);


#define QIANGHUA_LIMIT_GRADE    40
#define QIANGHUA_MAX_MIN        (20 * 60)

// 宝石升级所需要的宝石数量
#define BAOSHI_UPGRADE_AMOUNT   2

#define DIRECT_XILIAN_YUANBAO   10


#define  IS_EQUIP_TYPE(t)        (t[0..1] == "EQ")
#define  IS_BAOSHI_TYPE(t)       (t[0..1] == "BS")

#define  ASSERT_BAOSHI_ITEM(item) \
	if (!IS_BAOSHI_TYPE(GetItemSave(item, K_ITEM_TYPE))) { \
		return; \
	}

#define  ASSERT_EQUIP_ITEM(item) \
	if (!IS_EQUIP_TYPE(GetItemSave(item, K_ITEM_TYPE))) { \
		return; \
	}

int hasSameTypeBaoshi(mapping baoshiInfo, string type);
void sendBaoshiFreeFetchCnt(object user);

int getEquipLocate(string type)
}

int getEquipQianghuaValue(string type)
{
	return (ITEM_PATH + type)->getQianghuaValue();
}

mapping getEquipAttr(string type)
{
	return (ITEM_PATH + type)->getQianghuaAttr();
}

int getEquipColor(string type)
{
	return (ITEM_PATH + type)->getEquipColor();
}

int getEquipGrade(string type)
{
	return (ITEM_PATH + type)->getEquipGrade();
}

int getEquipType(string type)
{
	return (ITEM_PATH + type)->getEquipType();
}

mapping getEquipDesc(string type)
{
	return (ITEM_PATH + type)->getData();
}


void initXilianAttrTable()
{
	xilianAttrTable = ([]);
	foreach (int _index, mapping _data in staticXilianData) {
		mapping mpAttr = getOrSetMappingDepth(xilianAttrTable, ({ _data[K_EQUIP_GRADE], _data[K_EQUIP_TYPE], _data[K_EQUIP_COLOR] }));
		mpAttr["attributes"] = _data["attributes"];
		mpAttr["attrCnt"] = _data["attrCnt"];
		mpAttr[K_NEED_CASH] = _data[K_NEED_CASH];
	}
}

void initBaoshiGradeTable()
{
	staticGradeBaoshiData = ([]);

	foreach (string _type, mapping _data in staticBaoshiData) {
		mapping mpGradeData = getOrSetMapping(staticGradeBaoshiData, _data[K_BAOSHI_TYPE]);
		mpGradeData[_data[K_BAOSHI_GRADE]] = _type;
	}
}

int randomValue(int min, int max)
{
	return min + random(max - min);
}

mapping getRandXilianAttr(int item)
{
	string type = GetItemSave(item, K_ITEM_TYPE);
	int grade = getEquipGrade(type);
	int equipType = getEquipType(type);
	int color = getEquipColor(type);

	mapping mpAttr = getmappingdepth(xilianAttrTable, ({grade, equipType, color}));

	if (!mapp(mpAttr)) {
		return 0;
	}

	mixed *randArr = rand_x_array(keys(mpAttr["attributes"]), mpAttr["attrCnt"]);

	mapping res = ([]);

	foreach (string attrKey in randArr) {
		res[attrKey] = randomValue(mpAttr["attributes"][attrKey][0], mpAttr["attributes"][attrKey][1]);
	}
	return res;
}

void initEquip(int item)
{
	// 设置洗练属性
	mapping xilianAttr = getRandXilianAttr(item);

	if (sizeof(xilianAttr)) {
		SetItemSave(item, K_ITEM_XILIAN_ATTR, xilianAttr);
		/*
		foreach (string attrKey, int value in xilianAttr) {
			SetItemSave(item, attrKey, value);
		}
		*/
	}
}

// 获取装备属性
mapping getEquipAttribute(int item)
{
	string type = GetItemSave(item, K_ITEM_TYPE);
	mapping res = ([]);

	if (!IS_EQUIP_TYPE(type)) return res;

	int qianghuaCnt = GetItemSave(item, K_ITEM_QIANGHUA);
	int qianghuaVal = getEquipQianghuaValue(type);
	mapping baseAttr = getEquipAttr(type);


	mapping xilianAttr = GetItemSave(item, K_ITEM_XILIAN_ATTR);
	// 洗练属性
	// TODO:
	if (sizeof(xilianAttr)) {
		foreach (mixed _key, int _val in xilianAttr) {
			if (_val > 0) {
				res[_key] = _val;
			}
		}
	}

	// 基础属性
	foreach (mixed _key, int _val in baseAttr) {
		res[_key] += _val + qianghuaVal * qianghuaCnt;
	}

	// 宝石
	mapping baoshis = GetItemSave(item, K_ITEM_BAOSHI);
	if (sizeof(baoshis)) {
		foreach (string _type in keys(baoshis)) {
			if (undefinedp(staticBaoshiData[_type])) {
				debug_message("%s not exist!", _type);
				continue;
			}
			mapping attr = staticBaoshiData[_type][K_BAOSHI_ATTR];
			foreach (mixed _key, int _val in attr) {
				if (_val > 0) {
					res[_key] += _val;
				}
			}
		}
	}
	return res;
}

mapping getBagEquipAttr(object user, int bagId)
{
	int *items = user->getAllItemsInBag(bagId);
	mapping attr = ([]);

	foreach (int item in items) {
		mapping res = getEquipAttribute(item);
		if (sizeof(res))  {
			foreach (mixed key, int _val in res) {
				attr[key] += _val;
			}
		}
	}
	return attr;
}


// 计算某背包的装备属性
void calcUserEquipAttr(object user, int bagId)
{
	if (!isEquipBag(bagId)) return;

	// 装备是那个位置的
	if (bagId == ITEM_BAG_EQUIP) {
		user->calculateAllAttr();
		user->sendHeroSpeciInfo();
	} else {
		// mapping attr = getBagEquipAttr(user, bagId);
		// 找到对应的召唤兽
		int summonId = "module/summon/util"->findSummonIdByEquipBagId(user, bagId);
		if (summonId) {
			"module/summon/util"->calculateSummon(summonId);
			"module/summon/util"->rpcSummonInfo(user->getId(), summonId);
		}
	}
}



// 穿上装备
void wieldEquip(object user, int item, int bagId, int locate)
{
	int uid = user->getId();
	string type = GetItemSave(item, K_ITEM_TYPE);

	if (!isEquipBag(bagId)) return;

	if (!IS_EQUIP_TYPE(type)) {
		user->tellTips(_("装备类型出错"));
		debug_message("[%d] wield unvalid equip[%s]", uid, type);
		return;
	}

	// 是否等级够
	int grade = getEquipGrade(type);

	if (user->getGrade() < grade) {
		user->tellMe(_("你的等级不够，无法装备!"));
		return;
	}
	
	if (getEquipLocate(type) != locate) {
		locate = getEquipLocate(type);
	}

	// locate 即为位置
	int oriItem = user->getItemByPos(bagId, locate);

	if (oriItem) {

		// 先删除，保证一定有位置可以放现在的装备
		user->RemoveItem(item);
		user->MoveItem(oriItem, ITEM_BAG_NORMAL);
	}

	user->MoveItem(item, bagId, locate);

	calcUserEquipAttr(user, bagId);
}

// 卸下装备
void unwieldEquip(object user, int item, int pos)
{
	if (user->GetEmptyPos(ITEM_BAG_NORMAL) <= 0) {
		user->tellMe(_("你的物品栏满了!"));
		return;
	}

	int uid = user->getId();
	string type = GetItemSave(item, K_ITEM_TYPE);
	int bagId = GetItemTemp(item, KT_ITEM_BAG);
	int locate = getEquipLocate(type);

	int oriItem = user->getItemByPos(bagId, locate);
	
	if (oriItem != item) {
		debug_message("[%d] unwield item [%d] error", user->getId(), item);
		return;
	}

	user->MoveItem(item, ITEM_BAG_NORMAL, pos);

	calcUserEquipAttr(user, bagId);
}


void sendEquipItemInfo(int uid, int item)
{
	string type = GetItemSave(item, K_ITEM_TYPE);

	int qianghua = GetItemSave(item, K_ITEM_QIANGHUA);

	class equip_value_t *values = ({});


	mapping xilianAttr = GetItemSave(item, K_ITEM_XILIAN_ATTR);

	// 洗练属性
	if (sizeof(xilianAttr)) {
		foreach (mixed _key, int _val in xilianAttr) {
			class equip_value_t _value = new(class equip_value_t);
			if (_val > 0) {
				_value->key = _key;
				_value->value = _val;
				values += ({ _value });
			}
		}
	}

	string *baoshis = ({});

	mapping baoshiInfo = GetItemSave(item, K_ITEM_BAOSHI);

	if (sizeof(baoshiInfo)) {
		baoshis = keys(baoshiInfo);
	}
	rpc_client_equip_item_info(uid, item, qianghua, values, baoshis);
}

// 发送装备属性
void sendEquipInfo(int uid, int item)
{
	sendEquipItemInfo(uid, item);
	sendXilianInfo(uid, item);
}


// 发送装备属性
void sendEquipDesc(int uid, string type)
{
	int color = getEquipColor(type);
	int locate = getEquipLocate(type);
	int grade = getEquipGrade(type);
	int qianghuaVal = getEquipQianghuaValue(type);
	mapping attr = getEquipAttr(type);

	class equip_value_t *values = ({});

	foreach (mixed _key, mixed _val in attr) {
		class equip_value_t _value = new(class equip_value_t);
		_value->key = _key;
		_value->value = _val;
		values += ({ _value });
	}

	rpc_client_equip_desc(uid, type, grade, qianghuaVal, locate, color, values);
}


#define hasEnoughItem(user, desc) \
	(sizeof(desc) && user->getSameTypeItemAmount(desc[0]) > desc[1])

int hasEnoughCailiao(object user, mapping data)
{
	mixed *lowCailiao = data[K_NEED_L_LEV_CAILIAO];
	mixed *highCailiao = data[K_NEED_H_LEV_CAILIAO];
	mixed *tupus = data[K_NEED_TUPU];
	int cash = data[K_NEED_CASH];

	if (!hasEnoughItem(user, lowCailiao)) {
		user->tellMe(_("%s材料不足!"), Import("ITEM_UTIL")->getNameByType(lowCailiao[0]));
		return 0;
	}

	if (!hasEnoughItem(user, highCailiao)) {
		user->tellMe(_("%s材料不足!"), Import("ITEM_UTIL")->getNameByType(highCailiao[0]));
		return 0;
	}

	if (!hasEnoughItem(user, tupus)) {
		user->tellMe(_("%s图谱不足!"), Import("ITEM_UTIL")->getNameByType(tupus[0]));
		return 0;
	}

	if (!user->hasCash(cash)) {
		user->tellMe(_("金钱不足"));
		return 0;
	}

	return 1;
}

void doDeductCailiao(object user, mapping data)
{
	// TODO:
	return ;
}

int getEquipQianghuaCD(object user)
{
	mapping data = user->getEquip();
	if (!data[K_QIANGHUA_CD]) {
		return 0;
	}
	int remain = data[K_QIANGHUA_CD] - time();
	if (remain < 0) return 0;
	return remain;
}

void sendEquipQianghuaCD(object user)
{
	rpc_client_equip_qianghua_cd(user->getId(), getEquipQianghuaCD(user), QIANGHUA_MAX_MIN);
}


void addEquipQianghuaCD(object user, int min)
{
	mapping data = user->getEquip();

	if (data[K_QIANGHUA_CD] < time()) {
		data[K_QIANGHUA_CD] = time();
	}

	data[K_QIANGHUA_CD] += min * 60;
	sendEquipQianghuaCD(user);
}

static int getTypeQianghuaCash(int locate, int color, int qianghua)
{
	return "data/equip_qianghua_cash_data"->getQianghuaCash(qianghua, locate, color);
}

int getQianghuaCash(int item)
{
	int qianghua = GetItemSave(item, K_ITEM_QIANGHUA);
	string type = GetItemSave(item, K_ITEM_TYPE);
	int color = getEquipColor(type);
	int locate = getEquipLocate(type);
	return getTypeQianghuaCash(locate, color, qianghua + 1);
}


void sendEquipQianghuaInfo(object user, int item)
{
	int qianghuaCash = getQianghuaCash(item);
	rpc_client_equip_qianghua_info(user->getId(), item, qianghuaCash);
}

int getAllQianghuaCash(int item)
{
	int qianghua = GetItemSave(item, K_ITEM_QIANGHUA);

	string type = GetItemSave(item, K_ITEM_TYPE);
	int color = getEquipColor(type);
	int locate = getEquipLocate(type);

	int allCash = 0;

	while (qianghua > 0) {
		allCash += getTypeQianghuaCash(locate, color, qianghua);
		qianghua -= 1;
	}
	return allCash;
}

// 强化装备
void doQianghuaEquip(object user, int item)
{
	int qianghua = GetItemSave(item, K_ITEM_QIANGHUA);
	int grade = user->getGrade();

	if (grade < qianghua) {
		user->tellMe(_("装备已经强化到目前最高等级，请先提升人物等级。"));
		return;
	}

	if (grade < QIANGHUA_LIMIT_GRADE && (getEquipQianghuaCD(user) >= QIANGHUA_MAX_MIN)) {
		user->tellMe(_("你暂时无法进行装备强化!"));
		return;
	}

	int cash = getQianghuaCash(item);
	if (!cash) return;

	if (cash && !user->hasCash(cash)) {
		user->tellMe(_("金钱不足"));
		return;
	}

	int res = user->subCash(cash);
	user->tellMe(_("你消耗了%d金钱"), res);

	SetItemSave(item, K_ITEM_QIANGHUA, qianghua + 1);

	sendEquipInfo(user->getId(), item);

	int bagId = GetItemTemp(item, KT_ITEM_BAG);
	calcUserEquipAttr(user, bagId);

	if (grade < QIANGHUA_LIMIT_GRADE) {
		// 增加强化CD
		addEquipQianghuaCD(user, 5);
	}
}

mapping getQianghuaTransferInfo(object user, int equipItem, int transferItem)
{
	int allCash = getAllQianghuaCash(transferItem);

	if (allCash <= 0) {
		user->tellMe(_("你的装备没有强化!"));
		return ([]);
	}

	int qianghua = GetItemSave(equipItem, K_ITEM_QIANGHUA);
	int grade = user->getGrade();

	if (qianghua >= grade) {
		user->tellMe(_("装备已经强化到目前最高等级，请先提升人物等级。"));
		return ([]);
	}

	string type = GetItemSave(equipItem, K_ITEM_TYPE);
	int color = getEquipColor(type);
	int locate = getEquipLocate(type);

	allCash = allCash * 0.8;

	while (qianghua < grade) {
		int qianghuaCash = getTypeQianghuaCash(locate, color, qianghua + 1);
		if (!qianghuaCash || !allCash || qianghuaCash > allCash ) {
			break;
		}
		allCash -= qianghuaCash;
		qianghua += 1;
	}

	// 宝石继承
	mapping baoshiInfo = GetItemSave(equipItem, K_ITEM_BAOSHI);
	mapping transferBaoshiInfo = GetItemSave(equipItem, K_ITEM_BAOSHI);
	
	mixed *baoshis = ({});
	if (sizeof(transferBaoshiInfo)) {
		int transfer = 0;
		foreach (string _t in keys(transferBaoshiInfo)) {
			if (hasSameTypeBaoshi(baoshiInfo, _t)) {
				continue;
			}
			baoshis += ({ _t });
		}	
	}
	return ([ "qianghua" : qianghua, "remainCash": allCash, "baoshis" : baoshis ]);
}

void doQianghuaTransferConfirm(object user, int result, int equipItem, int transferItem)
{
	int uid = user->getId();
	assert( GetItemTemp(equipItem, KT_ITEM_OWNER) == uid );
	assert( GetItemTemp(transferItem, KT_ITEM_OWNER) == uid );

	mapping res = getQianghuaTransferInfo(user, equipItem, transferItem);

	if (!sizeof(res)) return;

	mixed *baoshis = res["baoshis"];
	int qianghua = res["qianghua"];
	int remainCash = res["remainCash"];

	if (sizeof(baoshis)) {
		mapping baoshiInfo = GetItemSave(equipItem, K_ITEM_BAOSHI);
		mapping transferBaoshiInfo = GetItemSave(equipItem, K_ITEM_BAOSHI);
		foreach (string _baoshi in baoshis) {
			baoshiInfo[_baoshi] = 1;
			map_delete(transferBaoshiInfo, _baoshi);
		}
	}

	if (qianghua != GetItemSave(equipItem, K_ITEM_QIANGHUA)) {
		SetItemSave(equipItem, K_ITEM_QIANGHUA, qianghua);
		SetItemSave(transferItem, K_ITEM_QIANGHUA, 0);
		if (remainCash > 0) {
			user->addCash(remainCash);
		}
		user->tellMe(_("%s强化等级升到%d级"), getItemName(GetItemSave(equipItem, K_ITEM_TYPE)), qianghua);
		user->tellMe(_("%s强化等级降为%d级"), getItemName(GetItemSave(equipItem, K_ITEM_TYPE)), 0);
		sendEquipItemInfo(user->getId(), equipItem);
		sendEquipItemInfo(user->getId(), transferItem);
	}
}

// 强化转移
void doQianghuaTransfer(object user, int equipItem, int transferItem)
{
	int uid = user->getId();
	assert( GetItemTemp(equipItem, KT_ITEM_OWNER) == uid );
	assert( GetItemTemp(transferItem, KT_ITEM_OWNER) == uid );

	mapping res = getQianghuaTransferInfo(user, equipItem, transferItem);

	if (!sizeof(res)) return;

	mixed *baoshis = res["baoshis"];
	int qianghua = res["qianghua"];
	int remainCash = res["remainCash"];

	if (sizeof(baoshis) || qianghua != GetItemSave(equipItem, K_ITEM_QIANGHUA)) {
		string message = _("将辅装备的80%强化金钱, 重新强化到主装备，辅装备强化等级回到0级");

		Import("CONFIRM_DLG")->sendConfirmDialog(uid, _("装备继承"), message, "确定", "取消", "doQianghuaTransferConfirm", ({ equipItem, transferItem }));
		return;
	} else {
		user->tellMe(_("无法继承"));
	}
}

// 装备进阶
void doAdvanceEquip(object user, int item)
{
	string type = GetItemSave(item, K_ITEM_TYPE);

	mapping mpData = getEquipDesc(type);

	if (!sizeof(mpData[K_EQUIP_PRODUCT])) {
		user->tellMe(_("该装备不允许进阶"));
		return;
	}

	if (!hasEnoughCailiao(user, mpData)) {
		return;
	}

	// TODO: 扣除材料
}


void resetData()
{
	staticEquipData = "data/equip_data.c"->get_data();
	staticXilianData = "data/equip_xilian_data"->get_data();
	staticBaoshiData = "data/baoshi_data"->get_data();
	initXilianAttrTable();
	initBaoshiGradeTable();
}

void loginCheck(object user)
{
	sendEquipQianghuaCD(user);
	sendBaoshiFreeFetchCnt(user);
}

void create()
{
	_log = Import("LOG")->New("equip");
	resetData();
}

// 穿上装备
void rpc_server_wield_equip(object user, int item, int bagId, int locate)
{
	assert( GetItemTemp(item, KT_ITEM_OWNER) == user->getId() );
	wieldEquip(user, item, bagId, locate);
}

// 卸下装备
void rpc_server_unwield_equip(object user, int item, int pos)
{
	assert( GetItemTemp(item, KT_ITEM_OWNER) == user->getId() );
	unwieldEquip(user, item, pos);
}

// 强化
void rpc_server_equip_qianghua(object user, int item)
{
	assert( GetItemTemp(item, KT_ITEM_OWNER) == user->getId() );
	doQianghuaEquip(user, item);
}


// 强化req
void rpc_server_equip_qianghua_info_req(object user, int item)
{
	sendEquipQianghuaCD(user);
	sendEquipQianghuaInfo(user, item);
}

// ====================装备洗练=================


int getXilianData(int item, mixed key)
{
	string type = GetItemSave(item, K_ITEM_TYPE);
	int grade = getEquipGrade(type);
	int equipType = getEquipType(type);
	int color = getEquipColor(type);
		
	mapping attrTable = getmappingdepth(xilianAttrTable, ({grade, equipType, color}));
	if (mapp(attrTable)) {
		return attrTable[key];
	} 
	return 0;
}

void doXilianNormal(object user, int item)
{
	int res = 0;

	// 设置洗练属性
	int cash = getXilianData(item, K_NEED_CASH);
	
	if (cash && !user->hasCash(cash)) {
		user->tellMe(_("金钱不足"));
		return;
	}

	// 设置洗练属性
	mapping xilianAttr = getRandXilianAttr(item);

	if (sizeof(xilianAttr)) {
		res = user->subCash(cash);
		user->tellMe(_("你消耗了%d金钱"), res);
		SetItemSave(item, K_ITEM_XILIAN_ATTR_REP, xilianAttr);
	}
	sendXilianInfo(user->getId(), item);
}

void doXilianAttrReplace(object user, int item)
{
	mapping attrRep = GetItemSave(item, K_ITEM_XILIAN_ATTR_REP);

	if (sizeof(attrRep)) {
		SetItemSave(item, K_ITEM_XILIAN_ATTR, attrRep);
		SetItemSave(item, K_ITEM_XILIAN_ATTR_REP, ([]));
		// sendXilianInfo(user->getId(), item);
		sendEquipInfo(user->getId(), item);
	}
}

void doXilianAttrDirect(object user, int item)
{
	string type = GetItemSave(item, K_ITEM_TYPE);
	int grade = getEquipGrade(type);
	int equipType = getEquipType(type);
	int color = getEquipColor(type);
	
	mapping xilianAttr = GetItemSave(item, K_ITEM_XILIAN_ATTR);

	if (!sizeof(xilianAttr)) {
		initEquip(item);
		return;
	}
	// 设置洗练属性
	int yuanbao = DIRECT_XILIAN_YUANBAO;

	if (yuanbao && user->getTotalYB() < yuanbao) {
		user->tellMe(_("元宝不足"));
		return;
	}

	user->minusYB(yuanbao);
	user->tellMe(_("你消耗了%d元宝"), yuanbao);

	mapping newAttr = ([]);

	mapping attrTable = getmappingdepth(xilianAttrTable, ({grade, equipType, color}));

	foreach (string key in keys(xilianAttr)) {
		newAttr[key] = randomValue(attrTable["attributes"][key][0], attrTable["attributes"][key][1]);
	}

	SetItemSave(item, K_ITEM_XILIAN_ATTR_REP, newAttr);
	sendXilianInfo(user->getId(), item);
}

void sendXilianInfo(int uid, int item)
{

	mapping mpAttr = GetItemSave(item, K_ITEM_XILIAN_ATTR_REP);

	class equip_value_t *attr = ({});


	if (sizeof(mpAttr)) {
		foreach (mixed _key, int _val in mpAttr) {
			class equip_value_t _value = new(class equip_value_t);
			if (_val > 0) {
				_value->key = _key;
				_value->value = _val;
				attr += ({ _value });
			}
		}
	}

	int cash = getXilianData(item, K_NEED_CASH);

	rpc_client_equip_xilian_info(uid, item, cash, DIRECT_XILIAN_YUANBAO, attr);
}

// ========================================================
// 宝石相关

int hasSameTypeBaoshi(mapping baoshiInfo, string type)
{
	if (!sizeof(baoshiInfo)) return 0;

	mapping types = ([]);
	foreach (string _t in keys(baoshiInfo)) {
		types[staticBaoshiData[_t][K_BAOSHI_TYPE]] += 1;
	}

	string baoshiType = staticBaoshiData[type][K_BAOSHI_TYPE];
	
	return (!undefinedp(types[baoshiType]));
}

void doEquipAddBaoshi(object user, int equipItem, int baoshiItem)
{
	int uid = user->getId();

	ASSERT_BAOSHI_ITEM(baoshiItem)
	string type = GetItemSave(baoshiItem, K_ITEM_TYPE);

	if (!IS_BAOSHI_TYPE(type)) {
		return;
	}

	mapping baoshiInfo = GetItemSave(equipItem, K_ITEM_BAOSHI);

	if (hasSameTypeBaoshi(baoshiInfo, type)) {
		user->tellMe(_("这把武器已经镶有同类宝石!"));
		return;
	}

	if (!mapp(baoshiInfo)) {
		SetItemSave(equipItem, K_ITEM_BAOSHI, ([]));
		baoshiInfo = GetItemSave(equipItem, K_ITEM_BAOSHI);
	}
	baoshiInfo[type] = 1;

	// user->RemoveItem(baoshiItem);
	Import("ITEM_UTIL")->DecAmount(baoshiItem, 1);

	int bagId = GetItemTemp(equipItem, KT_ITEM_BAG);
	calcUserEquipAttr(user, bagId);
	sendEquipItemInfo(uid, equipItem);
}

string getBaoshiUpGradeType(string type)
{
	string baoshiType = staticBaoshiData[type][K_BAOSHI_TYPE];
	int grade = staticBaoshiData[type][K_BAOSHI_GRADE];

	string upGradeType = staticGradeBaoshiData[baoshiType][grade + 1];

	if (undefinedp(upGradeType)) {
		return "";
	}
	return upGradeType;
}

void doUpGradeEquipBaoshi(object user, int equipItem, string type)
{
	int uid = user->getId();

	ASSERT_EQUIP_ITEM(equipItem)

	mapping baoshiInfo = GetItemSave(equipItem, K_ITEM_BAOSHI);

	assert(!undefinedp(baoshiInfo[type]));

	string upGradeType = getBaoshiUpGradeType(type);
	if (!sizeof(upGradeType)) {
		user->tellMe(_("你的宝石已经无法升级了!"));
		return;
	}

	// 找到同类的物品
	if (user->getSameTypeItemAmount(type, ITEM_BAG_BAOSHI) <= 0) {
		user->tellMe(_("你的宝石不足!"));
		return;
	}


	user->delSameTypeItemAmount(type, 1, ITEM_BAG_BAOSHI);

	map_delete(baoshiInfo, type);
	baoshiInfo[upGradeType] = 1;

	int bagId = GetItemTemp(equipItem, KT_ITEM_BAG);
	calcUserEquipAttr(user, bagId);
	sendEquipItemInfo(uid, equipItem);
	_log->Log("[%d] equip upgrade baoshi %s->%s", user->getId(), type, upGradeType);
}

void doEquipRemoveBaoshi(object user, int equipItem, string baoshiType)
{
	int uid = user->getId();

	ASSERT_EQUIP_ITEM(equipItem)

	mapping baoshiInfo = GetItemSave(equipItem, K_ITEM_BAOSHI);

	if (!sizeof(baoshiType)) return;

	if (mapp(baoshiInfo) && !undefinedp(baoshiInfo[baoshiType])) {
		 if (Import("ITEM_UTIL")->addNewItem(user, baoshiType, 1, ITEM_BAG_BAOSHI) > 0) {
			map_delete(baoshiInfo, baoshiType);
			// _log->Log("[%d] equip remove baoshi %s", uid, baoshiType);
		 } else {
			 user->tellMe(_("你的物品栏满了，无法卸下装备!"));
			 return;
		 }
	}

	int bagId = GetItemTemp(equipItem, KT_ITEM_BAG);
	calcUserEquipAttr(user, bagId);
	sendEquipItemInfo(uid, equipItem);
}

int doUpGradeBaoshi(object user, int itemid, int cnt)
{
	int uid = user->getId();

	if (!is_item_online(itemid)) return 0;

	string type = GetItemSave(itemid, K_ITEM_TYPE);

	if (!IS_BAOSHI_TYPE(type)) return 0;

	string upGradeType = getBaoshiUpGradeType(type);

	if (!sizeof(upGradeType)) {
		user->tellMe(_("这颗宝石已经是最高级别了!"));
		return 0;
	}

	int amount = user->getSameTypeItemAmount(type, ITEM_BAG_BAOSHI);

	if (amount < BAOSHI_UPGRADE_AMOUNT) {
		user->tellMe(_("你的宝石不足无法升级!"));
		return 0;
	}

	int newAmount = amount / BAOSHI_UPGRADE_AMOUNT;

	if (newAmount > 0) {
		Import("ITEM_UTIL")->addNewItem(user, upGradeType, newAmount, ITEM_BAG_BAOSHI);
		user->delSameTypeItemAmount(type, newAmount * BAOSHI_UPGRADE_AMOUNT, ITEM_BAG_BAOSHI);
		user->tellMe(_("升级成功!"));
		_log->Log("[%d] upgrade baoshi %s->%s %d", user->getId(), type, upGradeType, newAmount);
		return 1;
	}
	return 0;
}


void doUpGradeBaoshiCertainGrade(object user, int grade)
{
	int *items = user->getAllItemsInBag(ITEM_BAG_BAOSHI);

	int *baoshiItems = ({});
	mapping allBaoshi = ([]);

	foreach (int item in items) {
		string type = GetItemSave(item, K_ITEM_TYPE);
		if (!IS_BAOSHI_TYPE(type)) continue;
		int baoshiGrade = staticBaoshiData[type][K_BAOSHI_GRADE];
		if (baoshiGrade != grade) continue;
		int amount = GetItemSave(item, K_ITEM_AMOUNT);
		allBaoshi[type] += amount;
	}

	if (!sizeof(allBaoshi)) return;

	foreach (string _type, int _amount in allBaoshi) {
		// 无法升级
		string upgradeType = getBaoshiUpGradeType(_type);
		if (!sizeof(upgradeType)) continue;
		int newAmount = to_int(_amount / BAOSHI_UPGRADE_AMOUNT);
		if (newAmount <= 0) continue;
		user->delSameTypeItemAmount(_type, newAmount * BAOSHI_UPGRADE_AMOUNT, ITEM_BAG_BAOSHI);
		Import("ITEM_UTIL")->addNewItem(user, upgradeType, newAmount, ITEM_BAG_BAOSHI);
		_log->Log("[%d] equip upgrade baoshi %s->%s %d", user->getId(), _type, upgradeType, newAmount);
	}
}

void doUpGradeAllBaoshis(object user, int grade)
{
	int _grade = 1;
	while (_grade < grade) {
		doUpGradeBaoshiCertainGrade(user, _grade++);
	}
}


// ========================================================
// 宝石获得
// K_USER_DAY_FREE_BAOSHI

#define  BAOSHI_FREE_FETCH_CNT      3
#define  BAOSHI_DEFAULT_FETCH_CNT   50

string getRandBaoshi()
{
	int rand = random(100);

	// 1 ---> 3
	// 10---> 2
	// 89---> 1

	int grade = (rand < 1) ? 3 : ((rand < 11) ? 2 : 1);
	string randBaoshiType = rand_array(keys(staticGradeBaoshiData));

	return staticGradeBaoshiData[randBaoshiType][grade];
}

void doFetchBaoshi(object user, int cnt, int free)
{
	// 默认获得宝石数量:50
	if (cnt == -1) {
		cnt = BAOSHI_DEFAULT_FETCH_CNT;
	}

	if (cnt <= 0) return;

	if (!free) { 
		if (user->getTotalYB() < cnt) {
			user->tellMe(_("你的元宝不足，无法淬炼宝石!"));
			return;
		}
		user->minusYB(cnt);
		user->tellMe(_("你消耗了%d元宝"), cnt);
	}

	mapping baoshis = ([]);

	while (cnt >= 0) {
		string randType = getRandBaoshi();
		baoshis[randType] += 1;
		cnt -= 1;
	}

	if (sizeof(baoshis)) {
		foreach (string _type, int _amount in baoshis) {
			Import("ITEM_UTIL")->addNewItem(user, _type, _amount, ITEM_BAG_BAOSHI);
		}
	}
}

void doFetchBaoshiFree(object user)
{
	mapping dayData = user->getDayData();
	int cnt = dayData[K_USER_DAY_FREE_BAOSHI];
	if (cnt < BAOSHI_FREE_FETCH_CNT) {
		dayData[K_USER_DAY_FREE_BAOSHI] += 1;
		doFetchBaoshi(user, 1, 1);
		sendBaoshiFreeFetchCnt(user);
		return;
	}
	doFetchBaoshi(user, 1, 0);
}

void sendBaoshiFreeFetchCnt(object user)
{
	mapping dayData = user->getDayData();
	int cnt = dayData[K_USER_DAY_FREE_BAOSHI];
	rpc_client_baoshi_free_fetch_cnt(user->getId(), BAOSHI_FREE_FETCH_CNT - 1);
}


// ========================================================
// 普通洗练
// 重新随机生成
void rpc_server_equip_xilian_normal(object user, int item)
{
	assert( GetItemTemp(item, KT_ITEM_OWNER) == user->getId() );
	doXilianNormal(user, item);
}

// 定向洗练
// 不改属性
void rpc_server_equip_xilian_direct(object user, int item)
{
	assert( GetItemTemp(item, KT_ITEM_OWNER) == user->getId() );
	doXilianAttrDirect(user, item);
}

// 替换属性
void rpc_server_equip_xilian_replace_attr(object user, int item)
{
	assert( GetItemTemp(item, KT_ITEM_OWNER) == user->getId() );
	doXilianAttrReplace(user, item);
}

// 嵌上宝石
void rpc_server_equip_add_baoshi(object user, int equipItem, int baoshiItem)
{
	int uid = user->getId();
	assert( GetItemTemp(equipItem, KT_ITEM_OWNER) == uid );
	assert( GetItemTemp(baoshiItem, KT_ITEM_OWNER) == uid );
	doEquipAddBaoshi(user, equipItem, baoshiItem);
}

// 卸下宝石
void rpc_server_equip_remove_baoshi(object user, int equipItem, string baoshiType)
{
	int uid = user->getId();
	assert( GetItemTemp(equipItem, KT_ITEM_OWNER) == uid );
	doEquipRemoveBaoshi(user, equipItem, baoshiType);
}

// 合并宝石
void rpc_server_equip_merge_baoshi(object user, int baoshiItem, int cnt)
{
	int uid = user->getId();
	assert( GetItemTemp(baoshiItem, KT_ITEM_OWNER) == uid );
	doUpGradeBaoshi(user, baoshiItem, cnt);
}

void rpc_server_baoshi_upgrade(object user, int equipItem, string baoshiType)
{
	int uid = user->getId();
	assert( GetItemTemp(equipItem, KT_ITEM_OWNER) == uid );
	doUpGradeEquipBaoshi(user, equipItem, baoshiType);
}

// 批量升级宝石
void rpc_server_baoshi_batch_upgrade(object user, int grade)
{
	doUpGradeAllBaoshis(user, grade);
}

// 装备继承
void rpc_server_equip_cash_transfer(object user, int equipItem, int transferItem)
{
	doQianghuaTransfer(user, equipItem, transferItem);
}

// 获得宝石
void rpc_server_baoshi_fetch(object user, int cnt)
{
	doFetchBaoshi(user, cnt, 0);
}

// 免费获得宝石
void rpc_server_baoshi_free_fetch(object user)
{
	doFetchBaoshiFree(user);
}

