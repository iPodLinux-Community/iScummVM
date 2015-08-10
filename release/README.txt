ScummVM for iPodLinux 
ver. 0.2 based on ScummVM revision 23292 (X11/SDL backend)


Features:
- Full sound support for Adlib, PCSpeaker and PCJr
- New control scheme
- Complete sources
- No scaling. Supports games originally at 320x240 resolution. Requires iPod 5G.
- Savegames working


Binaries
There are three binaries in the release folder:
scummvm_aq -> Audio Quality. This versions features the best audio quality for the best sound without stuttering. It is afflicted by a huge latency though. Due to the nature of graphical adventures, a perfectly timed audio is not always noticeable so this issue may or may not bother you. If you want to enjoy a full instrumental Adlib sound choose this version.

scummvm -> Standard version. The sound of this version is acceptable (choppy) and tightly timed with the graphics. Using Adlib sound it sometimes produces noises and stuttering. If the game supports PCSpeaker or IBM PCjr sound (better), this version is raccomended choice.

scummvm_fs -> This version is exactly like the previous one with a frameskip of 4 frames to help the iPod to sustain the weight of the heavier games. Use it only if the game is really too slow or choppy to be playable. It could partially fix the Adlib sound issues (tested to be resolved completely at more than 25 frames skip!).

If you are using Zeroslackr, inside the 'release' folder there's a Launch directrory containing three launch script. They will work as long as you place the Launch directory and the binaries inside /ZeroSlackr/opt/Emulators/ScummVM
then add in your /loader.cfg files the following lines:
ScummVM_AUDIOQUALITY @ (hd0,1)/boot/vmlinux root=/dev/hda2 rootfstype=vfat rw quiet autoexec=/opt/Emulators/ScummVM/Launch/scummvm_aq.sh
ScummVM @ (hd0,1)/boot/vmlinux root=/dev/hda2 rootfstype=vfat rw quiet autoexec=/opt/Emulators/ScummVM/Launch/scummvm.sh
ScummVM_FRAMESKIP @ (hd0,1)/boot/vmlinux root=/dev/hda2 rootfstype=vfat rw quiet autoexec=/opt/Emulators/ScummVM/Launch/scummvm_fs.sh

In those scripts, the processor is set to 78 (try 80 for the FRAMESKIP version to minimize choppy audio).


Controls
Tap Wheel: Tap the wheel as a directional trackpad to move the mouse pointer (8 directions allowed)
(Pressed) Select button: Mouse Left Click
(Pressed) Rewind: Mouse Right Click
(Pressed) Forward: Mouse Right Click
(Pressed) Menu: In-Game Menu
(Pressed) Play/Pause: number 6 (to enter the codes and skip the protections, i.e. Monkey Island 2)
Hold: Esc/Skip 


How to right click on objects:
1) Just press Forward or Rewind on the hotspot. Be fast as the tapping will move the pointer at the same time. 
or (better)
2) Press the Select Button over the object (that equals to the Left Button) and THEN add the Forward or Rewind button. The controls have been designed to stop the Tap Wheel moving the pointer when the Select Button is pressed, so while you prepare to press Forward or Rewind, the pointer will stay in place. Use this tecniques to "anchor" the pointer to an hot spot. 


Source code:
To compile, follow the instructions here: http://www.ipodlinux.org/wiki/ScummVM#Compiling_melknin.27s_patch


Known Issues:
- Due to the new control scheme, you could have difficulties browsing the audio options in the configuration GUI. In order to change the Audio device edit the .scummvmrc file located in the same position of the executable (tun it once before you do that). I changed the location of the ScummVM configuration file to help you do this, but you can also specify a config files by commandline for any version of the executables (one of the Adlib games and one for the PCSpeaker games, for example). Refer to this page to understand how to edit those files: http://wiki.scummvm.org/index.php/User_Manual/Configuring_ScummVM#Using_the_configuration_file_to_configure_ScummVM
As quick reference to change audio device, addthe the [scummvm] section:
music_driver=
then chose between null, adlib, pcspk, pcjr
- The adusio driver "seq" which calls the OSS MIDI sequencer (dev/sequencer) does not work. 
- Audio CD games ("Talkie") won't work (no sound)
- Due to an undetected issue on input, your samegame will have a name like "??????????????????????????????" but it should load correctly.


To do:
- COP. Melknin wrote some code for COP that does not crash but does not work as expected. Here we need someone who knows what he's doing (please contact me). Hopefully this should fix the audio.
- Display scaling. The Melknin code should work, but I only own a 5G so I didn't merge it (as it won't work out-of-the-box). If you want to play those games and own a nano or a video, please contact me and help me test the builds.
- Eventually migrate this code to a newer revision. I tried but my audio implementation failed to work with the newer backend format: I don't know why, as the SDL/OSS build for the desktop works, the ipod build compiles but then crashes at the beginning cursing about threads.


Thanks to the iPodlinux community, sid77 and melknin to have worked on scummvm before.
For comments and info refer to this forum post on ipodlinux.org:
http://ipodlinux.org/forums/viewtopic.php?f=9&t=209&start=10


Contacts:
iPodLinux user: uomoartificiale
email: gr2.decode AT gmail DOT com


Downloads:
http://www.box.net/files#/files/0/f/34912494


