-module(qsort).

-export([
	 qsort/1
	 ]).

qsort([]) ->
    [];
qsort([Head|Rest]) ->
    qsort([X || X <- Rest, X =< Head]) ++ [ Head] ++ qsort([ X || X <- Rest, X > Head]).
