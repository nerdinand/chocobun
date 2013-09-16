/*
 * This file is part of Sokoban.
 *
 * Sokoban is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sokoban is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sokoban.  If not, see <http://www.gnu.org/licenses/>.
 */

// --------------------------------------------------------------
// Collection Parser
// --------------------------------------------------------------

// --------------------------------------------------------------
// include files

#include <core/CollectionParser.hpp>
#include <core/CollectionParserSOK.hpp>
#include <core/RLE.hpp>
#include <core/Level.hpp>
#include <core/Exception.hpp>
#include <core/Config.hpp>
#include <fstream>
#include <sstream>

namespace Sokoban {

// --------------------------------------------------------------
CollectionParser::CollectionParser( void )
{
}

// --------------------------------------------------------------
CollectionParser::~CollectionParser( void )
{
}

// --------------------------------------------------------------
std::string CollectionParser::parse( const std::string& fileName, std::map<std::string,Level*>& levelMap )
{

    // open the file
    std::ifstream file( fileName.c_str() );
    if( !file.is_open() )
        throw Exception( "[CollectionParser::parse] attempt to open collection file failed" );

    // TODO determine file format
    CollectionParserBase* parser = new CollectionParserSOK();

    // parse
    std::string result = parser->parse( file, levelMap );
    delete parser;
    return result;
}

// --------------------------------------------------------------
void CollectionParser::save( const std::string& fileName, std::map<std::string,Level*>& levelMap )
{

    //

    // open the file
    std::ofstream file( fileName.c_str(), std::ofstream::out );
    if( !file.is_open() )
        throw Exception( "[CollectionParser::save] unable to open file for saving" );

    // default export format is SOK
    CollectionParserBase* parser = new CollectionParserSOK();
    parser->save( file, levelMap );
    delete parser;
}

} // namespace Sokoban
