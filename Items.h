#pragma once

#include <string>
#include <map>

enum class ItemLevel : unsigned int
{
    ALL = 0,        //  use in filter as the default value only
    TRASH = 1,
    NORMAL = 2,
    EXCELLENT = 3,
    LEGEND = 4,
};

class Item
{
public:
    Item( unsigned int id, std::string name, unsigned int maxTrack, ItemLevel level, unsigned int sellPrice )
        : item_id( id ), item_name( name ), item_track( maxTrack ), item_level( level ), item_price( sellPrice ) {}

    std::string getName( void ) const;
    unsigned int getMaxTrack( void ) const;
    ItemLevel getLevel( void ) const;
    unsigned int getSellPrice() const;
    bool canAutoSell() const;
    bool needConfirmSell( void ) const;
    std::string getLevelName( void ) const;
    unsigned int needSpace( unsigned int number ) const;

protected:
    unsigned int item_id;
    std::string item_name;
    unsigned int item_track;
    ItemLevel item_level;
    unsigned int item_price;
};

extern const Item BLOOD_SMALL;
extern const Item MANA_SMALL;
extern const Item WOOD_SWORD;
extern const Item QUILTED_ARMOR;
extern const std::map<unsigned int, const Item&> InnerItems;