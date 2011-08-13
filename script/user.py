#!/usr/bin/python 
#-*- coding: utf-8 -*-
from __future__ import with_statement 
import os
import sys
import json
import user_cache

cloud = {"open":0, "close":0, "no":0}
effect = {"open":0, "close":0, "no":0}
sound = {"open":0, "close":0, "no":0}

if __name__ == '__main__':
	data = user_cache.data["_lru_data"]
	count = 0
	for uid,user in data.iteritems():
		count += 1
		try:
			config = json.loads(user["cClientConfig"])
			system_config = config["system"]
			if system_config["close_cloud_effect"]:
				cloud["close"] += 1
			else:
				cloud["open"] += 1

			if system_config["close_scene_effect"]:
				effect["close"] += 1
			else:
				effect["open"] += 1

			if system_config["music"]:
				sound["open"] += 1
			else:
				sound["close"] += 1
		except:
			cloud["no"] += 1
			effect["no"] += 1
			sound["no"] += 1

	print "cloud", cloud
	print "effect", effect
	print "sound", sound
