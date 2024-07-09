Building scuzEMU
================

I develop this using Symantec/THINK C++ 7. Earlier versions of THINK C down to
version 5 may work as well. This is the basic process to compile this yourself:

1. Clone the repository to a modern Linux system, or another computer that has
   access to the appropriate console environment.
2. Execute `script/export` to generate the `export` folder with files in Mac
   Roman / Mac line ending format.
3. Copy the files over to the development Mac.
4. In THINK Project Manager, create a new project folder.
5. Add all code files in `export` to the project (*.c, *.h, *.r). You will need
   to assign the type `TEXT` to `scuzemu.r` or it will not show up in the list
   of files.
6. Navigate to your "Mac Libraries" folder, find `MacTraps` and add it to the
   project.
7. Go to the Project menu, choose "Set Project Type..." and set "Creator" to
   `scuz`. Set "Partition (K)" to `128` and "SIZE Flags" to 5880.
8. Go to Project again, choose "Build Application..." and select where you want
   to save the compiled binary.

To get changes back into the git repo, copy changed files back to your modern
computer and put them in a folder called `import`. Use `script/import` to
**overwrite** any matching files in the `src` folder.

If you make changes to the compiled `.rsrc` file you'll need to re-export it.
Use `SADerez` in the Development/Utilities folder. Default settings seem fine,
but be sure to select an output path before you execute the program.
