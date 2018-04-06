#  Wolfenstein 3D for iOS 11

This is my update for Wolfenstein 3D for iOS to run on iOS 11. It also runs in modern resolutions including the full width of the iPhone X.

Improvements/Changes

- Compiles and runs in iOS 11 SDK
- Orientation and coordinate system fixed to reflect iOS 8 changes
- C warnings fixed for Xcode 9.3
- Basic MFi controller support
- Deprecated APIs removed and replaced with modern equivalents
- Code locations reorganized and consolodated

This commit adds a "menu_art" directory with placeholder images and a substitute font. These are images made from scratch and a public domain font in order to avoid distributing copyrighted material. 

Note that unlike the DOOM for iOS repository from id Software, this repository includes all the game content (levels, sounds, etc.) from the original Wolfenstein 3-D release because John Carmack included them in the initial drop in 2009. As a result, **you can clone this repository, build, and start playing immediately**. It does not include the Spear of Destiny levels as the original release did not have them, and the in-app purchase to purchase them has long since stopped working. If you try to start a Spear of Destiny episiode the app will crash, but if you have the files yourself from some other means you can drop them in the `base/maps` directory and play them. 

Have fun. For any questions I can be reached at tomkidd@gmail.com