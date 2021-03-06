/*
 * This file is part of Chocobun.
 *
 * Chocobun is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chocobun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Chocobun.  If not, see <http://www.gnu.org/licenses/>.
 */

// --------------------------------------------------------------
// Level
// --------------------------------------------------------------

#ifndef __CHOCOBUN_CORE_LEVEL_HPP__
#define __CHOCOBUN_CORE_LEVEL_HPP__

// --------------------------------------------------------------
// include files

#include <core/Config.hpp>

#include <string>
#include <vector>
#include <map>

#include <iostream>

namespace Chocobun {

/*!
 * @brief Holds information of a loaded level
 */
class Level
{
public:

    static const std::string validTiles;

    typedef std::map<std::string, Level*>::iterator metaDataIterator;

    /*!
     * @brief Default Constructor
     */
    Level( void );

    /*!
     * @brief Destructor
     */
    ~Level( void );

    /*!
     * @brief Adds meta data to the level
     *
     * Meta data is very loosely defined in the file format specifications,
     * therefore this method allows any key-value pair to be registered.
     *
     * Supported internal keys are
     * -Collection
     * -Author
     *
     * @note Keys are case sensitive and should be converted to lower case
     * before adding the meta data.
     *
     * @exception Chocobun::Exception If the key already exists
     *
     * @param key The key of the entry (used to get the data back later on)
     * @param value The value of the entry (can by any text string)
     */
    void addMetaData( const std::string& key, const std::string& value );

    /*!
     * @brief Retrieves meta data of the level
     *
     * @exception Chocobun::Exception if the key was not found
     *
     * @param key The key of the entry to search for
     * @return The value tied to the key
     */
    const std::string& getMetaData( const std::string& key );

    /*!
     * @brief Formats and streams all meta data to a stream object
     *
     * This is used when saving the level's meta data
     *
     * @param stream The stream object to stream to
     */
    void streamAllMetaData( std::ostream& stream );

    /*!
     * @brief Adds Header data and other text for this level
     *
     * This is used later on when the file is saved to disk again,
     * so headers that would not normally be loaded are preserved
     *
     * @param header Header to add
     */
    void addHeaderData( const std::string& header );

    /*!
     * @brief Removes header data from this level
     *
     * This is called when a level name is discovered. Because
     * there can be multiple passes of delay before a level name can
     * be confirmed, it usually occurs that it has been added as a
     * Header first. This will remove it again so it isn't exported twice.
     *
     * @param header The Header string to remove
     */
    void removeHeaderData( const std::string& header );

    /*!
     * @brief Streams all header data to a stream object
     *
     * This is used to save the header data of a level
     *
     * @param stream The stream object to stream to
     */
    void streamAllHeaderData( std::ostream& stream );

    /*!
     * @brief Inserts a tile into the level at the given coordinate
     *
     * Internally the map array is resized accordingly so it remains square.
     * <b>Valid tile characters are:</b>
     * - # Wall
     * - @ Pusher
     * - + Pusher on goal square
     * - $ Box
     * - * Box on goal square
     * - . Goal square
     * -   Floor (space)
     * - _ Floor
     *
     * @exception Chocobun::Exception if an invalid character is passed
     *
     * @param x The X coordinate for the tile to insert
     * @param y The Y coordinate for the tile to insert
     * @param tile The type of tile
     */
    void insertTile( const Chocobun::Uint32& x, const Chocobun::Uint32& y, const char& tile );

    /*!
     * @brief Inserts a whole line instead of a single tile
     *
     * Internally the map array is resized accordingly so it remains square.
     * <b>Valid tile characters are:</b>
     * - # Wall
     * - @ Pusher
     * - + Pusher on goal square
     * - $ Box
     * - * Box on goal square
     * - . Goal square
     * -   Floor (space)
     * - _ Floor
     *
     * @exception Chocobun::Exception if an invalid character is passed
     *
     * @param y The Y coordinate for the tile line to insert
     * @param tiles The line of tiles to insert
     */
    void insertTileLine( const Chocobun::Uint32& y, const std::string& tiles );

    /*!
     * @brief Streams all tile data to a stream object
     *
     * This is used to save the level data to a file
     *
     * @param stream The stream object to stream to
     * @param newLine If set to true (default), new lines are inserted.
     * Otherwise, "|" are inserted (for RLE compression)
     */
    void streamAllTileData( std::ostream& stream, bool newLine = true );

    /*!
     * @brief Gets the array of tile data
     *
     * @return Returns a 2-dimensional array of chars containing tile data
     */
    const std::vector< std::vector<char> >& getTileData( void ) const;

    /*!
     * @brief Gets a single tile from the level
     *
     * Will retrieve the tile data specified by the x and y parameters
     *
     * @param x The X-coordinate of the tile
     * @param y The Y-coordinate of the tile
     * @return Returns the tile at the specified coordinates. If the tile couldn't
     * be retrieved, a null character ('\0') is returned.
     */
    char getTile( Uint32 x, Uint32 y ) const;

    /*!
     * @brief Returns the X-size of the level
     *
     * @return The X-size of the level
     */
    Uint32 getSizeX( void ) const;

    /*!
     * @brief Returns the Y-size of the level
     *
     * @return The Y-size of the level
     */
    Uint32 getSizeY( void ) const;

    /*!
     * @brief Adds level notes to this level
     *
     * @param note The notes string to add
     */
    void addLevelNote( const std::string& note);

    /*!
     * @brief Removes level notes from this level
     *
     * @param note The note to search for and remove
     */
    void removeLevelNote( const std::string& note );

    /*!
     * @brief Streams all notes to a stream object
     *
     * This is used to save the level data to a file
     *
     * @param stream The stream object to stream to
     */
    void streamAllNotes( std::ostream& stream );

    /*!
     * @brief Sets the name of the level
     */
    void setLevelName( const std::string& name );

    /*!
     * @brief Gets the name of the level
     */
    std::string getLevelName( void ) const;

    /*!
     * @brief Validates the level
     *
     * Will perform various checks to see if the level is valid. This includes:
     * - Only one player can exist on a level
     * - All boxes can be reached by the player
     * - All boxes which can't be reached by the player are placed on goal squares
     * - The level is closed off entirely by a wall
     *
     * @note It is essential to call this method before using the level for game play.
     * This method also 'finalises' the level by performing some internal setup on the
     * provided tile data.
     *
     * @return If any of these fail, false is returned. If the level is considered valid, true is returned.
     */
    bool validateLevel( void );

    /*!
     * @brief Moves the player up by one tile
     * @note If the move is not possible, this method will silently fail
     */
    void moveUp( void );

    /*!
     * @brief Moves the player down by one tile
     * @note If the move is not possible, this method will silently fail
     */
    void moveDown( void );

    /*!
     * @brief Moves the player left by one tile
     * @note If the move is not possible, this method will silently fail
     */
    void moveLeft( void );

    /*!
     * @brief Moves the player right by one tile
     * @note If the move is not possible, this method will silently fail
     */
    void moveRight( void );

    /*!
     * @brief Undoes the last move
     * @note If no undo data exists, this method will silently fail
     */
    void undo( void );

    /*!
     * @brief Redoes a move
     * @note If no redo data exists, this method will silently fail
     */
    void redo( void );

private:

    /*!
     * @brief Moves the player and updates all tiles
     *
     * @param newX The new x position the player should have
     * @param newY The new y position the player should have
     * @return Returns true if the move was successful, false if otherwise
     */
    bool movePlayer( char direction );

    std::map<std::string, std::string> m_MetaData;
    std::vector< std::vector<char> > m_LevelArray;
    std::vector<std::string> m_HeaderData;
    std::vector<std::string> m_Notes;
    std::vector<char> m_UndoData;
    std::string m_LevelName;

    Uint32 m_PlayerX;
    Uint32 m_PlayerY;
    Uint32 m_UndoDataIndex;

    bool m_IsLevelValid;
};

} // namespace Chocobun

#endif // __CHOCOBUN_CORE_LEVEL_HPP__
