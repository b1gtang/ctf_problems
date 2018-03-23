#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct RC4_KEY_S{
    unsigned char S[256];
}RC4_KEY;

char *key;
char *box;
int has_key=0;

void rc4_init(unsigned char *s, unsigned char *key, unsigned long Len) //初始化函数
{
    int i =0, j = 0;
    char k[256] = {0};
    unsigned char tmp = 0;
    for (i=0;i<256;i++) {
        s[i] = i;
        k[i] = key[i%Len];
    }
    for (i=0; i<256; i++) {
        j=(j+s[i]+k[i])%256;
        tmp = s[i];
        s[i] = s[j]; //交换s[i]和s[j]
        s[j] = tmp;
    }
 }

void rc4_crypt(unsigned char *s, unsigned char *Data, unsigned long Len) //加解密
{
    int i = 0, j = 0, t = 0;
    unsigned long k = 0;
    unsigned char tmp;
    for(k=0;k<Len;k++) {
        i=(i+1)%256;
        j=(j+s[i])%256;
        tmp = s[i];
        s[i] = s[j]; //交换s[x]和s[y]
        s[j] = tmp;
        t=(s[i]+s[j])%256;
        Data[k] ^= s[t];
     }
} 

void init()
{
  unsigned int seed;

  alarm(0x1E);
  setvbuf(stdin,0,2,0);
  setvbuf(stdout,0,2,0);
  seed = time(0);
  srand(seed);
  key = malloc(0x10);
  box = malloc(0x100);

}

void read_input(char *buf, int len)
{
  char ch;
  int i;

  i = 0;
  if ( len  <= 1 )
  {
    buf[i] = 0;
  }
  else
  {
    while ( 1 )
    {
      read(0, &ch, 1uLL);
      if ( ch == 10 )
        break;
      buf[i++] = ch;
      if ( len - 1 == i )
        break;
    }
    buf[i] = 0;
  }
}

char read_choice(char *buf,int len)
{
  printf("> ");
  read_input(buf, len);
  return buf[0];
}

void main_menu()
{
  puts("Crypto Test!");
  puts("[a] Generate a key");
  puts("[b] Do encrypt");
  puts("[c] Do decrypt");
  puts("[d] Exit");
}

void key_menu()
{
  puts("[a] return to main");
  puts("[b] use static key");
  puts("[c] use random key");
}

void print_buf(unsigned char *buf,int len)
{
  int i;
  for(i=0;i<len;i++)
    printf("%02x",buf[i]);
  printf("\n");
}

void set_key()
{ 
  char buf[16];
  char static_key[16]= "0123456789abcdef";
  int i;
  key_menu();
  switch(read_choice(buf,8))
  {
    case 'a':
      return 0;
    case 'b':
      if (has_key==0)
      {
        memcpy(buf,static_key,16);
        has_key = 1;
      }
    case 'c':
      if (has_key==0)
      {
        for (i=0;i<16;i++)
          buf[i] = rand();
        has_key = 1;
      }
    default:
      memcpy(key,buf,16);
      break;
  }

  if (has_key)
  {
    print_buf(key,16);
    rc4_init(box,(unsigned char *)key,16);
  }
}

void do_game(char *buf)
{
  if (buf[0]==rand())
    printf(buf);
}

void do_exit()
{
  char buf[8];
  read_choice(buf,8);
  if (buf[0] == 'y')
  {
    exit(1);
  }
  else
  {
    do_game(buf);
    exit(1);
  }
}
int main()
{
  char buf[8] = {0};
  char s[0x100] = {0};
  int len;
  char *out;
  memset(s, 0, 0x100uLL);
  init();
  
  while ( 1 )
  {
    main_menu();
    switch(read_choice(buf, 8))
    {
      case 'a':
        set_key();
        break;
      case 'b':
      case 'c':
        if (has_key)
        {
          gets(s);
          len = strlen(s);
          rc4_crypt(box,s,len);
          print_buf(s,len);

        }
      case 'd':
        do_exit();
        break;
      default:
        puts("Invalid Choice!");
    }

  }
  return 0;
}