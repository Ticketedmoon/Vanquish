### Vanquish (C++ RPG)

A 2D RPG where you begin as a prisoner and find your way in the world.
- Complete quests,
- Slay monsters,
- Level up
- Purge tyranny
- (TBD)

---
#### Dev Log

##### 2023-07-18

**'Setting up the world'**
- Simple top down view.  
- Playable character with animations moving in the cardinal directions.  
- Camera moves upon movement  
- Player can chop down trees (No inventory system yet)  

![Tue 18 Jul 2023 21:01:47 IST](https://github.com/Ticketedmoon/vanquish/assets/21260839/db806127-c729-496e-bf47-cf09847c444e)

---

##### 2023-09-16

**'World Entities'**
- World expanded
- Tilemap Logic improved.
- New Player Spritesheet with 8 animations per direction.
- Player healthbar added.
- Debug View greatly improved.
- Enemies added to game world:
  - They move randomly every X seconds.
  - They will target and follow the player if the player is in their view/range.
  - They will damage the player if within close proximity.
  - Damage of enemies is currently randomly selected between a range.

![Sat 16 Sep 2023 18:37:11 IST](https://github.com/Ticketedmoon/vanquish/assets/21260839/72c614b2-4371-4b3b-b51f-dab6abd4f986)

**Debug View:** 

![Sat 16 Sep 2023 18:45:57 IST](https://github.com/Ticketedmoon/vanquish/assets/21260839/ff08f4a9-c724-4c8f-a0d8-291557f2b8fc)

---

##### 2023-09-18

**'Animation Overhaul'**
- Overhauled the animation system to easily add animations to entities.
- Animations added:
  - idle animation
  - hurt/damage-taken animation
  - death animation

![Mon 18 Sep 2023 00:35:11 IST](https://github.com/Ticketedmoon/vanquish/assets/21260839/eb3d50d7-b8c3-4cd9-9869-a036aeaaae34)

---

Resources:
- https://route1rodent.itch.io/16x16-rpg-character-sprite-sheet
