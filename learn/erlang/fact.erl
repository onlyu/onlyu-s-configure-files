-module(fact).
-export([
	 fact/1
	]).

fact(0) -> 1;

fact(X) when X > 0 -> X * fact(X-1);

fact(_) -> io:format("argument must >= 0!!!").
