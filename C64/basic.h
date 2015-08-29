/*
 * (C) 2006 Dirk W. Hoffmann. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _BASIC_INC
#define _BASIC_INC

// General Includes
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>
#include <assert.h>
#include <math.h>
#include <ctype.h> 

// C++ includes
#include <string>

//
//! @functiongroup Handling low level data objects
//

//! Evaluates to the high byte of x. x is expected to be of type uint16_t.
#define HI_BYTE(x) (uint8_t)((x) >> 8)

//! Evaluates to the low byte of x. x is expected to be of type uint16_t.
#define LO_BYTE(x) (uint8_t)((x) & 0xff)

//! Evaluates to the 16 bit value specified by x and y in little endian order (low, high).
#define LO_HI(x,y) (uint16_t)((y) << 8 | (x))

//! Evaluates to the 32 bit value specified by x and y in little endian order (lowest, low, high, highest).
#define LO_LO_HI_HI(x,y,z,w) (uint32_t)((w) << 24 | (z) << 16 | (y) << 8 | (x))

//! Evaluates to the 16 bit value specified by x and y in big endian order (high, low).
#define HI_LO(x,y) (uint16_t)((x) << 8 | (y))

//! Evaluates to the 32 bit value specified by x and y in big endian order (highest, high, low, lowest).
#define HI_HI_LO_LO(x,y,z,w) (uint32_t)((x) << 24 | (y) << 16 | (z) << 8 | (w))

//! Returns true iff bit n is set in x.
#define GET_BIT(x,nr) ((x) & (1 << (nr)))

//! Set a single bit.
#define SET_BIT(x,nr) ((x) |= (1 << (nr)))

//! Clear a single bit.
#define CLR_BIT(x,nr) ((x) &= ~(1 << (nr)))

//! Toggle a single bit.
#define TOGGLE_BIT(x,nr) ((x) ^= (1 << (nr)))


//
//! @functiongroup Pretty printing
//

void printReadable(const void *data, int length);

//
//! @functiongroup Converting low level data objects
//


/*! @brief Converts a PET character to a unicocde character.
 *  @discussion This function uses the PET upper case character set.
 *  @result Returns 0x00 if no unicode counterpart exists. */
uint16_t pet2unicode(uint8_t petchar);

/*! @brief Converts a PET character to an ASCII character.
 *  @discussion This function uses the PET upper case character set.
 *  @result Returns '.' if no ASCII counterpart exists .
 *  @deprecated Use pet2ascii instead. */
char toASCII(char c);

/*! @brief Converts a PET character to an ASCII character.
 *  @discussion This function uses the PET upper case character set.
 *  @result Returns '.' if no ASCII counterpart exists. */
uint8_t pet2ascii(uint8_t petchar);

/*! @brief Converts an PET string into a ASCII string. */
void pet2ascii(char *petstring);

/*! @brief Converts an ASCII character to a PET character.
 *  @discussion This function translates into the unshifted PET character set. I.e., lower case characters are converted to uppercase characters.
 *  @result Returns ' ' if the ASCII character is not covered. */
uint8_t ascii2pet(uint8_t asciichar);

/*! @brief Converts an ASCII string into a PET string. */
void ascii2pet(char *asciistring);


//! Write ASCII representation of 8 bit value to a string
void binary8_to_string(uint8_t value, char *s);

//! Write ASCII representation of 32 bit value to a string
void binary32_to_string(uint32_t value, char *s);

//! Convert a BCD number to a binary value
inline uint8_t BCDToBinary(uint8_t value) { return (10 * (value >> 4)) + (value & 0x0F); }

//! Convert a binary value to a BCD number
inline uint8_t BinaryToBCD(uint8_t value) { return ((value / 10) << 4) + (value % 10); }

//! Increment BCD number by one
inline uint8_t incBCD(uint8_t value) { return ((value & 0x0F) == 0x09) ?  (value & 0xF0) + 0x10 : (value & 0xF0) + ((value + 0x01) & 0x0F); }

//
//! Handling file and path names
//

//! Extract directory from path
inline std::string ExtractDirectory( const std::string& path )
{
	return path.substr(0, path.find_last_of( '/' ) + 1);
}

//! Extract filename from path
inline std::string ExtractFilename( const std::string& path )
{
	return path.substr( path.find_last_of( '/' ) +1 );
}

//! Change extension
inline std::string ChangeExtension( const std::string& path, const std::string& ext )
{
	std::string filename = ExtractFilename(path);
	return ExtractDirectory(path) + filename.substr(0, filename.find_last_of( '.' )) + ext;
}


//! Check file suffix
/*! The function is used for determining the type of a file.
	\param filename Path and name of the file to investigate
	\param suffix Expected suffix
*/
bool checkFileSuffix(const char *filename, const char *suffix);

//! Check file size
/*! The function is used for validating the size of a file.
	\param filename Path and name of the file to investigate
	\param min Expected minimum size (-1 if no lower bound exists)
	\param max Expected maximum size (-1 if no upper bound exists)	
*/
bool checkFileSize(const char *filename, int min, int max);

//! Check magic bytes of a file.
/*! The function is used for determining the type of a file.
	\param filename Path and name of the file to investigate
	\param header Expected byte sequence, terminated by EOF
	\return Returns true iff magic bytes match
	\see Memory::isBasicRom Memory::isKernelRom Memory::isCharRom
*/
bool 
checkFileHeader(const char *filename, int *header);

//
//! @functiongroup Managing time
//

//! Application launch time in seconds
/*! The value is read by function \a msec for computing the elapsed number of microseconds. */
extern long tv_base;

//! Return the number of elapsed microseconds since program launch
uint64_t usec();

//! Reads the real-time clock (1/10th seconds)
uint8_t localTimeSecFrac();

//! Reads the real-time clock (seconds)
uint8_t localTimeSec();

//! Reads the real-time clock (minutes)
uint8_t localTimeMin();

//! Reads the real-time clock (hours)
uint8_t localTimeHour();

//! Sleep for some microseconds
//  DEPRECATED. USE SleepUntil INSTEAD
void sleepMicrosec(uint64_t usec);

/*! @brief  Sleeps until kernel timer reaches kernelTargetTime
 *  @param  kernelEarlyWakeup To increase timing precision, the function wakes up the thread earlier
 *          by this amount and waits actively in a delay loop until the deadline is reached.
 *  @result Overshoot time (jitter), measured in kernel time. Smaller values are better, 0 is best. 
 */
int64_t sleepUntil(uint64_t kernelTargetTime, uint64_t kernelEarlyWakeup = 0);


//
//
//! @functiongroup Debugging
//

#define RANGE(val,min,max) ((val) >= (min) &&  (val) <= (max))

#endif

