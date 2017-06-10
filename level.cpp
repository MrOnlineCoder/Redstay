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

#include "level.h"

#include <iostream>
#include "tinyxml\tinyxml.h"

int Object::GetPropertyInt(std::string name)
{
    int i;
    i = atoi(properties[name].c_str());

    return i;
}

float Object::GetPropertyFloat(std::string name)
{
    float f;
    f = strtod(properties[name].c_str(), NULL);

    return f;
}

std::string Object::GetPropertyString(std::string name)
{
    return properties[name];
}

Level::Level()
{
    //ctor
}

Level::~Level()
{
    //dtor
}

bool Level::LoadFromFile(std::string filename)
{
	layers.clear();
	objects.clear();
    TiXmlDocument levelFile(filename.c_str());

    if (!levelFile.LoadFile())
    {
        std::cout << "Loading level \"" << filename << "\" failed." << std::endl;
        return false;
    }

    //Map element. This is the root element for the whole file.
    TiXmlElement *map;
    map = levelFile.FirstChildElement("map");

    //Set up misc map properties.
    width = atoi(map->Attribute("width"));
    height = atoi(map->Attribute("height"));
    tileWidth = atoi(map->Attribute("tilewidth"));
    tileHeight = atoi(map->Attribute("tileheight"));

    //Tileset stuff
    TiXmlElement *tilesetElement;
    tilesetElement = map->FirstChildElement("tileset");
    firstTileID = atoi(tilesetElement->Attribute("firstgid"));


    if (!tilesetImage.loadFromFile("data/images/tileset.png"))//Load the tileset image
    {
        std::cout << "Failed to load tile sheet." << std::endl;
        return false;
    }

    tilesetImage.setSmooth(false);

    //Columns and rows (of tileset image)
    int columns = tilesetImage.getSize().x / tileWidth;
    int rows = tilesetImage.getSize().y / tileHeight;

    std::vector <sf::Rect<int> > subRects;//container of subrects (to divide the tilesheet image up)

    //tiles/subrects are counted from 0, left to right, top to bottom
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            sf::Rect <int> rect;
            rect.top = y * tileHeight;
            rect.height = y * tileHeight + tileHeight;
            rect.left = x * tileWidth;
            rect.width = x * tileWidth + tileWidth;
            subRects.push_back(rect);
        }
    }

    //Layers
    TiXmlElement *layerElement;
    layerElement = map->FirstChildElement("layer");
    while (layerElement)
    {
        Layer layer;
        if (layerElement->Attribute("opacity") != NULL)//check if opacity attribute exists
        {
            float opacity = strtod(layerElement->Attribute("opacity"), NULL);//convert the (string) opacity element to float
            layer.opacity = 255 * opacity;
        }
        else
        {
            layer.opacity = 255;//if the attribute doesnt exist, default to full opacity
        }

        //Tiles
        TiXmlElement *layerDataElement;
        layerDataElement = layerElement->FirstChildElement("data");

        if (layerDataElement == NULL)
        {
            std::cout << "Bad map. No layer information found." << std::endl;
        }

        TiXmlElement *tileElement;
        tileElement = layerDataElement->FirstChildElement("tile");

        if (tileElement == NULL)
        {
            std::cout << "Bad map. No tile information found." << std::endl;
            return false;
        }

        int x = 0;
        int y = 0;

        while (tileElement)
        {
            int tileGID = atoi(tileElement->Attribute("gid"));
            int subRectToUse = tileGID - firstTileID;//Work out the subrect ID to 'chop up' the tilesheet image.
            if (subRectToUse >= 0)//we only need to (and only can) create a sprite/tile if there is one to display
            {
                sf::Sprite sprite;//sprite for the tile
                sprite.setTexture(tilesetImage);
                sprite.setTextureRect(subRects[subRectToUse]);
                sprite.setPosition(x * tileWidth, y * tileHeight);

				if (tileGID == 4) {
					sprite.setTextureRect(sf::IntRect(3 * 32, 0, 32, 32));
				}

                //sprite.setColor(sf::Color(255, 255, 255, layer.opacity));//Set opacity of the tile.

				if (tileGID == 0) {
					sprite.setColor(sf::Color(255,255,255,0));
				}


                //add tile to layer
                layer.tiles.push_back(sprite);
            }

            tileElement = tileElement->NextSiblingElement("tile");

            //increment x, y
            x++;
            if (x >= width)//if x has "hit" the end (right) of the map, reset it to the start (left)
            {
                x = 0;
                y++;
                if (y >= height)
                {
                    y = 0;
                }
            }
        }

        layers.push_back(layer);

        layerElement = layerElement->NextSiblingElement("layer");
    }

    //Objects
    TiXmlElement *objectGroupElement;
    if (map->FirstChildElement("objectgroup") != NULL)//Check that there is atleast one object layer
    {
        objectGroupElement = map->FirstChildElement("objectgroup");
        while (objectGroupElement)//loop through object layers
        {
            TiXmlElement *objectElement;
            objectElement = objectGroupElement->FirstChildElement("object");
            while (objectElement)//loop through objects
            {
                std::string objectType;
                if (objectElement->Attribute("type") != NULL)
                {
                    objectType = objectElement->Attribute("type");
                }
                std::string objectName;
                if (objectElement->Attribute("name") != NULL)
                {
                    objectName = objectElement->Attribute("name");
                }
                float x = (float) atoi(objectElement->Attribute("x"));
                float y = (float) atoi(objectElement->Attribute("y"));
                float width = (float) atoi(objectElement->Attribute("width"));
                float height = (float) atoi(objectElement->Attribute("height"));

                Object object;
                object.name = objectName;
                object.type = objectType;

                sf::Rect <float> objectRect;
                objectRect.top = y;
                objectRect.left = x;
                objectRect.height = height;
                objectRect.width = width;



                object.rect = objectRect;

                TiXmlElement *properties;
                properties = objectElement->FirstChildElement("properties");
                if (properties != NULL)
                {
                    TiXmlElement *prop;
                    prop = properties->FirstChildElement("property");
                    if (prop != NULL)
                    {
                        while(prop)
                        {
                            std::string propertyName = prop->Attribute("name");
                            std::string propertyValue = prop->Attribute("value");

                            object.properties[propertyName] = propertyValue;

                            prop = prop->NextSiblingElement("property");
                        }
                    }
                }

                objects.push_back(object);

                objectElement = objectElement->NextSiblingElement("object");
            }
            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
    }
    else
    {
        std::cout << "No object layers found..." << std::endl;
    }

    return true;
}

Object Level::GetObject(std::string name)
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i].name == name)
        {
            return objects[i];
        }
    }
}

void Level::SetDrawingBounds(sf::Rect<float> bounds)
{
    drawingBounds = bounds;

    //Adjust the rect so that tiles are drawn just off screen, so you don't see them disappearing.
    drawingBounds.top -= tileHeight;
    drawingBounds.left -= tileWidth;
}

void Level::Draw(sf::RenderWindow &window)
{
    for (int layer = 0; layer < layers.size(); layer++)
    {
        for (int tile = 0; tile < layers[layer].tiles.size(); tile++)
        {
            if (drawingBounds.contains(layers[layer].tiles[tile].getPosition().x, layers[layer].tiles[tile].getPosition().y))
            {
               window.draw(layers[layer].tiles[tile]);
            }
        }
    }
}

std::vector<Object> Level::GetAllObjects() {
	return objects;
}

bool Level::HasObject(std::string name) {
	
	for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i].name == name)
        {
            return true;
        }
    }

	return false;
}