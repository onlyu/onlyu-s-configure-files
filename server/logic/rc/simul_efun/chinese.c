//
static private string *sDigit = ({ "零", "十", "百", "千", "万", "亿", "兆" }); 
static private string *sNumber = ({ "零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十" });
static private string *SymTian = ({ "甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸" });
static private string *SymDi = ({ "子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥" });

varargs string chinese_number( int iNumLow, int iNumHigh )
{
        int iDiv, iMod;

        // 高位参数缺省时，默认高位参数值为零
        if( nullp(iNumHigh) ) iNumHigh = 0;

        // 由第一个参数（低位）的符号决定正负
        if( iNumLow < 0 && iNumHigh > 0 || iNumLow > 0 && iNumHigh < 0 ) iNumHigh = - iNumHigh;
        if( iNumLow < 0 || iNumHigh < 0 ) return "负" + chinese_number( - iNumLow, - iNumHigh );

        // 低位取值不能超过一亿
        iDiv = iNumLow / 100000000;
        iMod = iNumLow % 100000000;
        if( iDiv ) return chinese_number( iMod, iNumHigh + iDiv );

        // 先处理高位的情况
        if( iNumHigh )
        {
                if( !iNumLow )
                        return chinese_number(iNumHigh) + sDigit[5];
                else if( iNumLow < 10000000 )
                        return chinese_number(iNumHigh) + sDigit[5] + sDigit[0] + chinese_number(iNumLow);
                else
                        return chinese_number(iNumHigh) + sDigit[5] + chinese_number(iNumLow);
        }

        // 再处理低位的情况
        if( iNumLow < 11 ) return sNumber[iNumLow];
        if( iNumLow < 20 ) return sDigit[1] + sNumber[iNumLow - 10];
        if( iNumLow < 100 )             // 百以内
        {
                iDiv = iNumLow / 10;
                iMod = iNumLow % 10;
                if( !iMod ) return sNumber[iDiv] + sDigit[1];
                else return sNumber[iDiv] + sDigit[1] + sNumber[iMod];
        }
        if( iNumLow < 1000 )            // 千以内
        {
                iDiv = iNumLow / 100;
                iMod = iNumLow % 100;
                if( !iMod )
                        return sNumber[iDiv] + sDigit[2];
                else if( iMod < 10 )
                        return sNumber[iDiv] + sDigit[2] + sNumber[0] + chinese_number(iMod);
                else if( iMod < 20 )
                        return sNumber[iDiv] + sDigit[2] + sNumber[1] + chinese_number(iMod);
                else
                        return sNumber[iDiv] + sDigit[2] + chinese_number(iMod);
        }
        if( iNumLow < 10000 )           // 万以内
        {
                iDiv = iNumLow / 1000;
                iMod = iNumLow % 1000;
                if( !iMod )
                        return sNumber[iDiv] + sDigit[3];
                else if( iMod < 100 )
                        return sNumber[iDiv] + sDigit[3] + sDigit[0] + chinese_number(iMod);
                else
                        return sNumber[iDiv] + sDigit[3] + chinese_number(iMod);
        }
        if( iNumLow < 100000000 )       // 亿以内
        {
                iDiv = iNumLow / 10000;
                iMod = iNumLow % 10000;
                if( !iMod )
                        return chinese_number(iDiv) + sDigit[4];
                else if( iMod < 1000 )
                        return chinese_number(iDiv) + sDigit[4] + sDigit[0] + chinese_number(iMod);
                else
                        return chinese_number(iDiv) + sDigit[4] + chinese_number(iMod);
        }

        iDiv = iNumLow / 100000000;
        iMod = iNumLow % 100000000;
        if( !iMod )
                return chinese_number(iDiv) + sDigit[5];
        else if( iMod < 10000000 )
                return chinese_number(iDiv) + sDigit[5] + sDigit[0] + chinese_number(iMod);
        else
                return chinese_number(iDiv) + sDigit[5] + chinese_number(iMod);
}

// 函数：检查字串是否全部都是中文
int is_chinese(string str)
{
	int i;

	if (strlen(str) < 2) return 0;

	for (i = 0; i < strlen(str); i++)
	{
		if (str[i] < 161 || str[i] == 255) return 0;
		if (! (i % 2) && (str[i] < 176 || str[i] >= 248)) return 0;
	}

	return 1;
}
