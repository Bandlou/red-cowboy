# Red Cowboy
- Game engine: __Unreal (v5.0)__
- Contribution: __Louis BANDELIER__

A 4-day project to try to implement some basic RDR2 gameplay features.

## [2021/12/09] Day 1
- Setup blank project
- Recreated the third person template with an interest on the following features:
  - Character class, controller, 3D model, materials, animations, animation tree
  - Game mode default classes
- Add an NPC character with AI controller supported by a behavior tree, a blackboard and tasks
- Add a navmesh to the map and a simple roam behavior for the AI controlled NPCs
- Add a pawn sensing component to the NPC character

## [2021/12/10] Day 2
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

## [2021/12/11] Day 3
- Add custom actor rotation system
- Update NPC lock system:
  - Look for new candidates only if not already locked
  - Give locked character more distance before being unlocked
  - Increase maximal angle for valid lock
  - Update score function
- Add a NPC interaction system:
    - If a NPC is locked
    - (WIP)

## [2021/12/13] Day 4
- WIP
