#include <common_key.h>

#include "/module/fight/warrior.h"
#include "/module/fight/skill.h"
#include "/module/fight/status.h"

#define FTO_HEADER "header"
#define FTO_VERSION "version"
#define FTO_META "meta"
#define FTO_FIGHT "fight"
#define FTO_PLACE "place"
#define FTO_INIT "init"
#define FTO_CMD "cmd"
#define FTO_RESULT "result"
#define FTO_INIT "init"

static object logger;

void create()
{
	logger = Import("LOG")->New("fight");
}

mapping newOutput()
{
	mapping o = ([]);

	o[FTO_FIGHT] = ([
		FTO_PLACE:([]),
		FTO_INIT:({}),
		FTO_CMD:({}),
		FTO_RESULT:([]),
	]);

	return o;
}

static void filter(mapping output)
{
	mixed *cmds = ({});
	foreach (mixed * cmd in output[FTO_FIGHT][FTO_INIT]) {
		if (cmd[0] == "user") {
			cmds += ({cmd});
		}
	}
	logger->Debug("dump cmds:%O", cmds);
}

string record(mapping output)
{
	string str = lpc2pystr(output);
	filter(output);
	return str;
}

void place(mapping output, int map, int x, int y)
{
	mapping cmd = (["map":map, "x":x, "y":y]);
	output[FTO_FIGHT][FTO_PLACE] = cmd;
	//logger->Debug("output place:%O\n", cmd);
}

void initUser(mapping output, mapping users)
{
	mixed *cmd = ({	
		"user",
		users,
	});

	output[FTO_FIGHT][FTO_INIT] += ({ cmd });
}

void initAddWarrior(mapping output, class Warrior w)
{
	mixed *cmd = ({	
		"addWarrior", 
		([
			"basic":(["type":w->type, "icon":w->icon,"position":w->position, "name":w->name, "skillId":w->skill->desc->id]), 
			"fightAttr":([FT_HP:HP(w), FT_MAX_HP:MAX_HP(w), FT_SP:SP(w), FT_MAX_SP:MAX_SP(w), FT_SPEED:SPEED(w), ]),
		]),
	});

	output[FTO_FIGHT][FTO_INIT] += ({ cmd });
	//logger->Debug("output initWarrior:%O\n", cmd);
}

void cmdBoutStart(mapping output, class Warrior w)
{
	/*
	mixed *cmd = ({	
		"bout", 
		([
			"warrior":w->position,
		]),
	});
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
	//logger->Debug("output bout:%O\n", cmd);
	*/
}

void cmdBoutEnd(mapping output)
{
	/*
	mixed *cmd = ({	
		"boutEnd",
	});
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
	//logger->Debug("output boutEnd:%O\n", cmd);
	*/
}


void cmdPerformStart(mapping output, class Warrior w, class Skill sk, int costSp, class Warrior t)
{
	mixed *cmd = ({	
		"perform",
		([
			//"skill":SKILL_MODULE->animationType(sk),
			"skill":sk->desc->id,
			"warrior":w->position,
		]),
	});
	if (t) {
		cmd[1]["mainTarget"] = t->position;
	}
	if (costSp > 0) {
		cmd[1]["costSp"] = costSp;
	}
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
	//logger->Debug("output perform:%O\n", cmd);
}

void cmdPerformEnd(mapping output)
{
	mixed *cmd = ({	
		"performEnd",
	});
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
	//logger->Debug("output performEnd:%O\n", cmd);
}

void cmdSkip(mapping output, class Warrior w)
{
	mixed *cmd = ({	
		"skip",
		([
			"warrior":w->position,
		]),
	});
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
}

void cmdMiss(mapping output, class Warrior w)
{
	mixed *cmd = ({	
		"miss",
		([
			"warrior":w->position,
		]),
	});
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
	//logger->Debug("output miss:%O\n", cmd);
}

void cmdAddStatus(mapping output, class Warrior w, class Status st)
{
	mixed *cmd = ({	
		"addStatus",
		([
			"warrior":w->position,
			"status":st->desc->type,
			"id":st->id,
		]),
	});
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
	//logger->Debug("output addStatus:%O\n", cmd);
}

void cmdDelStatus(mapping output, class Warrior w, class Status st)
{
	mixed *cmd = ({	
		"delStatus",
		([
			"warrior":w->position,
			"id":st->id,
		]),
	});
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
	//logger->Debug("output delStatus:%O\n", cmd);
}

void cmdMod(mapping output, class Warrior w, string attr, int delta, int flag)
{
	mixed *cmd = ({	
		"mod",
		([
			"warrior":w->position,
			"attr":attr,
			"delta":delta,
			"flag":flag,
		]),
	});
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
	//logger->Debug("output mod:%O\n", cmd);
}

void cmdDie(mapping output, class Warrior w, string attr)
{
	mixed *cmd = ({	
		"die",
		([
			"warrior":w->position,
		]),
	});
	output[FTO_FIGHT][FTO_CMD] += ({ cmd });
	//logger->Debug("output die:%O\n", cmd);
}

