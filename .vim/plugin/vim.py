import vim

def bufnr(name):
	return vim.eval('bufnr("'+name+'")')

def expand(exp):
	return vim.eval('expand("'+exp+'")')

def bufexists(bufid):
	return int(vim.eval('bufexists('+str(bufid)+')'))

def bufname(bufid):
	return vim.eval('bufname('+str(bufid)+')')

def bufloaded(bufid):
	return vim.eval('bufloaded('+str(bufid)+')')

def buflisted(bufid):
	return vim.eval('buflisted('+str(bufid)+')')

def current_buffer_id():
	return int(vim.eval('bufnr("%")'))

def last_buffer_id():
	return int(vim.eval('bufnr("#")'))

def max_buffer_id():
	return int(vim.eval('bufnr("$")'))

def buffers():
	bufs = []
	for i in range(1, max_buffer_id()+1):
		if bufexists(i):
			bufs.append(i)
	return bufs

def echon(string):
	vim.command('echon "' + string + '"')	

def getchar():
	ret = vim.eval("getchar()")
	try:
		return int(ret)
	except:
		return 8 

def switch_to_buffer(bufid):
	vim.command('silent	buffer '+str(bufid))

