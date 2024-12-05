# Project Folder Structure Guide

This guide describes the purpose of each folder within the project.

- **`Binaries`**:  
  This folder stores all necessary `.dll` files. These files are included in the build process and are automatically moved to the correct location based on their version through Visual Studio's Post-Build Event.
- **`Builds`**:  
  Serves as the default directory for the output files of the build process.
- **`External`**:  
  Contains all third-party packages used in the project.
- **`Libraries`**:  
  This folder holds all library files (`.lib`) that are included in the project. These libraries are linked via Visual Studio's "Additional Library Directories" settings.
- **`Resources`**:  
  Stores game assets such as sprites, sound files, and other necessary resources for the game.

Folders not specifically mentioned, such as `Main` and `Math`, are considered individual projects within the larger structure.

The `Main` folder serves as the entry point of the program.
More information can be found in the `README.md` file of each respective project.
