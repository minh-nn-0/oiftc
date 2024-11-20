tags:: prototype

- # Goals
- TODO Basic [[Player Movement]]
	-
- TODO Basic [[Interactions]]
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
				  How about using one hotslot only (even for sponge or brush) ? #idea
				-
	-