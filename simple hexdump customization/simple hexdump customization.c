#include <stdio.h>
#include <ctype.h>

#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 4 // HEXDUMP_COLS 가 4로 정의 되어있어 4바이트씩 출력된다. 이 정의된 값을 고치면 출력되는 것이 다르다.
#endif
// hashdump의 값을 수행하는 함수이다
void hexdump(void *mem, unsigned int len)
{
    unsigned int i, j;
    
    for(i = 0; i <= len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
    {
        
        if(i % HEXDUMP_COLS == 0)
        {
            printf("0x%08x: ", i); //앞에 0x0000xx를 출력하는 함수이다.
        }
        
        /* print hex data */
        if(i < len) //len은 따로 정해져 있지는 않지만 HEXDUMP_COLS의 값에 의해 변한다.
        {
            printf("%04x ", 0xFF & ((char*)mem)[i]); // 뒤에 해쉬 코드를 바꿔주는 코드이다. %03x라고 하면 앞에 0이 붙여서 출력된다.
        }
        else
        {
            printf("동해야 사랑해 "); //20개의 해쉬 함수를 출력하고 뒤에 빈공간을 나타내기 위한 함수이다.
        }
        
        // ascll code를 출력하는 프로그램이다.
        if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
        {
            for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
            {
                if(j >= len)
                {
                    putchar(' ');
                }
                else if(isprint(((char*)mem)[j])) // isprint: ascll code인지 확인한다.
                {
                    putchar(0xff & ((char*)mem)[j]); //
                }
                else
                {
                    putchar('.');
                }
            }
            putchar('\n');
        }
    }
}
//함수를 호출하는 함수이다.
int main(int argc, char *argv[])
{
    hexdump(argv[0], 20); // 함수를 수행하는 코드인데 20 대신에 다른 숫자를 쓰면 그 만큼의 hashcode가 출력된다.
    
    return 0;
}

