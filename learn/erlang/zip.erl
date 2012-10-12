-module(zip).
-export([test/0]).
-vsn(1.0).

zip(L, R) ->
    lists:reverse(zip_acc(L, R, [])).

zip_acc([], _, Result) ->
    Result;
zip_acc(_, [], Result) ->
    Result;
zip_acc([HL | TL], [HR | TR], Result) ->
    zip_acc(TL, TR, [{HL, HR} | Result]).

zip_with(F, L, R) ->
    lists:map(fun({E1, E2}) -> F(E1, E2) end, zip(L, R)).

test() ->
    {zip ([1,2], [3,4,5]),
    zip_with(fun(X, Y) -> X + Y end, [1, 2], [3, 4, 5])}.
