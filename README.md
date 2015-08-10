ScummVM for iPodLinux 
ver. 0.3 based on ScummVM revision 23292 (X11/SDL backend)

Downloads:

http://www.box.net/files#/files/0/f/34912494


Features

    * Full sound support for Adlib, PCSpeaker and PCJr
    * New control scheme, trackpad-like
    * Complete sources
    * Scaling should work. Support for iPod 5G, nano, video (?).
    * Savegames working 


Binaries

There are two binaries in the release folder:

    * scummvm_aq -> Audio Quality. This versions features the best audio quality for the best sound without stuttering. It is afflicted by a huge latency though. Due to the nature of graphical adventures, a perfectly timed audio is not always required to enjoy the game, so this issue may or may not bother you. If you want to enjoy a full instrumental Adlib sound choose this version. 

    * scummvm -> Standard version. The sound of this version is barely acceptable (choppy) but tightly timed with the graphics. Using Adlib sound it sometimes produces noises and stuttering and noticeable slowdowns. Nonetheless it runs and sounds great with PCSpeaker or IBM PCjr (better), if you want to go retro this version is raccomended choice. 

    * scummvm_fs -> I don't build this version anymore, the gain is marginal and the issues are still there. If you feel to, experiment with FSKIP and FRAMES_SKIPPED values in backend/ipod/sdl-common.h 

If you are using Zeroslackr, inside the 'release' folder there's a Launch directory containing two launch script. They will work as long as you place the Launch directory and the binaries inside

/ZeroSlackr/opt/Emulators/ScummVM

then add in your /loader.cfg files the following lines:

ScummVM_AUDIOQUALITY @ (hd0,1)/boot/vmlinux root=/dev/hda2 rootfstype=vfat rw quiet autoexec=/opt/Emulators/ScummVM/Launch/scummvm_aq.sh

ScummVM @ (hd0,1)/boot/vmlinux root=/dev/hda2 rootfstype=vfat rw quiet autoexec=/opt/Emulators/ScummVM/Launch/scummvm.sh

    * In those scripts, the processor is set to 78. 


Controls

    * Tap Wheel: Tap the wheel as a directional trackpad to move the mouse pointer (8 directions allowed)
    * (Pressed) Select button: Mouse Left Click
    * (Pressed) Rewind: Mouse Right Click
    * (Pressed) Forward: Mouse Right Click
    * (Pressed) Menu: In-Game Menu
    * (Pressed) Play/Pause: number 6 (to enter the codes and skip the protections, i.e. Monkey Island 2)
    * Hold: Esc/Skip 

How to right click on objects:

1) Just press Forward or Rewind on the hotspot. Be fast as the tapping will move the pointer at the same time.

or (better)

2) Press the Select Button over the object (that equals to the Left Button) and THEN add the Forward or Rewind button. The controls have been designed to stop the Tap Wheel moving the pointer when the Select Button is pressed, so while you prepare to press Forward or Rewind, the pointer will stay in place. Use this tecniques to "anchor" the pointer to an hot spot.
 Compatibility

This iPL build uses an older scummvm revision than the previous one, and should be identified as v.0.10.0

Accordingly, the ScummVM compatibility list is the following: http://www.scummvm.org/compatibility/0.10.0/

(with audio enabled) the DIG will crash after the intro both in the retail and the demo version (not enough memory), even if you skip the intro. (with audio enabled) COMI will not load (not enough memory). (with audio enabled) BS1 and BS2 will not load (not enough memory)
 
 
 Known Issues:

    * Due to the new control scheme, you could have difficulties browsing the audio options in the configuration GUI. In order to change the Audio device edit the .scummvmrc file located in the same position of the executable (launch the binary once before you do that). Note that the dot '.' before the name means that the file is usually hidden in Linux/MacOs: turn on the Display Hidden File option your system provide. 

I changed the location of the default ScummVM configuration file to help you do edit it, but you can also specify a custom config files by commandline (one of the Adlib games and one for the PCSpeaker games, for example). Refer to this page to understand how to edit those files: http://wiki.scummvm.org/index.php/User_Manual/Configuring_ScummVM#Using_the_configuration_file_to_configure_ScummVM As quick reference to change audio device, add to the [scummvm] section:

music_driver=

then chose between null, adlib, pcspk, pcjr. You can add the parameter also to the single game configuration, for example:

[monkey-vga]
description=The Secret of Monkey Island (VGA/DOS/English)
path=/mnt/ZeroSlackr/opt/Emulators/ScummVM/monkey
music_driver=pcjr
subtitles=false
platform=pc
gameid=monkey
language=en

Then remember to chose Edit Game and in the Audio tab, activate the checkbox Override Global configuration.

    * The audio driver "seq" which calls the OSS MIDI sequencer (dev/sequencer) does not work. 

    * Audio CD games ("Talkie") won't work (no sound) 

    * Due to an unresolved issue on input, your samegame will have a name like "??????????????????????????????" but it should load correctly. 

 
Older versions

    * v.0.2 http://rapidshare.com/files/312751033/scummvmAUDIO0.2.tar.gz 


To do:

    * COP. Melknin wrote some code for COP that does not crash but does not work as expected either. Here we need someone who knows what he's doing (please contact me). Hopefully this should fix the audio.
    * Eventually migrate this code to a newer revision. I tried but my audio implementation failed to work with the newer backend format: I don't know why, as the SDL/OSS build for the desktop works, the ipod build compiles but then crashes at the beginning cursing about threads. 

Thanks to the iPodlinux community, sid77 and melknin to have worked on scummvm before.

For comments and info refer to this forum post on ipodlinux.org: http://ipodlinux.org/forums/viewtopic.php?f=9&t=209&start=10


Contacts for this build

iPodLinux user: user:uomoartificiale email: gr2.decode AT gmail DOT com 