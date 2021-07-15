#include "sys/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "base64.h"

void test_encoding_string()
{
    char str[] = "    Hello, world!\n    你好，世界！\n";
    printf("\nThe original string is:\n%s\n", (const char *)str);
    uint32_t size = Base64_getDecodedSize((uint8_t *)str, strlen((const char *)str));
    uint8_t *encodedStr = (uint8_t *)malloc((size_t)(size + 1));
    if(encodedStr)
    {
        Base64_encodeBytes((uint8_t *)str, (uint32_t)strlen((const char *)str), encodedStr);
    } else {
        return;
    }
    printf("The encoded string is:\n    %s\n\n", (const char *)encodedStr);
}

int main(int argc, char *argv[])
{   
    test_encoding_string();
    return 0;
}