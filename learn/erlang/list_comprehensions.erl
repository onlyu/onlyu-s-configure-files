-module(list_comprehensions).
-export([test/0]).
-vsn(1.0).

test1(N) ->
    [ X || X <- lists:seq(1, N),
	   X rem 3 == 0].

test2(L) ->
    [ X || X <- L,
	   is_integer(X)].

intersection(L, R) ->
    [ X || X <- L,
	   lists:member(X, R) ].

difference(L, R) ->
    [ X || X <- (L ++ R),
	   not (lists:member(X, L) and lists:member(X, R))].

test() ->
    {test1(100),
     test2([1, hello, 100, boo, "boo", 9]),
     intersection([1,2,3,4,5],
		  [4,5,6,7,8]),
     difference([1,2,3,4,5],
		[4,5,6,7,8])}.
