#include "sys/types.h"
#include "common.h"
#include "base64.h"

/* Be true to it's name. */
const char _Base64DigitIndexTable[] = 
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

/**
 * Convert 3 original bytes to 4 digits by using Base64 encoding(only once).
 * @param bytes The bytes that needs to be encoded.
 * @param dest  The memory used to hold the result of our encoded Base64 digit string.
 * @return      Base64RtnType_OK if encoding works correctly.
 */
static Base64RtnType_t _convert3BytesTo4Base64Digits(uint8_t *bytes, uint8_t *dest)
{
    #ifdef BASE64_CHECKPARAM
    if(!bytes || !dest) return Base64RtnType_ERR;
    #endif

    /* Start the conversion. */
    *dest = _Base64DigitIndexTable[(*bytes & 0xFC) >> 2];
    *(dest + 1) = _Base64DigitIndexTable[((*bytes & 0x03) << 4) | ((*(bytes + 1) & 0xF0) >> 4)];
    *(dest + 2) = _Base64DigitIndexTable[((*(bytes + 1) & 0x0F) << 2) | ((*(bytes + 2) & 0xC0) >> 6)];
    *(dest + 3) = _Base64DigitIndexTable[*(bytes + 2) & 0x3F];
    return Base64RtnType_OK;
}

/**
 * Continuously convert 3 original bytes to 4 digits by using Base64 encoding.
 * @param bytes The bytes that needs to be encoded.
 * @param num   The number of conversions.
 * @param dest  The memory used to hold the result of our encoded Base64 digit string.
 * @return      Base64RtnType_OK if encoding works correctly.
 */
static Base64RtnType_t _convert3BytesTo4Base64DigitsContinuously(uint8_t *bytes, uint32_t num, uint8_t *dest)
{
    #ifdef BASE64_CHECKPARAM
    if(!bytes || !dest || !num) return Base64RtnType_ERR;
    #endif

    /* Pre-offseting. */
    bytes -= 3;
    dest -= 4;

    /* Start the conversion continuously. */
    do
    {
        bytes += 3;
        dest += 4;
        *dest = _Base64DigitIndexTable[(*bytes & 0xFC) >> 2];
        *(dest + 1) = _Base64DigitIndexTable[((*bytes & 0x03) << 4) | ((*(bytes + 1) & 0xF0) >> 4)];
        *(dest + 2) = _Base64DigitIndexTable[((*(bytes + 1) & 0x0F) << 2) | ((*(bytes + 2) & 0xC0) >> 6)];
        *(dest + 3) = _Base64DigitIndexTable[*(bytes + 2) & 0x3F];
    } while(--num);
    return Base64RtnType_OK;
}

/**
 * Convert byte(s) to 4 digits by using Base64 encoding(need to pad the vacant byte(s) with BASE64_PADDINGCHAR).
 * @param bytes The byte(s) that needs to be encoded.
 * @param num   The number of conversions.
 * @param dest  The memory used to hold the result of our encoded Base64 digit string.
 * @return      Base64RtnType_OK if encoding works correctly.
 */
static Base64RtnType_t _convertByteToBase64DigitsWithPadding(uint8_t *bytes, u_int32_t num, uint8_t *dest)
{
    #ifdef BASE64_CHECKPARAM
    if(!bytes || !dest || num < 1 || num > 2) return Base64RtnType_ERR;
    #endif

    /* Start the conversion. */
    *dest = _Base64DigitIndexTable[(*bytes & 0xFC) >> 2];
    if(num == 1)
    {
        *(dest + 1) = _Base64DigitIndexTable[(*bytes & 0x03) << 4];
        *(dest + 2) = (uint8_t)BASE64_PADDINGCHAR;
    } else {
        *(dest + 1) = _Base64DigitIndexTable[((*bytes & 0x03) << 4) | ((*(bytes + 1) & 0xF0) >> 4)];
        *(dest + 2) = _Base64DigitIndexTable[(*(bytes + 1) & 0x0F) << 2];
    }
    *(dest + 3) = (uint8_t)BASE64_PADDINGCHAR;
    return Base64RtnType_OK;
}

Base64RtnType_t Base64_encodeBytes(uint8_t *bytes, uint32_t size, uint8_t *dest)
{
    #ifdef BASE64_CHECKPARAM
    if(!bytes || !dest || size < 1) return Base64RtnType_ERR;
    #endif
    uint32_t quotient = size / 3;
    uint32_t remainder = size % 3;
    if(!remainder)
    {
        _convert3BytesTo4Base64DigitsContinuously(bytes, quotient, dest);
    } else {
        _convert3BytesTo4Base64DigitsContinuously(bytes, quotient, dest);
        _convertByteToBase64DigitsWithPadding(bytes + 3 * quotient, remainder + 4 * quotient, dest);
    }
    return Base64RtnType_OK;
}

/**
 * Convert 4 digits to 3 bytes by using Base64 encoding(only once).
 * @param text  The digit string that needs to be decoded.
 * @param dest  The memory used to hold the result of our decoded bytes.
 * @return      Base64RtnType_OK if decoding works correctly.
 */
static Base64RtnType_t _convert4Base64DigitsTo3Bytes(uint8_t *text, uint8_t *dest)
{
    #ifdef BASE64_CHECKPARAM
    if(!text || !dest) return Base64RtnType_ERR;
    #endif

    /* Text conversion buffer. */
    uint8_t textCnvBuf[4];
    uint32_t i = 0;
    if(isDigitVaild(*text) && isDigitVaild(*(text + 1)) && isDigitVaild(*(text + 2)) && isDigitVaild(*(text + 3)))
    {
        while(i <= 3)
        {
            textCnvBuf[i] = cvtBase64DigitToByte((*(text + i)));
            i++;
        }
        *dest = (textCnvBuf[0] << 2) | ((textCnvBuf[1] & 0x30) >> 4);
        *(dest + 1) = ((textCnvBuf[1] & 0x0F) << 4) | ((textCnvBuf[2] & 0x3C) >> 2);
        *(dest + 2) = ((textCnvBuf[2] & 0x03) << 6) | (textCnvBuf[3] & 0x3F);
        return Base64RtnType_OK;
    } else {
        return Base64RtnType_ERR;
    }
}

/**
 * Continuously convert 4 digits to 3 bytes by using Base64 encoding.
 * @param text  The digit string that needs to be decoded.
 * @param num   The number of conversions.
 * @param dest  The memory used to hold the result of our decoded bytes.
 * @return      Base64RtnType_OK if decoding works correctly.
 */
static Base64RtnType_t _convert4Base64DigitsTo3BytesContinuously(uint8_t *text, uint32_t num, uint8_t *dest)
{
    #ifdef BASE64_CHECKPARAM
    if(!text || !dest || !num) return Base64RtnType_ERR;
    #endif

    /* Text conversion buffer. */
    uint8_t textCnvBuf[4];
    uint32_t i;

    while(num--)
    {   
        i = 0;

        /* Determine whether each digit is a valid Base64 digit. */
        if(isDigitVaild(*text) && isDigitVaild(*(text + 1)) && isDigitVaild(*(text + 2)) && isDigitVaild(*(text + 3)))
        {
            while(i <= 3)
            {
                textCnvBuf[i] = cvtBase64DigitToByte((*(text + i)));
                i++;
            }
            /* Start the conversion. */
            *dest = (textCnvBuf[0] << 2) | ((textCnvBuf[1] & 0x30) >> 4);
            *(dest + 1) = ((textCnvBuf[1] & 0x0F) << 4) | ((textCnvBuf[2] & 0x3C) >> 2);
            *(dest + 2) = ((textCnvBuf[2] & 0x03) << 6) | (textCnvBuf[3] & 0x3F);
        } else {
            return Base64RtnType_ERR;
        }
        text += 4;
        dest += 3;
    }
    return Base64RtnType_OK;
}

/**
 * Convert 4 digits(with padding) to byte(s) by using Base64 encoding.
 * @param text  The digit string that needs to be decoded.
 * @param dest  The memory used to hold the result of our decoded bytes.
 * @return      Base64RtnType_OK if decoding works correctly.
 */
static Base64RtnType_t _convert4Base64DigitsToByteWithPadding(uint8_t *text, uint8_t *dest)
{
    #ifdef BASE64_CHECKPARAM
    if(!text || !dest) return Base64RtnType_ERR;
    #endif

    /* Text conversion buffer. */
    uint8_t textCnvBuf[4];
    uint32_t i = 0;
    if(isDigitVaild(*text) && isDigitVaild(*(text + 1)) && isDigitVaild(*(text + 2)) && isDigitVaild(*(text + 3)))
    {
        while(i <= 3)
        {
            textCnvBuf[i] = cvtBase64DigitToByte((*(text + i)));
            i++;
        }
        *dest = (textCnvBuf[0] << 2) | ((textCnvBuf[1] & 0x30) >> 4);
        if((char)*(text + 2) == BASE64_PADDINGCHAR && (char)*(text + 3) == BASE64_PADDINGCHAR)
        {
            return Base64RtnType_OK;
        } else {
            *(dest + 1) = ((textCnvBuf[1] & 0x0F) << 4) | ((textCnvBuf[2] & 0x3C) >> 2);
            return Base64RtnType_OK;
        }
    } else {
        return Base64RtnType_ERR;
    }
}

Base64RtnType_t Base64_decodeText(uint8_t *text, uint32_t size, uint8_t *dest)
{
    #ifdef BASE64_CHECKPARAM
    if(!text || !dest || size < 1) return Base64RtnType_ERR;
    #endif
    uint32_t _quotient = size / 4 - 1;
    if(*(text + size - 1) == BASE64_PADDINGCHAR)
    {
        _convert4Base64DigitsTo3BytesContinuously(text, _quotient, dest);
        _convert4Base64DigitsToByteWithPadding(text + 4 * _quotient, dest + 3 * _quotient);
    } else {
        _convert4Base64DigitsTo3BytesContinuously(text, _quotient + 1, dest);
    }
    return Base64RtnType_OK;
}

uint32_t Base64_getDecodedSize(uint8_t *text, uint32_t size)
{
    uint32_t _size = (size / 4 - 1) * 3;
    text += size - 1;
    if((char)*text == BASE64_PADDINGCHAR)
    {
        if((char)*(text - 1) == BASE64_PADDINGCHAR)
        {
            return _size + 1;
        } else {
            return _size + 2;
        }
    } else {
        return _size + 3;
    }
}

#ifdef BASE64_FUNCTESTENABLE

Base64RtnType_t _convert4Base64DigitsTo3BytesContinuously_test(uint8_t *text, uint32_t num, uint8_t *dest)
{
    return _convert4Base64DigitsTo3BytesContinuously(text, num, dest);
}

Base64RtnType_t _convert4Base64DigitsToByteWithPadding_test(uint8_t *text, uint8_t *dest)
{
    return _convert4Base64DigitsToByteWithPadding(text, dest);
}

#endif
