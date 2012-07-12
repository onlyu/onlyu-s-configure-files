#compress:no
#cksum:

data = {
	"header":{
		"version":"1.0",
	},

	"meta": {
	},

	"tag":{
		"fight":{
			"place":{"map":1001, "x":100, "y":100},
			"init":[
				["addWarrior", {"basic":{"type":"user", "icon":10001,"position":1,}, "fightAttr":{"hp":100, "speed":100, },}],
				["addWarrior", {"basic":{"type":"user", "icon":10001,"position":10,}, "fightAttr":{"hp":100, "speed":100, },}],
			],
			"cmd":[
				["bout", {"warrior":1}, [
					["perform", {"skill":"phy", "warrior":1, "mainTarget":10}, [
						["miss", {"warrior":10}],
						["addStatus", {"warrior":10, "status":"jiaxue", "bout":3}],
						["delStatus", {"warrior":10, "status":"jiaxue"}],
						["mod", {"warrior":10, "attr":"hp", "value":1, "flag":["baoji"],}],
						["die", {"warrior":10},],
					],]],
				]
			],
			"result":{
			},
		},
	},
}

