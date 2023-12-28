JChess Server Protocol
======================

The JChess Server protocol is a pipe/newline delimited text stream. The client
sends a request followed a series of parameters. In return, the server sends
a response followed by the data that was requested.

When one of the players makes a move, the server will forward that move to
the other client. The server will then request the client for a move in
response.

Client Request Types
====================

move
----
Four/Five character, algebraic notation for a move.
Should be sent in response to a "go" request.

keepalive
---------
This is not implemented in favor of the built-in unix keepalive
Has no parameters, server will send "alive" as a response

join
----
Only parameter is a 4 character hex string representing the game ID.
The server responds with the color of the player, a fen string for the root
position and a list of moves in full algebraic notation that have been played.

create_room
-----------
Takes 4 parameters:
create_room|<GAME_ID>|<COLOR>|<TIME>|<INCR>

update
------
No parameters, the server sends responds with the state of the game.

resign
------
No parameters, forfeits the game.

exit
----
No parameters, no response. The client forfeits and leaves the game.


Server Response Format 
======================

When responding to a request, the server will send the name of the request
followed by the data that should be returned.

Every response is succeeded by the time remaining in milliseconds for white
followed by the time in milliseconds remaining for black.

The last part of the response is the status of the request, currently this
can only be success or invalid, but there may be more added later.

move
----

Server forwards the move as well as information about the state of the game
to both clients. The server will then send a go command to the proper client.
The time is recorded in milliseconds.
Move-Response (To BOTH):
    moved|<move>|<color>|<chk/stmt/chkmt/nrml>|time[0]|time[1]
Go-Response (To Next Player): go

create_room
-----------

Server sends an initial fen message that holds the root of the game.
The server then sends batches of no more than 10 moves at a time.

Fen-Format: state|<color_to_move>|<chk/stmt/chkmt/nrml>|time[0]|time[1]|<FEN_STRING>
Moves-Format: <MOVE_1>
            <MOVE_2>
            <MOVE_3>
            ...
            <MOVE_N>
Termination-Format: end_fen

join
----

Server responds the same way as with create room.

resign
------

Resignation Format: resign|<COLOR>

update
------

Server responds the same way as with join and create room.

exit
----

No response, just handles cleanup. Client can always saftely exit, this may
not be implemented in favor of default socket exit.
