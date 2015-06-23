#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>

using std::pair;
using std::make_pair;
using std::sort;
using std::vector;
using std::string;
using std::reverse;

int files = 0;
int directories = 0;
int links = 0;

bool need_sort = false;
bool need_reverse = false;

/*
	Author : Dmitry Prokopenko
	Date: 17 June 2015

		ITMO University

	This programm create a tree of folder in UNIX systems.
*/

//Colors for files
void print_file()
{
	//We work in terminal?
	if (isatty(fileno(stdout)))
	 printf("\033[4;32m"); //4 - Underscored. 32 - Green symbols
}

//Colors for files
void print_executable_file()
{
	//We work in terminal?
	if (isatty(fileno(stdout)))
 	 printf("\033[4;5;35m"); //4 - Underscored. 32 - Yellow symbols
}

//Colors for folders
void print_folder()
{
	//We work in terminal?
	if (isatty(fileno(stdout)))
	 printf("\033[1;31m");// 1 - Strong. 31 - Red symbols
}

//Normalize output
void print_normal()
{
	//We work in terminal?
	if (isatty(fileno(stdout))) 
		printf("\033[0m"); // 0 - default attributes
}

void print_link_title()
{
	//We work in terminal?
	if (isatty(fileno(stdout))) 
		printf("\033[1;36m"); // 1 - Strong 36 - Morskaya volna symbols
}

void print_link_to()
{
	//We work in terminal?
	if (isatty(fileno(stdout))) 
		printf("\033[42m"); // 42 - 

}

void tree(char* path, string before)
{
	// Link to folder
	DIR * dir;
	// Link to entry
	dirent * entry;
	// file status
	struct stat inode;
	// buffer
	char buf[1024];
	// path len
	int s;

	dir = opendir(path);
	if (!dir)
	{
		printf("Can't open this path: %s\n", path);
		return;
	}
	s = strlen(path);
	strcpy(buf, path);
	if(buf[s] != '/')
		buf[s] = '/';

	// files or folders in CURRENT folder
	vector<pair<int,char*> > names;

	while (entry = readdir(dir))
	{
		//Skip sys files
		if (strcmp(entry->d_name, ".") == 0) continue;
		if (strcmp(entry->d_name, "..") == 0) continue;
		strcpy(buf + s + 1, entry->d_name);
		buf[s + 1 + strlen(entry->d_name)] = 0;
		//Check type
		lstat(buf, &inode);
		if(S_ISDIR(inode.st_mode)) // folder
		{
			names.push_back(make_pair(1,entry->d_name));
			directories++;
		}
		else if(S_ISLNK(inode.st_mode)) // link
		{
			names.push_back(make_pair(2,entry->d_name));
		}
		else if(stat(buf, &inode) == 0 && inode.st_mode & S_IXUSR) // executable file
		{
			names.push_back(make_pair(4,entry->d_name));
			files++;
		}
		else //other
		{
			names.push_back(make_pair(3,entry->d_name));
			files++;
		}
	}
	if(need_sort) 
		sort(names.begin(),names.end());
	if(need_reverse) 
		reverse(names.begin(),names.end());
	for(int i = 0; i < names.size();++i)
	{
		strcpy(buf + s + 1, names[i].second);
		buf[s + 1 + strlen(names[i].second)] = 0;
		// "roots"
		printf("%s", before.c_str());
		string _and;
		if(i == names.size() - 1)
		{
			printf("└");
			_and = "    ";
		}
		else
		{
			printf("├");
			_and = "│   ";
		}
		switch(names[i].first)
		{
			// folder
			case 1:
				printf("── ");
				print_folder();
				printf("%s", names[i].second);
				print_normal();
				printf("\n");
				tree(buf,before + _and);
				break;

			//link
			case 2:
				links++;

				// get a real path to link
				char rp[300];
				memset(rp,0,sizeof(rp));
				readlink(buf,rp,sizeof(rp));

				printf("── ");
				print_link_title();
				printf("%s", names[i].second);
				print_normal();

				printf("->");
				print_link_to();
				printf(rp);
				print_normal();

				printf("\n");
				break;

			// executable
			case 4:
				printf("── ");
				print_executable_file();
				printf("%s", names[i].second);
				print_normal();
				printf("\n");
				break;

			// other
			default:
				printf("── ");
				print_file();
				printf("%s", names[i].second);
				print_normal();
				printf("\n");
				break;
		}
	}
	closedir(dir);
}

int main (int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <directory>\n", argv[0]);
		return 0;
	}
	// Attributes
	for(int i = 2;i < argc;i++)
		if(strcmp(argv[i],"-sort"))
			need_sort = true;
		else if(strcmp(argv[i],"-rev"))
			need_reverse = true;
	tree(argv[1], "");
	printf("\n");
	printf("Total:\n %d files\n %d directories\n %d links\n",files,directories,links);
}
