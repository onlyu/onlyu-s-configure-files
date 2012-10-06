-module(qsort).

-export([
	 qsort/1,
	 test/0
	 ]).

qsort([]) -> [];
qsort([Head|Rest]) ->
    qsort([X || X <- Rest, X =< Head]) ++ [ Head] ++ qsort([ X || X <- Rest, X > Head]).

test() ->
    qsort([45,12,76,45,34,76,875,23,567,342,453,657,345]).
