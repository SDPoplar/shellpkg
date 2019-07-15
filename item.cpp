#include "Items.h"

const Item BLOOD_SMALL( 1, "СѪƿ", 64, ItemLevel::NORMAL, 5 );
const Item MANA_SMALL( 2, "С��ƿ", 64, ItemLevel::NORMAL, 5 );
const Item WOOD_SWORD( 3, "ľ��", 1, ItemLevel::NORMAL, 30 );
const Item QUILTED_ARMOR( 4, "����", 1, ItemLevel::NORMAL, 20 );
const Item FISH_BONE( 5, "���", 200, ItemLevel::TRASH, 1 );
const Item BLIZZARD( 6, "������", 200, ItemLevel::TRASH, 1 );
const Item FALLEN_STAR( 7, "����", 16, ItemLevel::EXCELLENT, 300 );
const Item CHERRY_KEYBOARD( 8, "���Ƽ���", 1, ItemLevel::LEGEND, 1000 );

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
        return "����";
    case ItemLevel::LEGEND:
        return "��˵";
    case ItemLevel::NORMAL:
        return "��ͨ";
    case ItemLevel::TRASH:
        return "����";
    default:
        return "δ֪";
    }
}

unsigned int Item::needSpace( unsigned int number ) const
{
    return ( number / this->item_track ) + ( number % this->item_track ? 1 : 0 );
}
