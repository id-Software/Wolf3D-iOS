#  Wolfenstein 3D for iOS 11

This is my update for Wolfenstein 3D for iOS to run on iOS 11. It also runs in modern resolutions including the full width of the iPhone X.

Improvements/Changes

- Compiles and runs in iOS 11 SDK
- Orientation and coordinate system fixed to reflect iOS 8 changes
- C warnings fixed for Xcode 9.3
- Basic MFi controller support
- Deprecated APIs removed and replaced with modern equivalents
- Code locations reorganized and consolodated

This commit only includes the changes made to the original source code and the original files. A "menu_art" directory is still required to build correctly, but as that consists of copyrighted material, I have not included it in this commit. 

My plan is to do a pull request to id Software on this commit and then make a second commit with placeholder menu art for others to be able to compile from scratch.