# backup-utlity

A program which provides a file backup utlility. The utility needs to provide for 2 kinds of backups -

1. Full
2. Incremental

The project's scope is - 

1. Uses a SQLite backend to save data about backup configs and backups
2. Allows taking a simple full backup of a directory
3. Allows taking an incremental backup of a directory once a full backup has been taken
4. Allows resuming a full backup if it was stopped for some reason
5. Displays all info about historical backups and backup_configs
6. Limits and constraints -
   1. Backups only simple files located in the root level of a directory
   2. Does not handle file deletion
   3. Functionalities have to be triggered manually
   
   
#### Build instructions

1. `mkdir build` (at the same level as src)
2. `cd build`
3. `cmake ..`
4. `make`
5. `./BackupUtility` is the target executable. Simply run the executbale to open the interactive cli interface.

### Development Environment
1. CMake: 3.23.2
2. OS: macOS 12.4
3. Compiler: Apple clang version 13.0.0 with arm64-apple-darwin21.5.0 target
