#ifndef __BASE64_H__
#define __BASE64_H__

#include "sys/types.h"
#include "common.h"

/* If BASE64_CHECKPARAM is defined, some functions will check their parameter if it's vaild. */
#define BASE64_CHECKPARAM

/* The character used to pad the vacant byte(s). */
#define BASE64_PADDINGCHAR  '='

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
 * Encode a bunch of bytes by using Base64.
 * Decode the text in Base64 encoding.
 * @param bytes The text that needs to be decoded.
 * @param size  The size of the text.
 * @param dest  The memory used to hold the result of our decoding.
 * @return      Base64RtnType_OK if decoding works correctly.
 */
Base64RtnType_t Base64_decodeText(uint8_t text, uint32_t size, uint8_t *dest);

Base64RtnType_t Base64_funcTest(uint8_t *bytes, uint32_t num, uint8_t *dest);

#endif