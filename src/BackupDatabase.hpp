class BackupDatabase {
    public:
        virtual void create_backup()=0;
        virtual void update_backup()=0;
        virtual void create_backup_config()=0;
        virtual void update_backup_config()=0;
        virtual void delete_backup_config()=0;
};