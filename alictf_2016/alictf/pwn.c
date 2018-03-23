#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

#define MAX_INDEX 16

struct _chunk
{
	long pointer;
	long length;
} chunk[MAX_INDEX];

int chunk_idx = 0;

int std_read(unsigned char *buf, unsigned int count)
{
	int idx = 0, i = 0;
	unsigned char c_in;

	for (i = 0; i <= count; i++){
	if ( read(STDIN_FILENO, (void *)&c_in, 1) < 0 )
	{
		return -1;
	}
	if( c_in == '\n' )
	{
		*(buf + i) = 0;
		return i;
	}
	
	*(buf + i) = c_in;
	}
	buf[i-1] = 0;

	return i-1;
}

int std_atoi()
{
	char s_idx[8] = {0};
	std_read(s_idx, 8);
	return atoi(s_idx);
}

int output_banner()
{
	puts("Welcome to Alibaba Living Area, here you can");
	return 0;
}

int output_choice()
{
	puts("1. Init the message");
	puts("2. Set the message");
	puts("3. Delete the message");
	puts("4. Show the message");
	puts("5. Exit");
	printf("Choice:");
	return std_atoi();
}

int Init_message()
{
	int idx = 0;
	int len = 0;
	if ((chunk_idx<0) || (chunk_idx>=16))
	{	
		puts("Not allow~!");
                return 1;
	}
	
	printf("Input the message length:");
	len = std_atoi();
	if ((len<0) || (len>256))
	{
		puts("Not allow~!");
		return 1;
	}
	char *A = malloc(len);

	while (chunk[idx].length>0)
        {
                idx++;
        }
	chunk[idx].pointer = A;
	chunk[idx].length = len;
	
	chunk_idx++;
	puts("Done~!");
	return 0;
}

int Set_message()
{
	int idx;
	printf("Input the message index:");
	idx = std_atoi();
	if ((idx<0) || (idx>MAX_INDEX))
	{
		puts("Not allow~!");
		return 1;
	}
	if (chunk[idx].length > 0)
	{
		printf("Input the message content:");
		std_read(chunk[idx].pointer, chunk[idx].length);
	}
	else
	{
		puts("Not allow~!");
		return 1;
	}
	puts("Done~!");
	return 0;
}

int Delete_message()
{
	int idx;
	printf("Input the message index:");
	idx = std_atoi();
	if ((idx<0) || (idx>MAX_INDEX))
	{
		puts("Not allow~!");
		return 1;
	}

	if (chunk[idx].length > 0)	
	{
		chunk[idx].length = 0;
		free(chunk[idx].pointer);
		chunk_idx--;
	}
	else
	{
		puts("Not allow~!");
		return 1;
	}

	puts("Done~!");
	return 0;
}

void Show_message()
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
				Init_message();
				break;
			case 2:
				Set_message();
				break;
			case 3:
				Delete_message();
				break;
			case 4:
				Show_message();
				break;
			case 5:
				Exit();
			default:
				puts("Invalid input\n");
		
		}
		
	}
	
	return 0;
}
