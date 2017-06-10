/*********************************************************************
Quinn Schwab
16/08/2010

SFML Tiled Map Loader

The zlib license has been used to make this software fully compatible
with SFML. See http://www.sfml-dev.org/license.php

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment
   in the product documentation would be appreciated but
   is not required.

2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
   source distribution.
*********************************************************************/

#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

//Object class. Everything is kept public in case you want to get a different type to the supported ones.
//Also you will be able to modify things. For example doors/changing walls etc (Just give the door a static type and a unique name)
//Kept in same files as level just for convenience. (only have to add two files to your project)
class Object{
    public:
        int GetPropertyInt(std::string name);
        float GetPropertyFloat(std::string name);
        std::string GetPropertyString(std::string name);
        std::string name;
        std::string type;
        sf::Rect <float> rect;
        std::map <std::string, std::string> properties;//All properties of the object. Values are stored as strings and mapped by strings(names provided in editor).
};

struct Layer{
    int opacity;
    std::vector <sf::Sprite> tiles;
};

class Level
{
    public:
        Level();
        virtual ~Level();
        //Loads the map. Returns false if it fails.
        bool LoadFromFile(std::string filename);
        //Returns the first object found with the given name. This is why we need unique names :)
        Object GetObject(std::string name);
        //Returns true if the given pixel is solid.
        bool IsSolidPixel(int x, int y);
        //Returns true if the given tile is solid. DO WE NEED THIS?
        bool IsSolidTile(int x, int y);
        //Moves the map. Although I would recommend using a view instead in most cases.
        void Move(int xStep, int yStep);
        //Set the area to draw. This rect is usually provided directly from the view you are using.
        void SetDrawingBounds(sf::Rect<float> bounds);
        //Draws the map to the provided window.
        void Draw(sf::RenderWindow &window);
		//Get All objects
		std::vector<Object> GetAllObjects();
		//Needed.
		bool HasObject(std::string name);
        //Width(tiles), height(tiles), tile width(pixels), tile height(pixels) of the map.
        int width, height, tileWidth, tileHeight;
        //Used to offset the tile number.
        int firstTileID;
        //Used to clip anything off screen.
        sf::Rect <float> drawingBounds;
        //The tileset image.
        sf::Texture tilesetImage;
        //This stores all the solid areas (objects with type 'solid'). This gets checked by the IsSolid function.
        std::vector <sf::Rect <int> > solidObjects;
        //This stores all objects (including 'solid' types)
        std::vector <Object> objects;
        //This stores each layer of sprites/tiles so they can be drawn in order.
        std::vector <Layer> layers;
};

#endif // LEVEL_H
 