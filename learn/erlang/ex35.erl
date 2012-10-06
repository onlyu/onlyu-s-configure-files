-module(ex35).
-export([
	 filter/2,
	 reverse/1,
	 test/0
	 ]).

filter(L, N) when N >= 0 ->
    filter_acc(L, N, []).

filter_acc([], _, M) ->
    reverse(M);
filter_acc(_, 0, M) ->
    reverse(M);
filter_acc([H|T], N, M) ->
    filter_acc(T, N-1, [H|M]).


reverse(L) ->
    reverse_acc(L, []).

reverse_acc([], L1) ->
    L1;
reverse_acc([H|T], L) ->
    reverse_acc(T, [H|L]).

concatenate(L) ->
    concatenate_acc(L, []).

concatenate_acc([], L) ->
    reverse(L);
concatenate_acc([H|T], L) ->
    concatenate_acc(T, merge(H, L)).

merge([], R) ->
    R;
merge([H|T],R) ->
    merge(T, [H|R]).

flatten(L) ->
    reverse(flatten_acc(L, [])).

flatten_acc([], R)->
    R;
flatten_acc([H|T], R) ->
    flatten_acc(T, flatten_acc(H, R));
flatten_acc(N, R) ->
    [N|R].

test()->
    {filter([1,2,3,4,5,6], 3),
    reverse([1,2,3]),
    concatenate([[1,2,3], [], [4, five]]),
    flatten([[1,[2,[3],[]]], [[[4]]], [5, 6]])

}.

