==============
Code Structure
==============


Scenes
======

To keep code clean and leave room for expandability, this project is built
around scenes. Different states of the program are encapsulated in different
scenes (e.g. Game and StartMenu).

Scenes (along with many things in this project) are all made to be polymorphic.
Game and StartMenu are both descendants of the parent class Scene.
Children must override the four virtual functions declared in scenes/Scene.h. 
These functions include init(), update(), create_new(), and the ~Scene().

When the program begins, a start menu scene is created and initialized.
The update function for that scene is then called repeatedly. The scene
returns a state which notifies the main function of any actions that it has
to take in between updates.

Finally, each scene defines a function which creates the next scene in the
sequence. When a scene decides that it is no longer needed (e.g. when the user
clicks Start in the start menu), it returns the state M_RECREATE_ME. The main
function creates a new scene via the scene's create_new() function, then
deletes the old scene.


Start Menu
==========

I'll keep this one short, there isn't that much to talk about here. Using
a couple of ncurses library calls I allow the user to select from a couple
of choices for what the game will be (e.g. game length and player types).

Start Menu uses these choices to construct a Game.


Game
====

Currently not implemented.

The current Game scene simply acts as a bridge between the move generation
and the UI. It will probably be heavily changed (or completely replaced).

That being said, here are some of my thoughts:

A game should have one BoardRepresentation and two Players. Players can be
polymorphic in the same way that Scenes are (decendents of the class Player
defined in scenes/Game/Player/Player.h.


BoardRep and Move Generation
============================

The current implementation of the BoardRep and Move Generation works, but I
think the code should be cleaned up a little bit more. BoardRep is implemented
as defined in https://www.chessprogramming.org/Board_Representation.

..
        ------------Ignore this, I wrote it but now think it's wrong-------------------
        | The implementation (as it is now) has two main problems.
        |  
        | BoardRep has many member functions that would be better used outside of
        | the class. As a general note, the class should be used to abstract away
        | the implementation of the data structure. Functions that translate it to
        | a character array or initialize it to a certain FEN string
        | (https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation) should be
        | left in the class. However, I think that functions that read the board
	| should be written outside the class and have the class as an input (leaning
	| a little bit more towards C-Style). And yes, I'm sorry Tim Wren, I think
	| the best thing to use here is friend functions. I know you hate them.
	| Yes, I understand that this is totally nit-picky and over the top, but I'll
	| probably do it anyway because I already plan to clean up the code.
        |	
	| The implementation of the move tables is not intuitive and needs to be
	| replaced. As it is now, MoveTables exposes several functions which allow
	| users to read the rmagic, bmagic, knight, and king tables. The fact that
	| these tables are used in some cases but not others (things like pawn moves
	| are completely separate).
	-------------------------------------------------------------------------------
