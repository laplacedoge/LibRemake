#include "sys/types.h"
#include <stdio.h>
#include "common.h"
#include "base64.h"

int main(int argc, char *argv[])
{   
    char bytes[] = "Hello, this is the computer world!\n";
    char dest[]  = "Hello, this is the computer world!\n                                                                                           ";
    Base64_funcTest((uint8_t *)bytes, sizeof(bytes), (uint8_t *)dest);
    printf("%s\n", (uint8_t *)bytes);
    printf("%s\n", (uint8_t *)dest);
    return 0;
}