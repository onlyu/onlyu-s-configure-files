# -*- coding: utf-8 -*-
'''
parsetable的key为原来xls文件中的标识那列
parsetable dict的value项一定是个数组,数组内部是多个字典
parsetable = {
    'parsename':[{'parse_file':u'原导入文件那列','parse_sheet':u'原导入sheet那列','parse_tool':u'原工具列表那列','parse_output':u'原导出文件那列'},]
    }
'''
SVN_URL_BASE = 'https://192.168.0.3/qtz/wg01/design/'

parsetable = {
    'npc':[{'parse_file':'data/世界NPC.xls','parse_sheet':'NPC总表','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/scene/data/npc_layout.c'},],
    'chat':[{'parse_file':'data/世界NPC.xls','parse_sheet':'闲话','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/scene/data/npc_chat.c'},],
    'scene':[{'parse_file':'data/世界设定.xls','parse_sheet':'场景设定','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/scene/data/scene_info.c'},],
    'grow_exp':[{'parse_file':'游戏数值/时间规划表.xls','parse_sheet':'时间规划表','parse_tool':'tools/autocode/common_parse.py','parse_output':'data/user/exp.c'},],
    'mission':[{'parse_file':'任务剧情/missions.xls','parse_sheet':'','parse_tool':'tools/autocode/mission/gen_all_mission.py','parse_output':'module/mission/missions/'},],
    'drama':[{'parse_file':'任务剧情/剧情.xls','parse_sheet':'','parse_tool':'tools/autocode/drama.py','parse_output':'data/drama.c'},],
    'item':[
	{'parse_file':'data/物品表.xls','parse_sheet':'','parse_tool':'tools/autocode/items/gen_all_item.py','parse_output':'char/item/'},
	{'parse_file':'data/物品表.xls','parse_sheet':'装备表','parse_tool':'tools/autocode/common_parse.py','parse_output':'data/equip_data.c'},
	{'parse_file':'data/物品表.xls','parse_sheet':'宝石表','parse_tool':'tools/autocode/common_parse.py','parse_output':'data/baoshi_data.c'},
	],
    'fuben':[
	{'parse_file':'data/副本.xls','parse_sheet':'明雷','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fuben/data/minglei.c'},
	{'parse_file':'data/副本.xls','parse_sheet':'站位','parse_tool':'tools/autocode/begin_end_parse.py','parse_output':'module/fuben/data/layout.c'},
	{'parse_file':'data/副本.xls','parse_sheet':'副本','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fuben/data/fuben.c'},
	{'parse_file':'data/副本.xls','parse_sheet':'奖励','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fuben/data/reward.c'},
	{'parse_file':'data/副本.xls','parse_sheet':'触发','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fuben/data/trigger.c'},
	{'parse_file':'data/副本.xls','parse_sheet':'明雷数值','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fuben/data/numeric.c'},
	{'parse_file':'data/副本.xls','parse_sheet':'副本小地图','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fuben/data/map.c'},
	],

    'skill':[
	{'parse_file':'data/技能表.xls','parse_sheet':'技能表','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fight/data/skill.c'},
	{'parse_file':'data/技能表.xls','parse_sheet':'技能输出状态','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fight/data/skill_status.c'},
	{'parse_file':'data/技能表.xls','parse_sheet':'状态表','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fight/data/status.c'},
	{'parse_file':'data/技能表.xls','parse_sheet':'状态关系表','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fight/data/status_rel.c'},
	{'parse_file':'data/神格技能表.xls','parse_sheet':'数值','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/fight/data/shenge_skill.c'},
	],
    'summon':[
	{'parse_file':'data/角色属性表.xls','parse_sheet':'伙伴初始属性','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/summon/summon_base_data.c'},
	],
    'worldmap':[{'parse_file':'data/世界地图.xls','parse_sheet':'世界地图','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/worldmap/city.c'},],	
	'equip': [
		{'parse_file':'游戏系统/Z-装备系统/洗练属性.xls','parse_sheet':'洗练属性表','parse_tool':'tools/autocode/common_parse.py','parse_output':'data/equip_xilian_data.c'},
		{'parse_file':'游戏系统/Z-装备系统/洗练属性.xls','parse_sheet':'强化金钱表','parse_tool':'tools/autocode/yapp.py ','parse_output':'data/equip_qianghua_cash_data.c'},
	],
    'zhaocaimao':[
	{'parse_file':'data/招财猫数值.xls','parse_sheet':'VIP次数','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/zhaocaimao/viptimes.c'},
	{'parse_file':'data/招财猫数值.xls','parse_sheet':'摇钱元宝','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/zhaocaimao/yuanbao.c'},
	{'parse_file':'data/招财猫数值.xls','parse_sheet':'等级银两','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/zhaocaimao/grade_money.c'},
	],
    'dazuo':[
	{'parse_file':'data/打坐数值表.xls','parse_sheet':'经验','parse_tool':'tools/autocode/common_parse.py','parse_output':'module/dazuo/data.c'},
	],
    'char':[
	{'parse_file':'data/角色属性表.xls','parse_sheet':'主角初始属性','parse_tool':'tools/autocode/common_parse.py','parse_output':'data/user/init_attr.c'},
	],
  
    'effect':[{'parse_file':'data/特效表.xls','parse_sheet':'特效表','parse_tool':'tools/autocode/common_parse.py','parse_output':'null'},],
    'prompt':[{'parse_file':'data/X小提示.xls','parse_sheet':'提示内容','parse_tool':'tools/autocode/common_parse.py','parse_output':'data/prompt_data.c'},],
    'sanjiecg':[
	{'parse_file':'data/三界闯关.xls','parse_sheet':'明雷','parse_tool':'tools/autocode/common_parse.py','parse_output':'activity/sanjiecg/data/minglei.c'},
	{'parse_file':'data/三界闯关.xls','parse_sheet':'场景','parse_tool':'tools/autocode/common_parse.py','parse_output':'activity/sanjiecg/data/scene.c'},
	],
}



	
