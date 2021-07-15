#ifndef __BASE64_H__
#define __BASE64_H__

#include "sys/types.h"
#include "common.h"

/* Enable function for function testing. */
#define BASE64_FUNCTESTENABLE

/* If BASE64_CHECKPARAM is defined, some functions will check their parameter if it's vaild. */
#define BASE64_CHECKPARAM

/* The character used to pad the vacant byte(s). */
#define BASE64_PADDINGCHAR  '='

/* Is the digit a vaild Base64 digit. */
#define isDigitVaild(digit) ('A'<=digit&&digit<='Z'||'a'<=digit&&digit<='z'||'0'<=digit&&digit<='9'||digit=='+'||digit=='/'||digit=='=')

/* Convert Base64 digit to byte. */
#define cvtBase64DigitToByte(digit) ((uint8_t)(('A'<=digit&&digit<='Z')?digit-'A':(('a'<=digit&&digit<='z')?digit-'a'+26:(('0'<=digit&&digit<='9')?digit-'0'+52:(digit=='+'?62:63)))))

/* General purpose retuen type used by almost all functions. */
typedef enum{
    Base64RtnType_OK,
    Base64RtnType_ERR
}Base64RtnType_t;

/**
 * Encode bytes with specified size to Base64 digit string.
 * @param bytes The byte(s) that needs to be encoded.
 * @param size  The size of the bytes.
 * @param dest  The memory used to hold the result of our encoded Base64 digit string.
 * @return      Base64RtnType_OK if encoding works correctly.
 */
Base64RtnType_t Base64_encodeBytes(uint8_t *bytes, uint32_t size, uint8_t *dest);

/**
 * Decode the text in Base64 encoding.
 * @param text  The text that needs to be decoded.
 * @param size  The size of the text.
 * @param dest  The memory used to hold the result of our decoding.
 * @return      Base64RtnType_OK if decoding works correctly.
 */
Base64RtnType_t Base64_decodeText(uint8_t *text, uint32_t size, uint8_t *dest);

/**
 * Get decoded byte size of the Base64 digit string. 
 * @param text  The text that needs to be decoded.
 * @param size  The size of the text.
 * @return      Decoded byte size.
 */
uint32_t Base64_getDecodedSize(uint8_t *text, uint32_t size);

#ifdef BASE64_FUNCTESTENABLE

Base64RtnType_t _convert4Base64DigitsTo3BytesContinuously_test(uint8_t *text, uint32_t num, uint8_t *dest);
Base64RtnType_t _convert4Base64DigitsToByteWithPadding_test(uint8_t *text, uint8_t *dest);

#endif

#endif