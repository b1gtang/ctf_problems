#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

#define MAX_INDEX 16

struct _chunk
{
	char * pointer;
	unsigned int length;
} chunk[MAX_INDEX];

unsigned int chunk_idx = 0;

int std_atoi()
{
	char s_idx[8] = {0};
        int len = read(STDIN_FILENO, s_idx, 8);
	return atoi(s_idx);
}

int output_banner()
{
	puts("Welcome to 0ctf 2018 finals\nhere you can");
	return 0;
}

int output_choice()
{
	puts("1. init a note");
	puts("2. edit a note");
	puts("3. free a note");
	puts("4. show a note");
	puts("5. exit");
	printf("Choice:");
	return std_atoi();
}

int Init_note()
{
	int idx = 0;
	int len = 0;
        char buf[0x100];
        unsigned int buf_len = 0;

        memset(buf, 0x100, '\x0');

	if ((chunk_idx<0) || (chunk_idx>=16))
	{	
		puts("Not allow~!");
                return 1;
	}
	
	printf("Input the note length:");
	len = std_atoi();
	if ((len<=0) || (len>256))
	{
		puts("Not allow~!");
		return 1;
	}

	printf("Input the note content:");

        buf_len = read(STDIN_FILENO, buf, 0xff);

        if ( buf_len > len)
                buf_len = len;

	char *A = malloc(len+8);

        if (A && buf_len > 0)
                memcpy(A, buf, buf_len-1);

	while (chunk[idx].length>0)
        {
                idx++;
        }
	chunk[idx].pointer = A;
	chunk[idx].length = len;
	
	chunk_idx++;
        printf("Here is your label: %d\n", idx);
	puts("Done~!");
	return 0;
}

int Set_note()
{
	int idx;
	printf("Input the note index:");
	idx = std_atoi();
	if ((idx<0) || (idx>MAX_INDEX))
	{
		puts("Not allow~!");
		return 1;
	}
	if (chunk[idx].length > 0)
	{
		printf("Input the note content:");
		unsigned int len = read(STDIN_FILENO, chunk[idx].pointer, chunk[idx].length);
	}
	else
	{
		puts("Not allow~!");
		return 1;
	}
	puts("Done~!");
	return 0;
}

int Delete_note()
{
	int idx;
	printf("Input the note index:");
	idx = std_atoi();
	if ((idx<0) || (idx>MAX_INDEX))
	{
		puts("Not allow~!");
		return 1;
	}

	if (chunk[idx].length > 0)	
	{
	//	chunk[idx].length = 0;
		free(chunk[idx].pointer);
	//	chunk_idx--;
	}
	else
	{
		puts("Not allow~!");
		return 1;
	}

	puts("Done~!");
	return 0;
}

void Show_note()
{
	puts("Not allow~!");
}

void Exit()
{
	puts("Bye~!");
	exit(0);
}

int main(int argc, char** argv, char** envp)
{
	int idx;
	
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0 );
	alarm(30);
	
	output_banner();

	while(1)
	{
		idx = output_choice();
		
		switch (idx)
		{
			case 1:
				Init_note();
				break;
			case 2:
				Set_note();
				break;
			case 3:
				Delete_note();
				break;
			case 4:
				Show_note();
				break;
			case 5:
				Exit();
			default:
				puts("Invalid input\n");
		
		}
		
	}
	
	return 0;
}
