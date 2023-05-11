==============================
Algorithms and Data Structures
==============================


Introduction
============

I got most of my material from https://www.chessprogramming.org. They will
do a much better job explaining any of the algorithms that I wrote than I can,
so this file will serve to highlight key points on their website copy their
detailed explanation.

Before continuing, read this: https://www.chessprogramming.org/Getting_Started

Side Note: I absoluetely love Sebastian Lague, and he made a video on Chess
programming. I think everyone should watch is because its just fantastic
https://www.youtube.com/watch?v=U4ogK0MIzqk.

PS: I added this after I finished the documentation, you REALLY need to watch
the Sebastian Lague video above. It is a really good introduction to evaluation
theory that will give you some foundational knowledge before you start reading
ChessProgramming.org

Board Representation
====================

https://www.chessprogramming.org/Board_Representation

The classical representation of a board is a 2d array of integers. This is
the most intuitive way to store pieces on the board, but doesn't really have
any advantages other than that.

The board representation that is used in this project is a piece-centric board
representation based around bitboards. In this representation, there is a
64-bit integer (called a bitboard) for every piece type, as well as a couple
of redundant union bitboards (e.g. occupancy, white, and black boards). The
representation itself is quite intuitive, a 4x4 example is below.::

        Board          WKing      BRook     BPawn     Occupancy
        . K . .        0 1 0 0    0 0 0 0   0 0 0 0   0 1 0 0 
        . . r .        0 0 0 0    0 0 1 0   0 0 0 0   0 0 1 0
        . . . .  --->  0 0 0 0    0 0 0 0   0 0 0 0   0 0 0 0
        . . p .        0 0 0 0    0 0 0 0   0 0 1 0   0 0 1 0

In my implementation, the top left square from white's perspective (or a8)
is the lowest order bit, and the bottom right is the highest order bit.

This in and of itself is not enough to store all of the information for the
chess board as there are some special rules which require us to add different
structures. The special cases are all listed below:

1. Castling: https://en.wikipedia.org/wiki/Castling
   The important thing to note with castling is that it can only be done with
   pieces that have never been moved before. So each color has queen and king
   side castling rights. If a player moves their queen-side rook, they loose
   their queen-side castling rights. This is stored in the lowest four bits
   of the 8-bit special moves.

2. Enpassant (In Passing): https://en.wikipedia.org/wiki/En_passant
   Data for an enpassant can be stored in only 4 bits (assuming that you know
   which color's turn it is). This implementation relies on the fact that when
   it is white's turn, the enpassant square will always be on row 6. Therefore
   3 bits can be used to specify the column of the enpassant, while 1 bit is
   reserved to specify if there is currently a valid enpassant. This is stored
   in bits 5 and 6-8 of special moves.

3. 50-Move Rule: https://en.wikipedia.org/wiki/Fifty-move_rule
   This comes up rather rarely, but it is a rule. If no player captures a piece
   and no player pushes a pawn in the last 50 full moves, the game ends in a
   draw. This is not in the code at the moment, but it will be stored in an
   integer called the halfmove clock.

4. Threefold Repetition: https://en.wikipedia.org/wiki/Threefold_repetition 
   A threefold repetition happens any time there a position comes up more than
   three times in the course of the game. This is very difficult to implement
   as it requires efficiently looking and comparing the current position with
   previous moves. I think the solution will have something to do with the
   transposition table, which is better left untouched until we are far into
   the development of our chess engine. I'll write up my thoughts in
   Evaluation/TranspositionTable.rst nonetheless.


Pseudo-Legal Moves
==================

When dealing with move generation, the first step is to generate the
"pseudo-legal" moves. These moves are not moves themsleves, but are potential
move candidates. They need to be adjusted to look for checks and pins.

Most of pseud-legal move generation is done by table lookup stored in
ChessBoard/MoveTables. Knights and kings are easy to handle, as their path
cannot be blocked by any piece. Sliding pieces get a little bit tricky, as they
can be blocked, and require a perfect-hash lookup algorithm called magic
bitboards.

The magic bitboard algoritm hashes a position by getting rid of all of the bits
that are not relavent to a particular move. For a rook on square a8, the
relavent bits are as follows.::

        . x x x x x x .
        x . . . . . . .
        x . . . . . . .
        x . . . . . . .
        x . . . . . . .
        x . . . . . . .
        x . . . . . . .
        . . . . . . . .

Notice that there are 12 relavent bits in this set. If a piece is in any of
these squares, it will inhibit the possible number of squares that a rook can
see.

To get a hash of this position, we ideally want to shift all of these bits into
the 12 lowest bits. The best way to do this is an algorithm that looks like
this: (remember that by my definition, the square a8 is the lowest order bit)::

        . x x x x x x .                            . . . . . . . .
        x . . . . . . .                            . . . . . . . .
        x . . . . . . .                            . . . . . . . .
        x . . . . . . .                            . . . . . . . .
        x . . . . . . .  * MAGIC_FOR_ROOK_ON_A8 =  . . . . . . . .
        x . . . . . . .                            . . . . . . . .
        x . . . . . . .                            . . . . x x x x
        . . . . . . . .                            x x x x x x x x

The purpose of the magic number is to shift all of the relavent bits to the
top of the number. They do not have to be in the same order as they were before
but they do have to be at the top.

After the multiplication we then apply a left shift of (64 - RELAVENT_BITS) to
get all of these bits to the bottom of our number. After this, a simple lookup
can get return the pseudo-move set that we are looking for.


Adjusting Moves
===============

After the pseudolegal moves have been generated, we then have to adjust the
move sets to account for checks and pins.

Accounting for checks is easy. All we have to do is generate a set of squares
that will take the king out of check. This may be a ray of squares extending
from a sliding piece to the king or a single square on top of a checking
knight that can be captured.

Accounting for pins is slightly harder, but still isn't that bad. In my
implementation, pins are updated whenever a move is made (this is because pins
are a valuable part of position evaluation: you're pretty happy if you have
pinned a valuable piece). Pins are stored in a 2x9 array of bitboards (the 2 is
for black and white pin-lists). Boards 1-8 are rays in each direction that
currently has a pinned piece. Board 9 is a union of the first 8 boards.

To use the array of pin-boards, one must first use the 9th board to check if
a piece is currently pinned with a simple AND operation. After doing this, you
search through the rest of the boards to find which pin ray you are currently
on. Once you find that ray, you know that the only legal squares you can move
to are on that ray. The only step left is a simple AND operation between the
pseudo-legal moves and the applicable ray.
