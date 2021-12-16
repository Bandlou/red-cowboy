# Red Cowboy
- Game engine: __Unreal (v5.0)__
- Contribution: __Louis BANDELIER__
- Keywords: __lock system__, __animation tree__, __behavior tree__

A 4-day project to try to implement a RDR2 lock system on Unreal Engine 5.0. NPCs use a behavior tree to freely roam and react to the player's actions. The player can lock an NPC and interact with him.

## Showcase
![Demo showcase](ReadmeAssets/Demo.gif)

- [Debug mode showcase](ReadmeAssets/Debug.gif)
- [AI behavior tree showcase](ReadmeAssets/AI.gif)

## Project timeline
### Day 1
- Setup blank project
- Recreated the third person template with an interest on the following features:
  - Character class, controller, 3D model, materials, animations, animation tree
  - Game mode default classes
- Add an NPC character with AI controller supported by a behavior tree, a blackboard and tasks
- Add a navmesh to the map and a simple roam behavior for the AI controlled NPCs
- Add a pawn sensing component to the NPC character

### Day 2
- Add the dectection of actors around player
- Add an NPC lock system:
  - Detection:
    - Select valid NPCs around the player (valid angle & distance)
    - Give the selected NPCs a score based on angle & distance
    - Select the NPC with the highest score
    - (*Day 3*) Update the GUI to signal a NPC is lockable
  - Lock:
    - If not already locked and if there is a lockable NPC
    - Lock the NPC
    - (*Day 3*) Update the GUI to signal a NPC is locked and display available interactions

### Day 3
- Add custom actor rotation system
- Update NPC lock system:
  - Look for new candidates only if not already locked
  - Give locked character more distance before being unlocked
  - Increase maximal angle for valid lock
  - Update score function
  - Make player face its locked target
- Update AI roam controller:
  - Transition from Blueprint to C++ class

### Day 4
- Improve character rotation system
- Add a NPC interaction system:
  - Greet: the AI stops roaming, faces the player, waits a moment, goes back to normal
  - Aim weapon: the AI stops what he does, run around until the player stops aiming at him
- Add dialog system (+small lines for interaction)
- Add an animation for aim weapon
