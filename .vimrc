set number
map <space> <c-f>
map <backspace> <c-b>
syntax on
set number
set incsearch
set ruler
set laststatus=2
function Set_color_scheme_by_time()
	let current_hour = strftime("%H")
	echo "current_hour is".current_hour
	if current_hour < 6+0
	let color_scheme = "darkblue"
	elseif current_hour < 12+0
	let color_scheme = "morning"
	elseif current_hour < 18+0
	let color_scheme = "shine"
	else
	let color_scheme = "evening"
	endif

	echo "setting color scheme to".color_scheme
	execute "colorscheme ".color_scheme
	echo colors_name
endfunction

set ts=4
set sw=4
set foldcolumn=3
set foldmethod=syntax

