# MGP-Animation-Tool
To simplify and optimize the hitbox animation pipeline

## The three parts of the animation tool
Part 1: A GUI that lets the user create an animation and draw hitboxes (and other features in the future) onto the individual frames of an animation.

Part 2: A JSON protocol that sets the rules for how a group of animations are stored in disc. This group of animations is called a "Character".

Part 3: A converter that takes a Character JSON input file and then outputs files that can be used to play the animations in your game engine/framework of choice.

All these different parts may be their own executable and therefore might have their own individual version numbers.

## Version 1 goals:
Create a tool that lets you create, save and load a "Character".

A "Character" is a JSON file containing the information such as:
- The version of the JSON file protocol
- The frame rate of the Character's animations
- The name of the Character
- A list of sprite sheets that the Character uses
- A list of animations that the Character uses

An animation is made up by sprites and hitboxes.

The tool should let the user create/remove/edit animations and draw/edit/remove hitboxes to individual frames of an animation. With this there has to be a version 1 of the JSON protocol, and a converter that takes a Character and creates files that can play these animations in Unity.
