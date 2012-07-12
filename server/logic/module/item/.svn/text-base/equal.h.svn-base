#include <item_key.h>

static mapping mpEqualTypes = 
// -------------------  Auto Generate Begin --------------------
([
                "YP02003":"YP01003", 
                "YP02004":"YP01004", 
                "YP02005":"YP01005", 
                "YP02006":"YP01006", 
                "YP02007":"YP01007", 
                "YP02013":"YP01013", 
                "YP02014":"YP01014", 
                "YP02015":"YP01015", 
                "YP02016":"YP01016", 
                "YP02017":"YP01017", 
                "YP02021":"YP01021", 
                "ZH00055":"ZH00010", 
                "ZH00056":"ZH00011", 
                "ZH00057":"ZH00012", 
                "ZH00063":"ZH00008", 
                "ZH00064":"ZH00009", 
                "ZH00086":"ZH00006", 
                "ZH00097":"ZH00005", 
                "ZH00101":"ZH00296", 
                "ZH00102":"ZH00297", 
                "ZH00104":"ZH00298", 
                "ZH00105":"ZH00299", 
                "ZH00107":"ZH00300", 
                "ZH00108":"ZH00301", 
        ])
// -------------------  Auto Generate End   --------------------
;

string GetEqualType(string cType)
{
	object oItem;
	string cEqualType;
	int itemid;
	
	cEqualType = mpEqualTypes[cType];
	if ( cEqualType )
		return cEqualType;

	//todo: fix me !
	itemid = Import("UTIL")->CreateItem(cType);
	cEqualType = GetItemTemp(itemid, K_EQUK_ITEM);
	cEqualType = cEqualType ? cEqualType : "NONE";
	mpEqualTypes[cType] = cEqualType;

	DestroyItem(itemid);

	return cEqualType;
}
