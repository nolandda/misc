
#ifndef __SERIALIZE_FLOAT_H__
#define __SERIALIZE_FLOAT_H__ 1

#include <stdint.h>

/** 
 * @brief Break a IEEE 754 binary float apart into integer 
 *        representations of its mantissa and exponent.
 *         
 * @param mantissa 32-bit integer representation of mantissa returned 
 *                 by reference
 * @param exponent 8-bit integer representation of exponent returned 
 *                 by reference
 * @param number the float to be broken apart.
 */
void FlattenFloat (int32_t* mantissa, int8_t* exponent, float number);

/** 
 * @brief Recombine integer components (mantissa + exponent back 
 *        into an IEEE 754 binary float. 
 *         
 * @param number the location to recombine the float. Returned by
 *        reference. May be NULL. 
 * @param mantissa 32-bit integer representation of mantissa.
 * @param exponent 8-bit integer representation of exponent.
 * @return the recombined float. Equivelent to *number if number 
 *         was non-null. 
 */
float UnflattenFloat (float* number, int32_t mantissa, int8_t exponent);

/** 
 * @brief Convert an IEEE 754 binary float into an architecture
 *        independant byte buffer.
 *         
 * @param buf The buffer that will contain the serialized
 *        result. Allocation and ownership of this buffer are the
 *        caller's responsibility.
 * @param number the float to be serialized.
 * @return the number of bytes used to store the serialized number
 *         should always be == 5 i.e. sizeof(int32)+sizeof(int8_t)
 */
size_t SerializeFloat(uint8_t* buf, float number);

/** 
 * @brief Convert an architecture independant byte buffer created by
 *        SerializeFloat above back into an IEEE 754 binary float. 
 *         
 * @param buf The buffer containing the serialized float.
 * @param len The length of the input buffer. Should be >= 5. All
 *	  bytes beyond what are needed for deserialization are ignored.
 * @return the restored float.
 */
float DeserializeFloat(const uint8_t* buf, size_t len);


/** 
 * @brief Break a IEEE 754 binary double apart into integer 
 *        representations of its mantissa and exponent.
 *         
 * @param mantissa 64-bit integer representation of mantissa returned 
 *                 by reference
 * @param exponent 16-bit integer representation of exponent returned 
 *                 by reference
 * @param number the double to be broken apart.
 */
void FlattenDouble (int64_t* mantissa, int16_t* exponent, double number);

/** 
 * @brief Recombine integer components (mantissa + exponent back 
 *        into an IEEE 754 binary double. 
 *         
 * @param number the location to recombine the double. Returned by
 *        reference. May be NULL. 
 * @param mantissa 64-bit integer representation of mantissa.
 * @param exponent 16-bit integer representation of exponent.
 * @return the recombined double. Equivelent to *number if number 
 *         was non-null. 
 */
double UnflattenDouble (double* number, int64_t mantissa, int16_t exponent);

/** 
 * @brief Convert an IEEE 754 binary double into an architecture
 *        independant byte buffer.
 *         
 * @param buf The buffer that will contain the serialized
 *        result. Allocation and ownership of this buffer are the
 *        caller's responsibility.
 * @param number the double to be serialized.
 * @return the number of bytes used to store the serialized number
 *         should always be == 10 i.e. sizeof(int64_t)+sizeof(int16_t)
 */
size_t SerializeDouble(uint8_t* buf, double number);

/** 
 * @brief Convert an architecture independant byte buffer created by
 *        SerializeDouble above back into an IEEE 754 binary double. 
 *         
 * @param buf The buffer containing the serialized double.
 * @param len The length of the input buffer. Should be >= 10. All
 *	  bytes beyond what are needed for deserialization are ignored.
 * @return the restored double.
 */
double DeserializeDouble(const uint8_t* buf, size_t len);


#endif // __SERIALIZE_FLOAT_H__
