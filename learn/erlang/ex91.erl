-module(ex91).
-export([test/0]).
-vsn(1.0).

seq_print(N) when N > 0 ->
    lists:foreach(fun(Number) -> io:format("~p ", [Number]) end, lists:seq(1, N)).

filter_se(L, N) ->
    lists:filter(fun(Num) -> Num =< N end, L).

even(N) when N > 0 ->
    lists:filter(fun(Num) ->
			 Num rem 2 == 0
		 end,lists:seq(1,N)).

concat(L) ->
    lists:foldl(fun(Element, Acc) ->
			Acc ++ Element
	       end, [], L).

sum(L) ->
    lists:foldl(fun(Num, Acc) -> Num + Acc end, 0, L).		       

test() ->
    seq_print(100),
    io:format("~n"),
    {filter_se([12,35,23,654,76,3235,456,574,23,456,1321,93], 100),
    even(100),
    concat([ [1, 2,3 ], [4,5,6], [7,8,9] ]),
    sum(lists:seq(1, 100))}.

