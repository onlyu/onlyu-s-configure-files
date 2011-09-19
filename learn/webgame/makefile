

all:
	cd rsa && make
	cd z && make
	cd fs && make
	cp ~/client/trunk/script/game/net/rpc.cfg .
	python parse_rpc.py -f rpc.cfg -o rpc.js
