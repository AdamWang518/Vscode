/*******************************************************************************
 *
 * to list directory content
 *
 ******************************************************************************/

#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <stdio.h>


#define MAX_PATH_LEN	500

int main()
{
	char cwd[MAX_PATH_LEN];
	DIR *dp;
	struct dirent *dir_ent_info;
	char ent_path_name[MAX_PATH_LEN*2];
	struct stat inode_info;
	struct passwd *pwd_ent;

	getcwd (cwd, MAX_PATH_LEN);
	dp = opendir (cwd);
	while ((dir_ent_info=readdir(dp))!=NULL) {
		//get inode info of an directory entry
		sprintf (ent_path_name, "%s/%s", cwd, dir_ent_info->d_name);
		stat (ent_path_name, &inode_info);
		pwd_ent = getpwuid (inode_info.st_uid);
		int is_file = S_ISREG (inode_info.st_mode);
		int is_dir = S_ISDIR (inode_info.st_mode);
		if(is_file)
		{
			printf("%s","<REG>");
		}
		if(is_dir)
		{
			printf("%s","<DIR>");
		}
		printf ("%s\t\t%s\n", dir_ent_info->d_name, pwd_ent->pw_name);
		//print last modified time
		printf("Last modified time is: %s\n", ctime(&inode_info.st_mtime));
	}

	return 0;
}//main()

















