# Ubisoft Next 2024-2025

## Game Overview

This game is a cooperative experience where the player controls a red sphere and an AI controls a blue sphere. The objective is to clear all the courses, represented by medium-sized yellow spheres. Along the way, you'll encounter obstacles in the form of small green spheres that will slow you down due to physical effects.

Your goal is to minimize the total number of strokes. For example, you can collide with your teammate to help each other reach challenging holes and reduce your total strokes. However, be cautious—hitting obstacles on the ground can affect both your position and your teammate’s! Plan your moves wisely!

## Build Guide

1.  Navigate to the `Game` directory.
2.  Open `Game.sln`.
    - Note: Visual Studio may not immediately display a file. If this happens, open the **Solution Explorer** on the right. You should see the following projects listed: `Main`, `Math`, `Physics`, `Rendering`, `StateMachine`, and `VRPSolver`.
3.  In the top-left corner of Visual Studio, switch the build mode from **Debug** to **Release**.
    - Debug mode can be built, but it does not support vectorization and is not playable.
4.  Build the game by either:
    - Pressing **Ctrl + Shift + B**, or
    - Clicking the green play button in the IDE to run it directly.
5.  Alternatively, you can launch the game by running the `.exe` file located in `Builds/x64/Release`.

If you encounter any errors related to `xcopy`, simply rebuild the project. These issues are likely occasional errors caused by MSVC and can often be resolved with a second build attempt.
