#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define MAX_MSG 64
unsigned char *msg[MAX_MSG];

static uint8_t dec64table[] = {
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255, /*  0-15 */
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255, /* 16-31 */
  255,255,255,255,255,255,255,255,255,255,255, 62,255,255,255, 63, /* 32-47 */
   52, 53, 54, 55, 56, 57, 58, 59, 60, 61,255,255,255,255,255,255, /* 48-63 */
  255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, /* 64-79 */
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,255,255,255,255,255, /* 80-95 */
  255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, /* 96-111 */
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,255,255,255,255,255  /* 112-127*/
};

int
b64decode(const uint8_t *code, uint8_t **ptr)
{
  int x, y;
  uint8_t *result = malloc(3*(strlen(code)/4) + 1);

  *ptr = result;

/* Each cycle of the loop handles a quantum of 4 input bytes. For the last
quantum this may decode to 1, 2, or 3 output bytes. */

while ((x = *code++) != 0)
  {
  if (isspace(x)) continue;
  /* debug_printf("b64d: '%c'\n", x); */

  if (x > 127 || (x = dec64table[x]) == 255) return -1;

  while (isspace(y = *code++)) ;
  /* debug_printf("b64d: '%c'\n", y); */
  if (y > 127 || (y = dec64table[y]) == 255)
    return -1;

  *result++ = (x << 2) | (y >> 4);
  /* debug_printf("b64d:      -> %02x\n", result[-1]); */

  while (isspace(x = *code++)) ;
  /* debug_printf("b64d: '%c'\n", x); */
  if (x == '=')		/* endmarker, but there should be another */
    {
    while (isspace(x = *code++)) ;
    /* debug_printf("b64d: '%c'\n", x); */
    if (x != '=') return -1;
    while (isspace(y = *code++)) ;
    if (y != 0) return -1;
    /* debug_printf("b64d: DONE\n"); */
    break;
    }
  else
    {
    if (x > 127 || (x = dec64table[x]) == 255) return -1;
    *result++ = (y << 4) | (x >> 2);
    /* debug_printf("b64d:      -> %02x\n", result[-1]); */

    while (isspace(y = *code++)) ;
    /* debug_printf("b64d: '%c'\n", y); */
    if (y == '=')
      {
      while (isspace(y = *code++)) ;
      if (y != 0) return -1;
      /* debug_printf("b64d: DONE\n"); */
      break;
      }
    else
      {
      if (y > 127 || (y = dec64table[y]) == 255) return -1;
      *result++ = (x << 6) | y;
      /* debug_printf("b64d:      -> %02x\n", result[-1]); */
      }
    }
  }

*result = 0;
return result - *ptr;
}


static uint8_t *enc64table =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

uint8_t *
b64encode(uint8_t *clear, int len)
{
uint8_t *code = malloc(4*((len+2)/3) + 1);
uint8_t *p = code;

while (len-- >0)
  {
  int x, y;

  x = *clear++;
  *p++ = enc64table[(x >> 2) & 63];

  if (len-- <= 0)
    {
    *p++ = enc64table[(x << 4) & 63];
    *p++ = '=';
    *p++ = '=';
    break;
    }

  y = *clear++;
  *p++ = enc64table[((x << 4) | ((y >> 4) & 15)) & 63];

  if (len-- <= 0)
    {
    *p++ = enc64table[(y << 2) & 63];
    *p++ = '=';
    break;
    }

  x = *clear++;
  *p++ = enc64table[((y << 2) | ((x >> 6) & 3)) & 63];

  *p++ = enc64table[x & 63];
  }

*p = 0;

return code;
}

int std_read(unsigned char *buf, unsigned int count)
{
	int idx = 0, i = 0;
	unsigned char c_in;

	for (i = 0; i < count; i++){
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
	printf("Which? :");
	std_read(s_idx, 8);
	return atoi(s_idx);
}

int output_choice()
{
	puts("1. read message");
	puts("2. edit message");
	puts("3. wipe message");
	puts("4. exit");
	return std_atoi();
}

void read_message()
{
	unsigned char msg_b64[0x400];

	int index = std_atoi();
	if (index >=0 && index < MAX_MSG && msg[index]==0)
	{
		printf("msg:");
		std_read(msg_b64, 0x400);
		b64decode(msg_b64, &msg[index]);
	}
}

void edit_message()
{
	int index = std_atoi();
	if (index >=0 && index < MAX_MSG && msg[index]!=0)
	{
		printf("new msg:");
		std_read(msg[index], strlen(msg[index]));
	}
}
void wipe_message()
{
	int index = std_atoi();
	if (index >=0 && index < MAX_MSG && msg[index]!=0)
	{
		free(msg[index]);
		msg[index] = 0;
	}
}

int main()
{

	int idx;

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0 );

	alarm(0x3c);
	sleep(3);

	while(1)
	{
		idx = output_choice();

		switch (idx)
		{
			case 1:
				read_message();
				break;
			case 2:
				edit_message();
				break;
			case 3:
				wipe_message();
				break;
			case 4:
				exit(0);
			default:
				puts("Invalid input\n");

		}

	}
}
