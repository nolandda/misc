
#pragma once

#ifndef __XPLATFORM_ENDIAN_H__
#define __XPLATFORM_ENDIAN_H__ 1

#include <stdint.h>
#include <stddef.h>

/* These functions will be slower than some macros, but they should be
   agressively cross-platform compatable. The functions do not assume
   the endianness or representation of the integer. All work is done
   algebraicaly and so should be correct even on a middle endian
   machine that does not use a twos compliment representation. */

/* These funcs serialize integer types to big-endian buffers. They 
   return the number of bytes written to the buffer which is caller
   allocated. */
size_t u8_to_be_buf(uint8_t* buf, uint8_t num);
size_t i8_to_be_buf(uint8_t* buf, int8_t num);

size_t u16_to_be_buf(uint8_t* buf, uint16_t num);
size_t i16_to_be_buf(uint8_t* buf, int16_t num);

size_t u32_to_be_buf(uint8_t* buf, uint32_t num);
size_t i32_to_be_buf(uint8_t* buf, int32_t num);

size_t u64_to_be_buf(uint8_t* buf, uint64_t num);
size_t i64_to_be_buf(uint8_t* buf, int64_t num);

/* Deserialize from a big-endian buffer */
uint8_t be_buf_to_u8(const uint8_t* buf, size_t len);
int8_t be_buf_to_i8(const uint8_t* buf, size_t len);

uint16_t be_buf_to_u16(const uint8_t* buf, size_t len);
int16_t be_buf_to_i16(const uint8_t* buf, size_t len);

uint32_t be_buf_to_u32(const uint8_t* buf, size_t len);
int32_t be_buf_to_i32(const uint8_t* buf, size_t len);

uint64_t be_buf_to_u64(const uint8_t* buf, size_t len);
int64_t be_buf_to_i64(const uint8_t* buf, size_t len);



#endif /* __XPLATFORM_ENDIAN_H__ */
