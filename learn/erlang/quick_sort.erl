-module(quick_sort).

-export([
	 qsort/1
	 ]).


qsort([X]) ->
    [X];
qsort(List) ->
    [Head|Rest] = List,
    SmallList = [X || X <- Rest,
		 X <= Head],
    BigList = [ X || X <- Rest,
		     X > Head],
    [SmallList | Head | BigList].
