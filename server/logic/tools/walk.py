
import os
import sys

def walkfile(top, func):
	for root, dirs, files in os.walk(top):
		sys.path.append(root)
		for leaf in files:
			if leaf[-2:] == "py":
				leaf = leaf.replace(".py", "")
				mod = __import__(leaf)
				func(mod)
				del sys.modules[leaf]

		del sys.path[-1]
				
	
def name(mod):
	name = mod.data["prop"]["cName"];
	print name, len(name)

def vyuanbao(mod):
	name = mod.__name__
	vyb = mod.data["prop"].get("ivYuanBao", 0)
	print name,vyb
	
	
if __name__ == "__main__":
	top = "../user"
	walkfile(top, vyuanbao)

