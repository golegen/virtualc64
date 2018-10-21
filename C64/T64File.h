/*!
 * @header      T64File.h
 * @author      Dirk W. Hoffmann, www.dirkwhoffmann.de
 * @copyright   Dirk W. Hoffmann, all rights reserved.
 */
/* This program is free software; you can redistribute it and/or modify
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

#ifndef _T64FILE_INC
#define _T64FILE_INC

#include "AnyArchive.h"

/*! @class   T64File
 *  @brief   The T64File class declares the programmatic interface for a file
 *           in T64 format.
 */
class T64File : public AnyArchive {
    
    //! @brief    Header signature
    static const uint8_t magicBytes[];
    
    /*! @brief    Number of the currently selected item
     *  @details  -1, if no item is selected
     */
    long selectedItem  = -1;
    
public:
    
    //
    //! @functiongroup Creating and deleting objects
    //
    
    //! @brief    Standard constructor
    T64File();
    
    //! @brief    Factory method
    static T64File *makeT64ArchiveWithBuffer(const uint8_t *buffer, size_t length);
    
    //! @brief    Factory method
    static T64File *makeT64ArchiveWithFile(const char *path);
    
    /*! @brief    Factory method
     *  @details  otherArchive can be of any archive type
     */
    static T64File *makeT64ArchiveWithAnyArchive(AnyArchive *otherArchive);

    
    //
    // Methods from AnyC64File
    //
    
    C64FileType type() { return T64_FILE; }
    const char *typeAsString() { return "T64"; }
    const char *getName();
    const unsigned short *getUnicodeName();
    
    //
    // Methods from AnyArchive
    //
    
    int getNumberOfItems();
    const char *getNameOfItem(unsigned item);
    const char *getTypeOfItem(unsigned item);
    uint16_t getDestinationAddrOfItem(unsigned item);
    uint16_t getDestinationAddr();
    void selectItem(unsigned item);
    
    //
    //! @functiongroup Serializing
    //
    
    //! @brief    Returns true iff buffer contains a T64 file
    static bool isT64(const uint8_t *buffer, size_t length);
    
    //! Returns true of filename points to a valid file of that type
    static bool isT64File(const char *filename);
    
    //! Check file type
    bool hasSameType(const char *filename) { return isT64File(filename); }
    
    //! Read container data from memory buffer
    bool readFromBuffer(const uint8_t *buffer, size_t length);

    //! Write container data to memory buffer
    size_t writeToBuffer(uint8_t *buffer);
    
    
    //
    // @functiongroup Scanning and repairing an archive
    //
    
    //! @brief Check if the file header contains information at the specific location
    bool directoryItemIsPresent(int n);

    /*! @brief    Check archive consistency and repair inconsistent information
     *  @details  This method can eliminate the following inconsistencies:
     *            number of files: some archives state falsely in their header that zero
     *            files are present. This value will be fixed.
     *            end loading address: Archives that are created with CONVC64 often contain
     *            a value of 0xC3C6, which is wrong (e.g., paradrd.t64). This value will be
     *            changed such that getByte() will read until the end of the physical file.
     * @result    true, if archive was consistent or could be repaired. false, if an inconsistency
     *            has been detected that could not be repaired.
     */
    bool repair();
    
};

#endif

