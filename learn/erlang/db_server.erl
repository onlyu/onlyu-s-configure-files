-module(db_server).
-export([start/0, stop/0, upgrade/1]).
-export([write/2, read/1, delete/1]).
-export([init/0, loop/1]).
-vsn(1.0).

start() ->
    register(db_server, spawn(db_server, init, [])).

stop() ->
    db_server ! stop.

upgrade(Data) ->
    db_server ! {upgrade, Data}.

write(Key, Data) ->
    db_server ! {write, Key, Data}.

read(Key) ->
    db_server ! {read, self(), Key},
    receive Replay ->
	    Replay
    end.

delete(Key) ->
    db_server ! {delete, Key}.

init() ->
    loop(db2:new()).

loop(Db) ->
    receive
	{write, Key, Data} ->
	    loop(db2:write(Key, Data, Db));
	{read, Pid, Key} ->
	    Pid ! db2:read(Key, Db),
	    loop(Db);
	{delete, Key} ->
	    loop(db2:delete(Key, Db));
	{upgrade, Data} ->
	    NewDb = db2:convert(Data, Db),
	    db_server:loop(NewDb);
	stop ->
	    db2:destroy(Db)
    end.
