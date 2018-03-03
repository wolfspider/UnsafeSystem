# UnsafeSystem
OS X build of the unsafe* system
This Requires Cairo to link and run which I have not included. This was the boilerplate for cairo-ios-gles but while working with it have found a way to make it somewhat *stable* which I will continue to improve if necessary. Right now trap_render is the only function which includes the null pointer check which makes all the magic happen. Glib,SDL,SDL_Image, and Rsvg are not necessary to make this function correctly and can be taken out at your discretion they are just included in the testbed until release.
