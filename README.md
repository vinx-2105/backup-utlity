# os-backup-utlity

A program which provides a file backup utlility. The utility needs to provide for 2 kinds of backups -

1. Full
2. Incremental

The project is currently very limited in its scope - 

1. Uses a SQLite backend to save data about backup configs and backups
2. Allows taking a simple full backup of a directory
3. Allows taking an incremental backup of a directory once a full backup has been taken
4. Allows resuming a full backup if it was stopped for some reason
5. Displays all info about historical backups and backup_configs
6. Limits and constraints -
   1. Backups only simple files located in the root level of a directory
   2. Does not handle file deletion
   3. Functionalities have to be triggered manually
