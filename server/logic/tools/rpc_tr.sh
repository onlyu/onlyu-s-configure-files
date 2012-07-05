#!/bin/sh

lua ./tools/rpc_tr.lua `find . -name "*decl.c"`

python ./tools/rpc2json.py
