-module(ex32).
-export([
	 create/1,
	 reverse_create/1,
	 test/0
	 ]).

create(N) when N > 0 ->
    create_acc(N, []).

create_acc(0, L) ->
    L;
create_acc(N,L) ->
    create_acc(N-1, [N|L]).

reverse_create(N) when N>0 ->
    reverse_create_acc(1,N,[]).

reverse_create_acc(N,N, L) ->
    [N|L];
reverse_create_acc(N,M,L) when N < M ->
    reverse_create_acc(N+1, M, [N|L]).

test() ->
    {create(10), reverse_create(10)}.
