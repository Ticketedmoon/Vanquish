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

##### 2023-09-17

**'Animation Overhaul'**
- Overhauled the animation system to easily add animations to entities.
- Animations added:
  - idle animation
  - hurt/damage-taken animation
  - death animation
  - sword swing animation

![Mon 18 Sep 2023 00:35:11 IST](https://github.com/Ticketedmoon/vanquish/assets/21260839/eb3d50d7-b8c3-4cd9-9869-a036aeaaae34)

---

##### 2023-09-18

**'Usual RPG updates'**
- Enemies can die after enough player sword swings.
- The player gains experience points (`exp`) from these fallen enemies and can level up.
- Basic GUI to show player banner, player level and `exp` bar. 

### Starting Off
![Mon 18 Sep 2023 21:07:50 IST](https://github.com/Ticketedmoon/vanquish/assets/21260839/e632dbd2-e4f1-4b16-9543-d8769972014b)

### Exp. gained
![Mon 18 Sep 2023 21:14:17 IST](https://github.com/Ticketedmoon/vanquish/assets/21260839/868d2785-eaec-4b1e-9107-75dba4928842)

---

##### 2023-09-19

**'More UI additions'**

- Enemy health bars are displayed by default and will reduce upon receiving damage as one might expect. 
- Debug UI also improved in this update to show bounding box around player and other world entities. 

### Enemy health bars
![Tue 19 Sep 2023 18:28:15 IST](https://github.com/Ticketedmoon/vanquish/assets/21260839/66159f67-c8b8-4152-aa40-a28f0efb1c72)


##### 2023-09-20

**'Entity-Component-System'**

- Today has been about researching into the Entity-Component-System architecture for managing our game.  
- This may spill over into other days for this to be incorporated into the codebase and the current architecture revamped.
See the **resources** section at the bottom of this `README` for links to ECS information sources.

---

Resources:
- https://route1rodent.itch.io/16x16-rpg-character-sprite-sheet
- https://www.codingwiththomas.com/blog/an-entity-component-system-from-scratch