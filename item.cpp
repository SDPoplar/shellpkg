#include "Items.h"

const Item BLOOD_SMALL( 1, "Ð¡ÑªÆ¿", 64, ItemLevel::NORMAL, 5 );
const Item MANA_SMALL( 2, "Ð¡À¶Æ¿", 64, ItemLevel::NORMAL, 5 );
const Item WOOD_SWORD( 3, "Ä¾½£", 1, ItemLevel::NORMAL, 30 );
const Item QUILTED_ARMOR( 4, "²¼¼×", 1, ItemLevel::NORMAL, 20 );
const Item FISH_BONE( 5, "Óã´Ì", 200, ItemLevel::TRASH, 1 );
const Item BLIZZARD( 6, "²£Á§Ôü", 200, ItemLevel::TRASH, 1 );
const Item FALLEN_STAR( 7, "ÔÉÌú", 16, ItemLevel::EXCELLENT, 300 );
const Item CHERRY_KEYBOARD( 8, "ÃûÅÆ¼üÅÌ", 1, ItemLevel::LEGEND, 1000 );

const std::map<unsigned int, const Item&> InnerItems = {
    { 1, BLOOD_SMALL }, { 2, MANA_SMALL }, { 3, WOOD_SWORD }, { 4, QUILTED_ARMOR },
    { 5, FISH_BONE }, { 6, BLIZZARD }, { 7, FALLEN_STAR }, { 8, CHERRY_KEYBOARD }
};

std::string Item::getName( void ) const
{
    return this->item_name;
}
unsigned int Item::getMaxTrack( void ) const
{
    return this->item_track;
}
ItemLevel Item::getLevel( void ) const
{
    return this->item_level;
}
unsigned int Item::getSellPrice() const
{
    return this->item_price;
}

bool Item::canAutoSell( void ) const
{
    return false
        || ( this->item_level == ItemLevel::TRASH )
        || false;
}

bool Item::needConfirmSell( void ) const
{
    return false
        || ( this->item_level == ItemLevel::LEGEND )
        || ( this->item_level == ItemLevel::EXCELLENT )
        || false;
}

std::string Item::getLevelName( void ) const
{
    switch( this->item_level )
    {
    case ItemLevel::EXCELLENT:
        return "¾«Á¼";
    case ItemLevel::LEGEND:
        return "´«Ëµ";
    case ItemLevel::NORMAL:
        return "ÆÕÍ¨";
    case ItemLevel::TRASH:
        return "À¬»ø";
    default:
        return "Î´Öª";
    }
}

unsigned int Item::needSpace( unsigned int number ) const
{
    return ( number / this->item_track ) + ( number % this->item_track ? 1 : 0 );
}
