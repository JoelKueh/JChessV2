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
1. 4 character string representing the game ID.
2. 
Only parameter is a 4 character hex string representing the game ID.
The server responds with success if the game ID is not currently in use.

update
------
No parameters, the server sends a fen-string re


Server Response Format 
======================

When responding to a request, the server will send the name of the request
followed by the data that should be returned.

Every response is succeeded by the time remaining in milliseconds for white
followed by the time in milliseconds remaining for black.

The last part of the response is the status of the request, currently this
can only be success or invalid, but there may be more added later.
