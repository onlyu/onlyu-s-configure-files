"============================================================================
"        File: Find.vim
" Description: smart switch from buffers and files
"      Author: only@175game.com 
"     Version: 0.1
"
"       Usage: This is the vim copy of emacs iswitch, please see emacs iswitch
"			   or ido-mode
"     History:
"
"============================================================================


" Install command and default key mapping
if !exists(":Buffer")
  	command Buffer :call <SID>buffer()
endif

if !hasmapto("<Plug>Buffer")
    nmap <C-X>b :Buffer<CR>
endif

if !exists(":File")
	command File :call <SID>file()
endif

if !hasmapto("<Plug>File")
	nmap <C-X>f :File<CR>
endif

" load vim.py
if !exists('g:loaded_python_lib') 
	pyfile ~/.vim/plugin/vim.py
	let g:loaded_flymake = 1
endif

python << EOF
import os
import difflib

def build_buffer_list():
	buffer_list = buffers()
	current = current_buffer_id()
	last = last_buffer_id()
	if current>0:
		buffer_list.remove(current)
	if last>0:
		buffer_list.remove(last)
		buffer_list.insert(0, last)
	return buffer_list

def build_buffer_string(buffer_list):
	str = ""
	for buf in buffer_list:
		str = "|" + buffer_base_name(buf) + str
	str = str.lstrip('|')
	if len(str)>40:
		str = "..." + str[len(str) - 40:]
	return str

def shift_buffer_list(buf_list):
	if len(buf_list) < 2:
		return buf_list[:]
	ret = buf_list[1:]
	ret.append(buf_list[0])
	return ret

def get_sort_func(search):
	def sort_func(buf):
		name = buffer_base_name(buf)
		matcher = difflib.SequenceMatcher(None, name, search)
		#print(name, search, matcher.ratio())
		return 1-matcher.ratio()
	return sort_func
		
def buffer_base_name(buf):
	name = bufname(buf)
	return os.path.basename(name)
		
def sort_buffer_list(buf_list, search):
	if len(buf_list) < 2:
		return buf_list[:]
	return sorted(buf_list, key=get_sort_func(search))
EOF

" silent buffer [buffer-number]
"

function! <SID>buffer()
python << EOF
search_name = ""
current = current_buffer_id()
last = last_buffer_id()

buf_list = build_buffer_list()
buf_str = build_buffer_string(buf_list)

sorted_list = buf_list
echon("find:("+buf_str+")")
while True:
	char = getchar()
	if char == 13:
		if len(sorted_list) > 0:
			buf = sorted_list[0]
			switch_to_buffer(buf)
		break
	if char == 27: #esc
		echon("\r")		
		break
	if char == 8: # backspace  
		search_name = search_name[:-1]
		sorted_list = sort_buffer_list(sorted_list, search_name)
		pass
	elif char == 9: # tab
		#print sorted_list
		sorted_list = shift_buffer_list(sorted_list)
		#print sorted_list
		pass
	else:
		search_name += chr(char)
		sorted_list = sort_buffer_list(sorted_list, search_name)
	buf_str = build_buffer_string(sorted_list)
	echon("\r                      ")
	echon("\rbuffer:(" + buf_str + ")" + search_name)
EOF
endfunction

function! <SID>file()
python << EOF
import os
def join_file_name(files):
	ret = "|".join(files)
	if len(ret)>40:
		return "..." + ret[ len(ret) - 40:]
	return ret

def get_ratio_func(search):
	def ratio_func(file_name):
		matcher = difflib.SequenceMatcher(None, file_name, search)
		return matcher.ratio()
	return ratio_func

def sort_file_name(search):
	return sorted(os.listdir(os.path.dirname(search)), key=get_ratio_func(os.path.basename(search)))


current_file = bufname(current_buffer_id())
current_dir = ""
search_list = []
search_string = ""
if os.path.isdir(current_file):
	current_dir = current_file
else:
	current_dir = os.path.dirname(current_file)

search_string = current_dir + "/"
search_list = os.listdir(current_dir) 

echon("file:(" + join_file_name(search_list) + ")" + search_string)

while True:
	char = getchar()
	if char == 13:
		if len(search_list) != 0:
			file_name = search_list.pop()
			file_path = os.path.dirname(search_string) + "/" + file_name
			if os.path.exists(file_path):
				vim.command("e "+file_path)
		break
	if char == 27: #esc
		echon("\r                                                                 ")
		break
	if char == 8: # backspace  
		search_string = search_string[:-1]
	elif char == 23: # ctrl+w
		search_string = search_string.rstrip("/")
		search_string = os.path.dirname(search_string) + "/"
	elif char == 9: # tab
		if len(search_list) != 0:
			file_name = search_list.pop()
			file_path = os.path.dirname(search_string) + "/" + file_name
			if os.path.isdir(file_path):
				file_path += "/"
			search_string = file_path 
	elif chr(char) == '/':
		search_string = '/'
	elif chr(char) == '~':
		search_string = '~'
	else:
		search_string += chr(char)

	search_list = sort_file_name(search_string)
	echon("\r                      ")
	echon("\rfile:(" + join_file_name(search_list) + ")" + search_string)

EOF
endfunction
