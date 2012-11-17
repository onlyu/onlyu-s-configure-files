-module(fact).
-export([
	 fact/1,
	 test/1
	]).

fact(0) -> 1;

fact(X) when X > 0 -> X * fact(X-1);

fact(_) -> io:format("argument must >= 0!!!").

test(0) -> 1;
test(X) ->
    1/6 * lists:foldl(fun(Z, A) -> Z + A end, 0, [ test(Y) || Y <- lists:seq(0,X-1)]).
%    1/6 * lists:foldl(fun(X, Acc) -> X + Acc, 0, [test(Y) || Y <- lists:seq(0, X-1)]).
