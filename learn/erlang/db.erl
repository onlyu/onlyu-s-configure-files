-module(db).
-export([
	 new/0,
	 destroy/1,
	 write/3,
	 delete/2,
	 read/2,
	 match/2,
	 test/0
	 ]).

new() ->
    [].

destroy(_) ->
    ok.

write(Key,Element,Db) ->
    [{Key,Element} | Db].

delete(Key,Db) ->
    delete_acc(Key,Db,[]).

delete_acc(_, [], NewDb) ->
    NewDb;
delete_acc(Key, [{Key,_} | DbTail], NewDb) ->
    delete_acc(Key, DbTail, NewDb);
delete_acc(Key, [H | DbTail], NewDb) ->
    delete_acc(Key, DbTail, [H | NewDb]).

read(_, []) ->
    {error, instance};
read(Key, [{Key, Element} | _]) ->
    {ok, Element};
read(Key, [_|Db]) ->
    read(Key, Db).

match(Element, Db) ->
    match_acc(Element, Db, []).

match_acc(_, [], L) ->
    L;
match_acc(Element, [{Key, Element} | Db], L) ->
    match_acc(Element, Db, [Key | L]);
match_acc(Element, [_ | Db], L) ->
    match_acc(Element, Db, L).

test() ->
    ok.
