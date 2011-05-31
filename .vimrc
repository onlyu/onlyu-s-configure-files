" An example for a vimrc file.
"
" Maintainer: a18ccms <a18ccms@gmail.com>
" Last change: 2010 03 12
"
" To use it, copy it to
" for Unix and OS/2: ~/.vimrc
" for Amiga: s:.vimrc
" for MS-DOS and Win32: $VIM\_vimrc
" for OpenVMS: sys$login:.vimrc

" When started as "evim", evim.vim will already have done these settings.
if v:progname =~? "evim"
finish
endif

set history=50   " keep 50 lines of command line history
set showcmd   " display incomplete commands

" For Win32 GUI: remove 't' flag from 'guioptions': no tearoff menu entries
" let &guioptions = substitute(&guioptions, "t", "", "g")

" Don't use Ex mode, use Q for formatting
map Q gq


" Convenient command to see the difference between the current buffer and the
" file it was loaded from, thus the changes you made.
command DiffOrig vert new | set bt=nofile | r # | 0d_ | diffthis
\ | wincmd p | diffthis
\ | wincmd p | diffthis


set guioptions-=T


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" һ���趨
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" �趨Ĭ�Ͻ���
set fileencodings=gb2312,chinese,latin-1
if has("win32")
set fileencoding=chinese
else
set fileencoding=gb2312
endif
"language message zh_CN.utf-8
"����˵�����
source $VIMRUNTIME/delmenu.vim
source $VIMRUNTIME/menu.vim
"set font
"set guifont=Nsimsun

"���ô��ڴ�С
"set lines=35
"set columns=120

" ��Ҫʹ��vi�ļ���ģʽ������vim�Լ���
"set nocompatible

" �ڴ���δ�����ֻ���ļ���ʱ�򣬵���ȷ��
set confirm

" ��windows���������
set clipboard+=unnamed

" ����ļ�����
filetype on

" �����ļ����Ͳ��
filetype plugin on

" Ϊ�ض��ļ�����������������ļ�
filetype indent on

" �������·��ŵĵ��ʲ�Ҫ�����зָ�
set iskeyword+=_,$,@,%,#,-

" �﷨����
syntax on

" �����ַ������䲻��100������
:highlight OverLength ctermbg=red ctermfg=white guibg=red guifg=white
:match OverLength '\%101v.*'

" ״̬����ɫ
highlight StatusLine guifg=SlateBlue guibg=Yellow
highlight StatusLineNC guifg=Gray guibg=White

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" �ļ�����
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" ��Ҫ�����ļ��������Լ���Ҫȡ�ᣩ
set nobackup

" ��Ҫ����swap�ļ�����buffer��������ʱ��������
setlocal noswapfile
set bufhidden=hide

" �ַ���������������Ŀ
set linespace=0

" ��ǿģʽ�е��������Զ���ɲ���
set wildmenu

" ��״̬������ʾ�������λ�õ��кź��к�
set ruler
set rulerformat=%20(%2*%<%f%=\ %m%r\ %3l\ %c\ %p%%%)

" �����У���״̬���£��ĸ߶ȣ�Ĭ��Ϊ1��������2
set cmdheight=2

" ʹ�ظ����backspace����������indent, eol, start��
set backspace=2

" ����backspace�͹�����Խ�б߽�
set whichwrap+=<,>,h,l

" ������buffer���κεط�ʹ����꣨����office���ڹ�����˫����궨λ��
set mouse=a
set selection=exclusive
set selectmode=mouse,key

" ������ʱ����ʾ�Ǹ�Ԯ���������ͯ����ʾ
set shortmess=atI

" ͨ��ʹ��: commands������������ļ�����һ�б��ı��
set report=0

" ����vim��������ĵε���
set noerrorbells

" �ڱ��ָ�Ĵ��ڼ���ʾ�հף������Ķ�
set fillchars=vert:\ ,stl:\ ,stlnc:\

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" ������ƥ��
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" ������ʾƥ�������
set showmatch

" ƥ�����Ÿ�����ʱ�䣨��λ��ʮ��֮һ�룩
set matchtime=5

" ��������ʱ����Դ�Сд
" set ignorecase

" �����������ľ��ӣ�phrases��
set hlsearch

" ������ʱ������Ĵʾ�����ַ�����������firefox��������
set incsearch

" ����:set list������Ӧ����ʾЩɶ��
set listchars=tab:\|\ ,trail:.,extends:>,precedes:<,eol:$

" ����ƶ���buffer�Ķ����͵ײ�ʱ����3�о���
set scrolloff=3

" ��Ҫ��˸
set novisualbell

" �ҵ�״̬����ʾ�����ݣ������ļ����ͺͽ��룩
set statusline=%F%m%r%h%w\ [FORMAT=%{&ff}]\ [TYPE=%Y]\ [POS=%l,%v][%p%%]\ %{strftime(\"%d/%m/%y\ -\ %H:%M\")}

" ������ʾ״̬��
set laststatus=2

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" �ı���ʽ���Ű�
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" �Զ���ʽ��
set formatoptions=tcrqn

" �̳�ǰһ�е�������ʽ���ر������ڶ���ע��
set autoindent

" ΪC�����ṩ�Զ�����
set smartindent

" ʹ��C��ʽ������
set cindent

" �Ʊ��Ϊ4
set tabstop=4

" ͳһ����Ϊ4
set softtabstop=4
set shiftwidth=4

" ��Ҫ�ÿո�����Ʊ��
set noexpandtab

" ��Ҫ����
set nowrap

" ���кͶο�ʼ��ʹ���Ʊ��
set smarttab

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Autocommands
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" ֻ�������ļ����ͱ���⵽��ʱ����ʾ�кţ���ͨ�ı��ļ�����ʾ

if has("autocmd")
   autocmd FileType xml,html,c,cs,java,perl,shell,bash,cpp,python,vim,php,ruby set number
   autocmd FileType xml,html vmap <C-o> <ESC>'<i<!--<ESC>o<ESC>'>o-->
   autocmd FileType java,c,cpp,cs vmap <C-o> <ESC>'<o
   autocmd FileType html,text,php,vim,c,java,xml,bash,shell,perl,python setlocal textwidth=100
   autocmd Filetype html,xml,xsl source $VIMRUNTIME/plugin/closetag.vim
   autocmd BufReadPost *
      \ if line("'\"") > 0 && line("'\"") <= line("$") |
      \   exe "normal g`\"" |
      \ endif
endif " has("autocmd")

"����Java������Զ���ȫ
au FileType java setlocal omnifunc=javacomplete#Complete

let mapleader = "."
"���Զ���ȫ�Ŀ�ݼ�<C-X><C-O>��<leader>;
imap <leader>; <C-X><C-O>

"�趨����Taglist����Ŀ�ݼ�ΪF4��������VIM��������г������б��
map <F4> :Tlist<CR>

"���ó���ı������к͵��ԵĿ�ݼ�F5��F6��Ctrl-F5
map <F5> :call CompileRun()<CR>
map <C-F5> :call Debug()<CR>
map <F6> :call Run()<CR>

"���ô����ʽ����ݼ�F3
map <F3> :call FormartSrc()<CR>

"����tab�����Ŀ�ݼ�����:tabnew��<leader>t����:tabn, :tabp��<leader>n,
"<leader>p
map <leader>t :tabnew<CR>
map <leader>n :tabn<CR>
map <leader>p :tabp<CR>

"��cscope֧��
"set csprg=d:\bin\cscope


"ʹ��<leader>e�򿪵�ǰ�ļ�ͬĿ¼�е��ļ�
if has("unix")
map ,e :e <C-R>=expand("%:p:h") . "/" <CR>
else
map ,e :e <C-R>=expand("%:p:h") . "\" <CR>
endif

"����CompileRun�������������ý��б��������
func CompileRun()
exec "w"
"C����
if &filetype == 'c'
exec "!del %<.exe"
exec "!gcc % -o %<.exe"
exec "!%<.exe"
elseif &filetype == 'cpp'
exec "!del %<.exe"
exec "!g++ % -o %<.exe"
exec "!%<.exe"
"Java����
elseif &filetype == 'java'
exec "!del %<.class"
exec "!javac %"
exec "!java %<"
endif
endfunc
"��������CompileRun

"����Run�������������ý��б��������
func Run()
exec "w"
"C����
if &filetype == 'c'
exec "!%<.exe"
elseif &filetype == 'cpp'
exec "!%<.exe"
"Java����
elseif &filetype == 'java'
exec "!java %<"
endif
endfunc
"��������Run


"����Debug�������������Գ���
func Debug()
exec "w"
"C����
if &filetype == 'c'
exec "!del %<.exe"
exec "!gcc % -g -o %<.exe"
exec "!gdb %<.exe"
elseif &filetype == 'cpp'
exec "!del %<.exe"
exec "!g++ % -g -o %<.exe"
exec "!gdb %<.exe"
"Java����
exec "!del %<.class"
elseif &filetype == 'java'
exec "!javac %"
exec "!jdb %<"
endif
endfunc
"��������Debug
"����FormartSrc()
func FormartSrc()
exec "w"
"C����,Perl����,Python����
if &filetype == 'c'
exec "!astyle --style=gnu --suffix=none %"
exec "e! %"
elseif &filetype == 'cpp'
exec "!astyle --style=gnu --suffix=none %"
exec "e! %"
elseif &filetype == 'perl'
exec "!astyle --style=gnu --suffix=none %"
exec "e! %"
elseif &filetype == 'py'
exec "!astyle --style=gnu --suffix=none %"
exec "e! %"
"Java����
elseif &filetype == 'java'
exec "!astyle --style=java --suffix=none %"
exec "e! %"
elseif &filetype == 'jsp'
exec "!astyle --style=gnu --suffix=none %"
exec "e! %"
elseif &filetype == 'xml'
exec "!astyle --style=gnu --suffix=none %"
exec "e! %"
elseif &filetype == 'html'
exec "!astyle --style=gnu --suffix=none %"
exec "e! %"

elseif &filetype == 'htm'
exec "!astyle --style=gnu --suffix=none %"
exec "e! %"

endif
endfunc
"��������FormartSrc

" �ܹ�Ư������ʾ.NFO�ļ�
set encoding=utf-8
function! SetFileEncodings(encodings)
    let b:myfileencodingsbak=&fileencodings
    let &fileencodings=a:encodings
endfunction
function! RestoreFileEncodings()
    let &fileencodings=b:myfileencodingsbak
    unlet b:myfileencodingsbak
endfunction

au BufReadPre *.nfo call SetFileEncodings('cp437')|set ambiwidth=single
au BufReadPost *.nfo call RestoreFileEncodings()

" ������ʾ��ͨtxt�ļ�����Ҫtxt.vim�ű���
au BufRead,BufNewFile * setfiletype txt

" �ÿո���������۵�
set foldenable
set foldmethod=manual
nnoremap <space> @=((foldclosed(line('.')) < 0) ? 'zc' : 'zo')<CR>

colo darkblue

" FOR Tlist
let Tlist_Ctags_Cmd = "/usr/local/bin/exctags" " FOR BSD
"let Tlist_Auto_Open=1
let Tlist_Auto_Update=1
let Tlist_Enable_Fold_Column=1
let Tlist_Sort_Type = "name"
let Tlist_WinWidth = 30
let Tlist_Show_One_File = 0
let Tlist_Exit_OnlyWindow = 1
let Tlist_Use_SingleClick = 1

"set termencoding=utf-8
"set fileencoding=utf-8
set fileencodings=gb2312,gbk,gb18030
set fileformats=unix
set encoding=prc 
