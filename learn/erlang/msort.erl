-module(msort).
-export([
	 msort/1
	 ]).

msort([]) ->
    [];
msort([X]) ->
    [X];
msort(L) ->
    {H,T} = lists:split( length(L) div 2, L),
    merge(msort(H), msort(T)).

merge([],L) ->
    L;
merge(L, []) ->
    L;
merge([H1|T1], [H2|T2]) ->
    case H1 > H2 of
	true ->
	    [H2 | merge([H1|T1], T2)];
	false ->
	    [H1 | merge( T1, [H2 | T2])]
    end.
