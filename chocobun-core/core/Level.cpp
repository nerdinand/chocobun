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

// --------------------------------------------------------------
// include files

#include <core/Level.hpp>
#include <core/Exception.hpp>

const std::string Chocobun::Level::validTiles = "#@+$*. _pPbB";

namespace Chocobun {

// --------------------------------------------------------------
Level::Level( void ) :
    m_IsLevelValid( false ),
    m_UndoDataIndex( -1 ) // type is unsigned, but the wrap around is intended
{
    m_LevelArray.push_back( std::vector<char>(0) );
}

// --------------------------------------------------------------
Level::~Level( void )
{
}

// --------------------------------------------------------------
void Level::addMetaData( const std::string& key, const std::string& value )
{
    if( m_MetaData.find( key ) != m_MetaData.end() )
        throw Exception( "[Level::addMetaData] meta data already exists" );
    m_MetaData[key] = value;
}

// --------------------------------------------------------------
const std::string& Level::getMetaData( const std::string& key )
{
    std::map<std::string, std::string>::iterator p = m_MetaData.find( key );
    if( p == m_MetaData.end() )
        throw Exception( "[Level::getMetaData] meta data not found" );
    return p->second;
}

// --------------------------------------------------------------
void Level::streamAllMetaData( std::ostream& stream )
{
    for( std::map<std::string, std::string>::iterator it = m_MetaData.begin(); it != m_MetaData.end(); ++it )
        stream << it->first << ": " << it->second << std::endl;
}

// --------------------------------------------------------------
void Level::addHeaderData( const std::string& header )
{
    m_HeaderData.push_back( header );
}

// --------------------------------------------------------------
void Level::removeHeaderData( const std::string& header )
{
    for( std::vector<std::string>::iterator it = m_HeaderData.begin(); it != m_HeaderData.end(); ++it )
    {
        if( it->compare( header ) == 0 )
        {
            m_HeaderData.erase( it );
            break;
        }
    }
}

// --------------------------------------------------------------
void Level::streamAllHeaderData( std::ostream& stream )
{
    for( std::vector<std::string>::iterator it = m_HeaderData.begin(); it != m_HeaderData.end(); ++it )
        stream << *it << std::endl;
}

// --------------------------------------------------------------
void Level::insertTile( const Chocobun::Uint32& x, const Chocobun::Uint32& y, const char& tile )
{

    // check if character is valid
    if( validTiles.find_first_of(tile) == std::string::npos )
        throw Exception( "[Level::insertTile] attempt to insert invalid character into level array" );

    // resize array if necessary
    while( x+1 > m_LevelArray.size() )
    {
        m_LevelArray.push_back( std::vector<char>(0) );
        m_LevelArray[m_LevelArray.size()-1].resize( m_LevelArray[0].size(), 32 );
    }
    if( y+1 > m_LevelArray[0].size() )
        for( size_t i = 0; i != m_LevelArray.size(); ++i )
            m_LevelArray[i].resize( y+1, 32 );

    // write tile
    m_LevelArray[x][y] = tile;

}

// --------------------------------------------------------------
void Level::insertTileLine( const Chocobun::Uint32& y, const std::string& tiles )
{
    for( size_t x = 0; x != tiles.size(); ++x )
        this->insertTile( x, y, tiles[x] );
}

// --------------------------------------------------------------
void Level::streamAllTileData( std::ostream& stream, bool newLine )
{
    for( size_t y = 0; y != m_LevelArray[0].size(); ++y )
    {
        for( size_t x = 0; x != m_LevelArray.size(); ++x )
            stream << m_LevelArray[x][y];
        if( newLine )
            stream << std::endl;
        else
            stream << "|";
    }
    if( !newLine ) stream << std::endl;
}

// --------------------------------------------------------------
const std::vector< std::vector<char> >& Level::getTileData( void ) const
{
    return m_LevelArray;
}

// --------------------------------------------------------------
char Level::getTile( Uint32 x, Uint32 y ) const
{
    if( x < 1 || x > m_LevelArray.size() ) return '\0';
    if( y < 1 || y > m_LevelArray[0].size() ) return '\0';
    return m_LevelArray[x-1][y-1];
}

// --------------------------------------------------------------
Uint32 Level::getSizeX( void ) const
{
    return m_LevelArray.size();
}

// --------------------------------------------------------------
Uint32 Level::getSizeY( void ) const
{
    return m_LevelArray[0].size();
}

// --------------------------------------------------------------
void Level::addLevelNote( const std::string& note )
{
    m_Notes.push_back( note );
}

// --------------------------------------------------------------
void Level::removeLevelNote( const std::string& note)
{
    for( std::vector<std::string>::iterator it = m_Notes.begin(); it != m_Notes.end(); ++it )
    {
        if( it->compare( note ) == 0 )
        {
            m_Notes.erase( it );
            break;
        }
    }
}

// --------------------------------------------------------------
void Level::streamAllNotes( std::ostream& stream )
{
    for( std::vector<std::string>::iterator it = m_Notes.begin(); it != m_Notes.end(); ++it )
        stream << *it << std::endl;
}

// --------------------------------------------------------------
void Level::setLevelName( const std::string& name )
{
    m_LevelName = name;
}

// --------------------------------------------------------------
std::string Level::getLevelName( void ) const
{
    return m_LevelName;
}

// --------------------------------------------------------------
bool Level::validateLevel( void )
{

    // does another check need to be done?
    if( m_IsLevelValid ) return true;

    // make sure there's only one player
    // this also sets the internal positions of the player
    bool playerFound = false;
    for( size_t x = 0; x != m_LevelArray.size(); ++x )
    {
        for( size_t y = 0; y != m_LevelArray[0].size(); ++y )
        {
            if( m_LevelArray[x][y] == '@' )
            {
                if( playerFound )
                {
                    return false;
                }else
                {
                    m_PlayerX = x;
                    m_PlayerY = y;
                    playerFound = true;
                }

            }
        }
    }

    // arriving here means the level is valid
    m_IsLevelValid = true;
    return true;
}

// --------------------------------------------------------------
void Level::moveUp( void )
{
    if( !m_IsLevelValid ) return;
    if( !this->movePlayer( 'u' ) ) return;
    --m_PlayerY;
}

// --------------------------------------------------------------
void Level::moveDown( void )
{
    if( !m_IsLevelValid ) return;
    if( !this->movePlayer( 'd' ) ) return;
    ++m_PlayerY;
}

// --------------------------------------------------------------
void Level::moveLeft( void )
{
    if( !m_IsLevelValid ) return;
    if( !this->movePlayer( 'l' ) ) return;
    --m_PlayerX;
}

// --------------------------------------------------------------
void Level::moveRight( void )
{
    if( !m_IsLevelValid ) return;
    if( !this->movePlayer( 'r' ) ) return;
    ++m_PlayerX;
}

// --------------------------------------------------------------
bool Level::movePlayer( char direction )
{

    bool isPushingBox = false;

    // calculate new positions of player
    Uint32 newX = m_PlayerX, newY = m_PlayerY;
    if( direction == 'u' ) --newY;
    if( direction == 'd' ) ++newY;
    if( direction == 'l' ) --newX;
    if( direction == 'r' ) ++newX;

    // calculate next step the player would take if traveling linearly
    Uint32 nextX = newX + (newX-m_PlayerX);
    Uint32 nextY = newY + (newY-m_PlayerY);

    // can't move up if there is a wall
    if( m_LevelArray[newX][newY] == '#' ) return false;

    // can't move up if box is against a wall or another box
    if( m_LevelArray[newX][newY] == '$' || m_LevelArray[newX][newY] == '*' )
    {
        if( m_LevelArray[nextX][nextY] == '#' ||
            m_LevelArray[nextX][nextY] == '$' ||
            m_LevelArray[nextX][nextY] == '*' )
            return false;
        isPushingBox = true;
    }

    // move box up (if any)
    if( isPushingBox )
    {
        if( m_LevelArray[newX][newY] == '$' )
            m_LevelArray[newX][newY] = ' ';   // box is on floor, expose floor
        else
            m_LevelArray[newX][newY] = '.';   // box is on goal, expose goal
        if( m_LevelArray[nextX][nextY] == '.' )
            m_LevelArray[nextX][nextY] = '*';   // target is goal, place box on goal
        else
            m_LevelArray[nextX][nextY] = '$';     // target is floor, place box on floor
    }

    // move player up
    if( m_LevelArray[newX][newY] == ' ' )
        m_LevelArray[newX][newY] = '@';     // target is floor, place player on floor
    else
        m_LevelArray[newX][newY] = '+';     // target is goal, place player on goal
    if( m_LevelArray[m_PlayerX][m_PlayerY] == '@')
        m_LevelArray[m_PlayerX][m_PlayerY] = ' ';
    else
        m_LevelArray[m_PlayerX][m_PlayerY] = '+';

    // generate undo data
    if( isPushingBox ) direction -= 32; // convert to upper case for pushing boxes
    if( m_UndoDataIndex != -1 )
        while( m_UndoData.size() != m_UndoDataIndex )
            m_UndoData.pop_back();
    m_UndoData.push_back( direction );
    ++m_UndoDataIndex;

    return true;
}

// --------------------------------------------------------------
void Level::undo( void )
{
    if( !m_IsLevelValid ) return;
    if( m_UndoDataIndex == -1 ) return;

    // get undo move
    char move = m_UndoData.at( m_UndoDataIndex );
    --m_UndoDataIndex;

    // determine if a box was pushed and convert to lower case
    bool boxPushed = false;
    if( move == 'U' || move == 'D' || move == 'L' || move == 'R' )
    {
        boxPushed = true;
        move += 32; // convert to lower case
    }

    // calculate old positions
    Uint32 oldX = m_PlayerX, oldY = m_PlayerY;
    if( move == 'u' ) ++oldY;
    if( move == 'd' ) --oldY;
    if( move == 'l' ) ++oldX;
    if( move == 'r' ) --oldX;

    // calculate previous step player would have taken if he were traveling linearly
    Uint32 previousX = oldX + (oldX-m_PlayerX);
    Uint32 previousY = oldY + (oldY-m_PlayerY);

    // revert back player position
    if( m_LevelArray[m_PlayerX][m_PlayerY] == '@' )
        m_LevelArray[m_PlayerX][m_PlayerY] = ' ';
    else
        m_LevelArray[m_PlayerX][m_PlayerY] = '+';
    if( m_LevelArray[oldX][oldY] == ' ' )
        m_LevelArray[oldX][oldY] = '@';
    else
        m_LevelArray[oldX][oldY] = '+';

    // player was pushing a box
    if( boxPushed )
    {
        if( m_LevelArray[previousX][previousY] == '$' )
            m_LevelArray[previousX][previousY] = ' ';
        else
            m_LevelArray[previousX][previousY] = '.';
        if( m_LevelArray[oldX][oldY] == ' ')
            m_LevelArray[oldX][oldY] = '$';
        else
            m_LevelArray[oldX][oldY] = '*';
    }

}

// --------------------------------------------------------------
void Level::redo( void )
{
    if( !m_IsLevelValid ) return;
    if( m_UndoDataIndex == m_UndoData.size()-1 ) return;
    this->movePlayer( m_UndoData.at(m_UndoDataIndex) );
    ++m_UndoDataIndex;
}

} // namespace Chocobun
