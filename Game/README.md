# Project Folder Structure

This document outlines the folder structure of the project and its purpose.

- **`app`**:  
  Contains all the source code and serves as the entry point for the game.
- **`bin`**:  
  Stores all the required `.dll` files. These files are included in the build and are automatically copied to the appropriate location based on their version using Visual Studio's Post-Build Event.
- **`build`**:  
  The default directory for build output files.
- **`extern`**:  
  Contains all third-party packages utilized in the project.
- **`lib`**:  
  Houses all library files (`.lib`) included in the project. These libraries are linked using Visual Studio's "Additional Library Directories" settings.
- **`resources`**:  
  Contains game assets, including sprites, sound files, and other resources necessary for the game.
