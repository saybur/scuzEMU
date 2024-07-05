scuzEMU
=======

Welcome to scuzEMU, an open source Mac "SCSI EMulator Utility." This program
allows you to remotely control certain SCSI emulators to:

* Download files within a /shared/ folder on the SD card,
* Switch between different removable disk images on supported drives (CD-ROMs).

Compatibility
-------------

Minimum firmware versions tested to work with this application:

* ZuluSCSI 2024.05.17

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
SCSI ID and only choose known-working devices. A future release may incorporate
checking for the paranoid but I would like to avoid artificially limiting
compatibility; see Why Did You Make This? for thoughts on that.

Bugs And Missing Features
-------------------------

The following issues are known to exist in this application:

* Keyboard arrows / enter key are not handled by the file/image list.
* Uploading files is unimplemented (I haven't decided if this would be useful
  or not).
* Ejecting disks has to be done by the user. It should be possible to integrate
  that step into the application, at least in most circumstances.
* Some graphical elements are missing.
* Downloading is slower than it should be; adding blind transfers on supported
  systems would probably improve this.
* If a SCSI drive reports a fault the software will not issue the needed
  REQUEST SENSE to address it. You will likely need to restart your computer to
  fix this problem.

More generally:

* Testing on System 6 has been minimal. It should theoretically work, at least
  under 6.0.8, but as a kid who grew up with System 7 I prefer that environment
  on my retro Macs and have done most of my debugging there.
* The user interface is simple, to a fault. I haven't decided if that is good
  or bad yet.
* As noted earlier no checks are made to verify compatibility for commands.

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
