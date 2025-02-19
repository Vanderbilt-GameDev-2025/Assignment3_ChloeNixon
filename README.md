# Solaur System: Dinosaur Head Solar System
This is a 3d scene showing a simple, simulated solar system through colliding dino heads. The game is simply playing with the gravity parameters to see how the dinos interacts.

## Functionality
The assignment implemented custom gravity fields through the CustomGravity GD-extension node. The class overrides the _integrate_forces function to apply a gravitational force to the child 'planet' towards either a point or collision object, depending on the mode.

## Controls
R to restart, and menu widgets to interact with gravity parameters

## How to run
1. Clone this repository onto your computer
2. Open the Godot application, use the 'import' button to open the folder 'SolaurSystem' within the repo.
3. Run the application. The default scene should be set to 'mainscene.tcsn', otherwise run 'mainscene.tcsn' directly. 
4. If there are issues importing the GD nodes, run 
 ```bash
 git submodule update --init
 scons platform=<platform>
```
within the home directory of the repository (not within the game directory)
