
The original Wolfenstein 3D code was written in late 1991 / early 1992 using 16 bit Turbo C and the TASM assembler and targeted at 286 based MSDOS systems with VGA graphics and ideally a bit of extended or expanded memory.

I released the original source for Wolfenstein 3D many years ago, originally under a not-for-commercial purposes license, then later under the GPL.  The old code is still available in various places ( http://www.btinternet.com/~belowe/ ) but it isn't very useful on modern platforms.  There are several open source projects that have modernized the code so that it works on 32 bit systems and can take advantage of OpenGL acceleration.  I started the iphone version with the Wolf3D Redux codebase ( http://wolf3dredux.sourceforge.net/ ), which apparently incorporated a lot of code from NewWolf ( http://newwolf.sourceforge.net/ ).

At first, I considered trying to build the iphone version as a patch, but when I decided to turn the little research project into a commercial release (and do it in a hurry), I started making more wholesale changes.  The Redux codebase had basically gutted the Quake 2 codebase and grafted Wolfenstein into it, which had some nice points, but it meant that the system code was many times as large as the actual Wolfenstein game code.  It wasn't really hurting anything, and I considered leaving it all in, but it was such a mess that I finally flattened everything out and cut out about half of the environment code.  No attempt was made to make this project portable, although it wouldn't be very hard to clean that up.

In the past, Id source releases did not include any data files, and you had to extract data files from a commercially obtained version of the game if you wanted to experiment with the original game data.  Because it isn't possible for users to tear open an app bundle from the App Store to get at the data, I am including it with the source code to make it easy.  You are on-your-honor to buy a copy at the App Store before using the data. :-)  The source code is under the GPL, but the data is still strictly copyright Id Software with no license given to distribute outside this code release package or to use for any commercial purpose.  You are certainly free to replace all the data and make commercial applications, as long as the code is made available under the GPL.

/newCode/wolf	The 32 bit Wolfenstein code
/newCode/env	The Quake 2 derived code
/newCode/iphone	The newly written iphone code and xcode project files
/newCode/Tremor	Unodified ogg Tremor code for the background music
/base		Game data

I can't say there is a lot of really good code here -- the wolf code is mutated, the quake 2 code is vestigial, and the new code was written in a hurry, but it does all hang together as a pretty fun game to play, and a good testbed for various things.

If anyone does build another quality commercial application based on this code, let us know, and we can probably do some kind of cross linking.

John Carmack
2009/03/20

