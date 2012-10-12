-module(ex33).
-export([
	 print/1,
	 test/0
	 ]).

print(N) when N > 0 ->
    print_acc(1, N).

print_acc(N,N) ->
    io:format("Number:~p~n", [N]);
print_acc(M,N) ->
    io:format("Number:~p~n", [M]),
    print_acc(M+1,N).

test() ->
    print(100).
