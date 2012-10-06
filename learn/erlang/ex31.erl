-module(ex31).
-export([
	 sum/1,
	 sum/2,
	 test/0
	]).

sum(N) when N >= 0 ->
    sum_acc(0,N,0).

sum(N,M) when N =< M ->
    sum_acc(N,M, 0).

sum_acc(N,N,M) ->
    M + N;
sum_acc(L,N,M) ->
    sum_acc(L,N-1,M+N).


test() ->
    sum(100).
