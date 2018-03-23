#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#pragma pack(8)

int is_login = 0;
char *key="BB2FA36AAA9541F0";

typedef struct _course_info
{
    struct _course_info *next;
    int  course_id;
    char course_name[16];
}course_info, *pcourse_info;

typedef struct _mem_info
{
	char username[16];
	char password[16];
	char message[128];
	pcourse_info self_course_info;
}mem_info,*pmem_info;

pmem_info person_info = NULL;

void init()
{
	setvbuf(stdin,0,2,0);
	setvbuf(stdout,0,2,0);
	alarm(60);
}

void welcome()
{
	char *banner =
		"\n========= Welcome to XOCSS ========="
		"\n __   ________  _____  _____  _____ "
		"\n \\ \\ / /|  _  \\/  ___||  ___|/  __ \\"
		"\n  \\ V / | | | |\\ `--. | |__  | /  \\/"
		"\n  /   \\ | | | | `--. \\|  __| | |    "
		"\n / /^\\ \\| |/ / /\\__/ /| |___ | \\__/\\"
		"\n \\/   \\/|___/  \\____/ \\____/  \\____/"
		//"\n"
		//"\n####=======####=======####=======####"
		"\n"
		"\nXidian Online Course Selection System";
	puts(banner);
}

void menu()
{
	puts("--------------- Menu ---------------");
	puts("1. login in");
	puts("2. list all course");
	puts("3. add a course");
	puts("4. edit a course");
	puts("5. delete a course");
	puts("6. exit");
	puts("------------------------------------");
	printf("Your choice:");
}

signed int read_input(int fd,char input[],int len)
{
	signed int i;
	char ch;

	i = 0;
	if ( len > 0 )
	{
		while ( read(fd, &ch, 1) > 0 )
		{
			if ( ch != '\n' )
			{
				input[i++] = ch;
        		if ( i != len )
					continue;
      			}
      		return i;
		}
		i = -1;
	}
	//printf("%s\n",input);
	return i;
}

int check_shellcode(char buffer[],int len)
{
	int i;
	int result = 0;
	for (i=0;i<len;i++)
	{
		if (buffer[i]<'0' || buffer[i]>'z')
		{
			result = 1;
			break;
		}
	}
	return result;
}


void backdoor()
{
	char *shellcode;
	char buffer[128];
	int len;
	int result;

	memset(&buffer,0,128);
	//PYIIIIIIIIIIIIIIII7QZjAXP0A0AkAAQ2AB2BB0BBABXP8ABuJIFQkyHwhah0vk0QaxtoDorSCX1xDobB59rNlIXcHMOpAA
	shellcode = (char *)mmap(0x80000000,0x1000,7,50,-1,0);
	printf("$ ");
	len = read_input(0,buffer,128);
	result = check_shellcode(buffer,len);
	if (strstr(buffer,"Bigtang") && (result == 0))
	{
		strcpy(shellcode,buffer);
		(*(void (*)()) shellcode)();
	}
}

void do_exit()
{
	if( is_login )
		is_login = 0;
	if( person_info )
	{
        free( person_info );
        person_info = NULL;
    }
    puts("See you~");
    exit(0);
}

int do_login()
{
	char username[16];
	char password[16];
	if( is_login ){
        puts("you already logined!");
        return -1;
    }

    person_info = malloc( sizeof(mem_info) );
    if( !person_info )
        return -1;

    memset( person_info, 0, sizeof(mem_info) );

    printf("Please input your username:");
    gets(username);

    printf("Please input your password:");
    gets(password);


	if (strncmp(username,"student_",8)!=0)
	{
		is_login = 0;
		puts("Invalid username!");
		return -1;
	}
	    
	if (strcmp(password,"000000"))
	{
		puts("Initial password is '000000'");
		return -1;
	}
	else
	{
		is_login = 1;
	}
    
    if (is_login)
    {
    	strncpy(person_info->username,username,16);
		strncpy(person_info->password,password,16);
    	printf("Please leave a message:");
    	read_input(0,person_info->message,128);
    	puts("Successfully logined!Enjoy it!");
    	person_info->self_course_info = NULL;

    }

    return 0;
}


int do_list()
{
	if (is_login == 0)
	{
		puts("You must login system first!");
		return -1;
	}

	if (person_info->self_course_info == NULL)
	{
		puts("You must add a course first!");
		return -1;
	}

	pcourse_info pcourse_temp = NULL;
	pcourse_temp = person_info->self_course_info;

	puts("+-----------+-----------------+");
	puts("| course_id |   course_name   |");
	puts("+-----------+-----------------+");

	while (pcourse_temp!=NULL)
	{
		printf("| %08x  ",pcourse_temp->course_id);
		printf("|%16s |\n",pcourse_temp->course_name);
		//printf("%x\n",pcourse_temp->next);
		pcourse_temp = pcourse_temp->next;
	}
	puts("+-----------+-----------------+");


	// free(pcourse_temp);
	// pcourse_temp = NULL;
}

int do_add()
{
	char num[16];
	if (is_login == 0)
	{
		puts("You must login system first!");
		return -1;
	}
	pcourse_info pcourse_temp = NULL;
	pcourse_temp = (pcourse_info *)malloc(sizeof(course_info));

	printf("Input the course_id:");
	read_input(0,num,16);
	pcourse_temp->course_id = atoi(num);

	printf("Input the course_name:");
	read_input(0,pcourse_temp->course_name,16);

	pcourse_temp->next = person_info->self_course_info;
	person_info->self_course_info = pcourse_temp;
	//free(pcourse_temp);
	//pcourse_temp = NULL;
	return 0;
}

int do_edit()
{
	char num[16];
	int id;

	if (is_login == 0)
	{
		puts("You must login system first!");
		return -1;
	}

	if (person_info->self_course_info == NULL)
	{
		puts("You must add a course first!");
		return -1;
	}

	pcourse_info pcourse_temp = NULL;
	pcourse_temp = person_info->self_course_info;

	printf("Input the course_id:");
	read_input(0,num,16);

	id = atoi(num);

	while (pcourse_temp->course_id!=id)
	{
		pcourse_temp = pcourse_temp->next;
	}

	printf("Input the new course_name:");
	read_input(0,pcourse_temp->course_name,64);

	puts("Done!");


	return 0;
}

int do_delete()
{
	char num[16];
	int id;

	if (is_login == 0)
	{
		puts("You must login system first!");
		return -1;
	}

	if (person_info->self_course_info == NULL)
	{
		puts("You must add a course first!");
		return -1;
	}

	pcourse_info pcourse_temp = NULL;
	pcourse_temp = person_info->self_course_info;

	printf("Input the course_id:");
	read_input(0,num,16);

	id = atoi(num);

	if (pcourse_temp->course_id==id)
	{
		person_info->self_course_info = pcourse_temp->next;
		free(pcourse_temp);
		return 0;
	}

	while (pcourse_temp->next->course_id!=id)
	{
		pcourse_temp = pcourse_temp->next;
	}

	pcourse_temp->next = pcourse_temp->next->next;
	free(pcourse_temp->next);

	puts("Done!");

	return 0;
}


int get_choice()
{
	char nptr[16];
	read_input(0,nptr,16);
	return atoi(nptr);	
}
int main()
{	
	int choice;
	init();
	welcome();
	while (1)
	{
		menu();
		choice = get_choice();
		switch (choice)
		{
			case 1:
				do_login();
				break;
			case 2:
				do_list();
				break;
			case 3:
				do_add();
				break;
			case 4:
				do_edit();
				break;
			case 5:
				do_delete();
				break;
			case 6:
				do_exit();
				break;
			case 31337:
				backdoor();
				break;
			default:
				puts("Invalid number!");
		}
	}

	return 0;
}
