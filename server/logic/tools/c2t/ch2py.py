# -*- coding: utf-8 -*-

import os,sys
import string

dict = {}
inited = False

wordfile="picac_words.txt"
pinyinfile="picac_pinyin.txt"

def build_dict():
	global dict, inited
	input = file(pinyinfile)

	for line in input.read().splitlines():
		if len(line.strip()) == 0: continue
		if '-' in line : continue

		token = line.split(':')
		pr = token[0].strip().strip(string.digits)
		ch = token[1].strip()

		for i in range(0,len(ch),2):
			dict[ch[i:i+2]] = pr
	inited = True

def ch2pr(ch):
	global inited, dict

	if not inited: build_dict()

	i = 0
	pr = ''
	while True:
		if i >= len(ch) : break
		if ch[i].isalnum():
			pr+=str(ch[i])
			i+=1
			continue
		if dict.has_key(ch[i:i+2]):
			pr+=dict[ch[i:i+2]]+" "
			i+=1
		i+=1
	return pr

def word_2_pinyin():
	for x in file(wordfile).readlines():
			print x.strip(), ch2pr(x)

if __name__ == '__main__':
	#word_2_pinyin()
	#print ch2pr(raw_input("please input chinese:"))
	print ch2pr(sys.argv[1])
