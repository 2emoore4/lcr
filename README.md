lcr
===

This code is used to project patterns using the DLP Lightcrafter projector. It is also capable of using the projector, along with a camera, to do structured light range scanning (3d scanning).

This repo isn't very organized; it contains a bunch of different things. 
1) A python script to generate patterns.
2) An interface to send these patterns to the Lightcrafter projector (mostly written by TI).
3) Some scripts in different languages that process videos of these patterns in order to create point clouds (3d models from the scans).

Most of this code is tailored to my setup, but as I continue, I'll try to make it more generic so that this could possibly be used by others.
