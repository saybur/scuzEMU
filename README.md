scuzEMU
=======

Welcome to scuzEMU, an open source Mac "SCSI EMulator Utility." This program
allows you to remotely control certain SCSI emulators to:

* Download files within a /shared/ folder on the SD card,
* Upload files into that folder,
* Switch between different removable disk images on supported drives (CD-ROMs).

![example download in progress](https://github.com/saybur/scuzEMU/blob/main/extra/example_download.png?raw=true)

Compatibility
-------------

Minimum firmware versions tested to work with this application:

* ZuluSCSI 2024.07.16
    * In `zuluscsi.ini` under the `[SCSI]` section, requires `EnableToolbox=1`.
* scuznet
    * Only file downloads from the `shared` directory are supported.
    * Requires firmware built after 2024-07-16 with `-DUSE_TOOLBOX` set.

If you find this works with an emulator, or if you have changes you would like
to contribute, please contact @saybur via 68kmla.org or let me know on Github.

What's Inside This Document
---------------------------

Information about the program, basic usage instructions, and more
self-indulgent stuff in the style of old-school "Read Me First" documents
included with Mac apps.

How To Use This Program
-----------------------

1. Start the application by double-clicking the scuzEMU program.
2. Choose the SCSI ID you have your emulator configured to use.
3. Choose whether you want to download files or change images.
4. Press OK.
5. Double-click an item to either download it (in files mode) or change to that
   image (in images mode).

When switching images, you will still need to drag the current image to the
Trash to "eject" it. The image chosen should be loaded automatically.

A Note of Caution
-----------------

To work, this software needs to issue SCSI commands in the undocumented vendor
range. This could _theoretically_ result in an unsupported device doing
something unexpected. While it is unlikely anything catastrophic would happen
in response to these commands, it is still best to use care when selecting the
SCSI ID and only choose known-working devices.

As of version 0.4, the application checks the mode page version and will notify
you if it thinks there might be an incompatibility. You can bypass this by
clicking "Yes" when asked if you'd like to continue connecting.

Bugs And Missing Features
-------------------------

The program should be mostly feature-complete. Any known problems or planned
enhancements will be listed on Github.

I am a novice Mac application developer and the code likely shows it. There are
many more bugs yet to be discovered. If you find one, please report it!

Why Did You Make This?
----------------------

There was no existing open source Mac client implementation for the
emulator-specific SCSI commands originally made by the BlueSCSI project for
their devices, then later ported to ZuluSCSI and possibly others. I wanted one,
so I made one (such as it is). I received no compensation to write this program
and nobody asked me to create it.

This program is intended to be emulator agnostic. It will not perform checks
against a specific device vendor name or otherwise try to limit your options to
a particular ecosystem. Open source software allows you, the user, maximum
freedom to make the choices you like, change things to fit your needs, and
otherwise tinker to your heart's content. Please support user freedom by
choosing software available under an OSI-approved open license whenever
possible. See www.opensource.org for details.

Changelog
---------

Major changes in each release:

### 0.5

- Fixed some significant bugs found on earlier systems.
- User interface adjustments.

### 0.4

- You can now upload to the memory card via the File menu.
- Keyboard navigation has been added.
- Mode page parsing of the API version.
- Included type/creator for Compact Pro.

### 0.3

- Switching CD images is now automatic.
- Type/creator is assigned to certain file types (hqx/bin/sit).
- Files can download while the program is in the background.

### 0.2

- Multiple files per download are now supported.
- Added progress bar dialog for transfers.
- List of files/images will always be alphabetized.

### 0.1

- First alpha release.

License
-------

This program is licensed under the GNU GPL, version 3. The full details of the
license are in the COPYING document accompanying this software.
