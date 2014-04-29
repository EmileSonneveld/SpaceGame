SpaceGame
=========

A fast paced 2D game. With strategy elements.


Beginning of the game framework
Graphics: SFML
Physics: Box2D
Sound: FMOD
(Scripting: Lua)

Space makes the balls stick to the player
Shift: Accelerate
Arrow: Move
LMB: shoot


TODO:
-----
- Clean up the source code
	- Don't inerit from sf::Sprite, but make a register system like in MOJITO
	- More typedefs (COUNT and INDEX for example)
	- Make classes clearer
	- Make a binary compatible vector3 betwen Box2D and SFML
	- Place the dependend repos in the main repos
	- Port to Linux
- Better level
	- level editor (extend SVG usage?)
- Easy edit spaceship
	- Editor state
	- GUI (HTML5 http://www.awesomium.com/ ?)
V Shooting Kills
- a Masive spaceship as level
- Ai
	V Basic
	- Extending himself
	- Adaptable by number of thrusters
- Special pieces:
	- Turret
	- ExtraSpeed
	- Power generator
	- Shield
