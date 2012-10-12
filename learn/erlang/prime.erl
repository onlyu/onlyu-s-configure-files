-module(prime).

-export([
	 prime/1,
	 prime2/1,
	 test/0,
	 test1/0
	 ]).

prime(2) -> [2];

prime(N) ->
    Set = [X || X <- lists:seq(2, N-1), N rem X =:= 0],
    case length(Set) of
    	0 ->
    	    [N | prime(N-1)];
    	_ ->
    	    prime(N-1)
    end.

prime2(N) ->
    prime2(N, 2, [2]).

prime2(2, _, List) ->
    List;

prime2(X, Y, List) ->
    NewList = case length([M || M <- List,
			     (Y + 1) rem M =:= 0]) of
	       0 ->
		   [Y + 1 | List];
	       _ ->
		   List
	   end,
    prime2(X-1, Y+1, NewList).


test() ->
    prime(100).

test1() ->
    prime2(100).

    
