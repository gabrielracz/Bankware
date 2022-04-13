INSTRUCTIONS:
mkdir bin
cd bin
cmake ..
make
./bnk

CONTROLS:
- Character points towards mouse
- W to thrust forwards
- Left-Click to shoot
- Right-Click to dash

At GameOver / Win screen:
- R to respawn
- P to play arcade mode

WALKTHROUGH
Once the player has collected all 3 keys and destroyed the Gate of Destiny they win.
The player is protected with shields, which act like health. When you get hit, a shield is consumed.
Once you are out of shields and you get hit, it starts consuming your items to stay alive.
Once you are out of items, you lose.

The map is divided into 4 zones, in each of the corners, with progressing difficulty.
If at any point you are feeling week, go into the deep space in the middle and farm some collectibles.
The first zone is in the bottom right corner, you should go here first.
Collect the key and proceed clockwise to the next bottom left corner.
Collect the gun upgrades around the perimeter and slowly proceed to the corner to collect the key.

Go clockwise again to reach the final key stronghold. Collect the gun upgrades near the
bottom edge and slowly make your way to the far corner to collect the last key.

The gate is heavily guarded. It is located in the top right corner of the game world. Once you have
all 3 keys, shooting the gate will win you the game.

CHECKLIST
1.
Clear the zones, collect the keys, destroy the gate, win.

2.
There are three Enemies:
	1. Satellite
		- Stays still and can hit physically by player which causes them to bounce off eachother
		- Different sizes shoot different amounts of bullets in a star pattern at a slow rate
		- Have a large health pool and Iframes making them hard to remove
	2. Chaser
		- Patrols in circle waiting for player (parametric motion).
		- Once player in range chases them down
		- Low health and not a threat by itself but can be troublesome in large groups
		- Rotates to player and chases them down (using path prediction)
	3. Gunner
		- Patrols in circle as well as rotating around
		- Once player in range chases at a distance
		- If gets too close gunner dashes to the side (perpendicular to the vector between it and player) to keep distance
		- Shoots a shotgun spread that gives a recoil to the gunner shooting.
		- Has 2 health and is a difficult enemy
3.
We have a modular weapon system that allows for many different types of weapons and projectiles.
Players start with a simple pea-shooter that shoots slowly straight infront. As they progress,
they collect side cannons that aim towards the cursor and shoot much faster. The game was designed
around rogue-likes where you shoot all of your weapons at once with just the mouse button. The pear-shooter
uses a different texture than the side cannons.

4.
The particle system was used for the player's thrust aswell as the entity explosions. The thrust is a 
concentrated cone of fast moving particles while the explosion is a 360 release of many more slow moving particles.

5.
Collectible Items:
	There are various collectibles for the player. Player starts the game with a few shields and thats it the rest is picked up in the game world. 
	Items:
		- Dash (gives the player the dash ability, subsequent dash items picked up upgrade the dash ability) Colour: Orange
		- Keys (keys are picked up and needed to beat the game) 
		- Shield (is the first line of defence for the player takes priority above all other items when taking damage) Colour: Blue
	Buffs:
		- Buff (picks a random buff to give the player. Choices are speed up and rate of fire up) Colour: Green
		- PowerUp (boosts weapon power for a short time being our TIMED POWERUP) : Colour: Yellow
	Weapons:
		- Pea Shooter. Colour: Red
		- Left Canon. Looks like a cannon
		- Right Canon. Looks like a cannon
		- Dual Canon (gives player both canons or the missing one)
		When weapons are picked up again it buffs the weapon they already have that is the same type
		Players can choose what weapons they want by choosing wether or not to pick up a weapon (sometimes they can be risky to go for)
		
	Counters are kept for shields and total items

6.
The rendering of the game objects and their transformations is all done in View::RenderObject() using
transformation matricies. The Entity class defines an object that can move using physics based motion
in the world. Look at Entity::Update() for those maths. Basic enemies can move using both physics and
parametric based motion based on their state. GameObjects can have children which are transformed
relative to the parent space. This is how the weapons and items are attached to the player. View::RenderObject
passes the parent transformation matrix to all its children if it has them.

7.
The game performs sweep-and-prune circle-circle collisions against all entities in Game::SweepAndPruneEntityCollisions().
It also checks collisions against projectiles and entities using ray-circle collisions in Projectile::RayCircleCollision(). Collectibles only check
for collisions against the player.

8.
In OpenGL coordinates, the world is 200x200 units. The camera follows the player as they travel around the world.
The map is tiled with many different background sprites. In deep space, the star textures are chosen randomly to avoid
repetition. In the key strongholds, the background is tiled using a grid.

ON GAME OVER OR WIN PRESS "R" TO RESTART STORY MODE OR "P" TO ENTER ARCADE MODE
