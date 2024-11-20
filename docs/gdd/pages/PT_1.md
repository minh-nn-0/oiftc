tags:: prototype

- # Goals
- TODO Make a few [[System]] for handling data in the level ([[Objects]] -> [[Tools]] and [[Messes]])
	- What needs to be known at start of the level?
		- [[Objects]]: What and where they are.
			- Make dedicated tile layer(s) for objects ? #idea
- TODO Basic [[Player Movement]]
- TODO [[Timer]]
-
- # Issues
- # Logs
	- [[Nov 19th, 2024]]
		- How does player stay on floor/platform ?
			- Physics based -> powerful but maybe overkill
			- Fixed Y position -> less versatile, easy to implement
			- Player doesn't need to jump -> doesn't really need physics
		- How messes interaction works
			- How do the game knows player are ready for an interaction ?
				- Based on position ?
			- How do the game knows what tools player's having in hand
				- Player's hot slot
			- -> The game will trigger interaction if
				- Player in position of a mess
				- Player hold **use item** button
			- -> Interaction are choosen based on
				- Tool in hand <- If player have 2 items in the hotslot, there have to be 2 buttons
				  id:: 673c18ea-4d89-4238-9a52-5b6b54fcc7db
				  How about using one hotslot only (even for sponge or brush) ? #idea
					- 1 hot slot can be easier to work with, and makes more sense
				-
	-