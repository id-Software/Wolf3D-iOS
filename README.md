<img align="left" width="100" height="100" src="https://raw.githubusercontent.com/tomkidd/Wolf3D-iOS/master/icon_wolf3d.png">  

#  Wolfenstein 3D for iOS 11 and tvOS for Apple TV

This is my update for Wolfenstein 3D for iOS to run on iOS 11, running in modern resolutions including the full width of the iPhone X. I have also made a target and version for tvOS to run on Apple TV

![screenshot](https://raw.githubusercontent.com/tomkidd/Wolf3d-iOS/master/ss_wolf3d.png)

Improvements/Changes

- Compiles and runs in iOS 11 SDK
- Orientation and coordinate system fixed to reflect iOS 8 changes
- C warnings fixed for Xcode 9.3
- Basic MFi controller support
- Deprecated APIs removed and replaced with modern equivalents
- Code locations reorganized and consolodated
- Second project target for tvOS that takes advantage of focus model and removes on-screen controls.

This commit adds a "menu_art" directory with placeholder images and a substitute font. These are images made from scratch and a public domain font in order to avoid distributing copyrighted material. 

Note that unlike the DOOM for iOS repository from id Software, this repository includes all the game content (levels, sounds, etc.) from the original Wolfenstein 3-D release because John Carmack included them in the initial drop in 2009. As a result, **you can clone this repository, build, and start playing immediately**. It does not include the Spear of Destiny levels as the original release did not have them, and the in-app purchase to purchase them has long since stopped working. If you try to start a Spear of Destiny episiode the app will crash, but if you have the files yourself from some other means you can drop them in the `base/maps` directory and play them. 

For a rundown of getting the code up and running in iOS 11, I wrote this [lengthy article](http://schnapple.com/wolfenstein-3d-and-doom-on-ios-11/) on the subject. For a rundown of the effort to get it running on tvOS, I wrote a [second lenghty article](http://schnapple.com/wolfenstein-3d-and-doom-on-tvos-for-apple-tv/) on the subject. 

[Video of Wolfenstein 3-D running on an iPhone X](https://www.youtube.com/watch?v=O_tC5krFVZ8)

[Video of Wolfenstein 3-D running on an Apple TV](https://www.youtube.com/watch?v=ghkjUi_1Bos)

I have also made apps for [*DOOM*, *DOOM II* and *Final DOOM*](https://github.com/tomkidd/DOOM-iOS), [*Quake*](https://github.com/tomkidd/Quake-iOS), [*Quake II*](https://github.com/tomkidd/Quake2-iOS), [*Quake III: Arena*](https://github.com/tomkidd/Quake3-iOS), [*Return to Castle Wolfenstein*](https://github.com/tomkidd/RTCW-iOS) and [*DOOM 3*](https://github.com/tomkidd/DOOM3-iOS).

Have fun. For any questions I can be reached at tomkidd@gmail.com