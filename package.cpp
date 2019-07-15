#include "ShellPackage.h"

#include <iostream>

unsigned int PackageItem::getItemId() const
{
    return this->item_id;
}
std::string PackageItem::getName( void ) const
{
    return this->item_name;
}
unsigned int PackageItem::getNumber() const
{
    return this->item_num;
}
unsigned int PackageItem::addNumber( unsigned int num )
{
    this->item_num += num;
    return this->getNumber();
}
unsigned int PackageItem::addNumberTillMax( unsigned int num, unsigned int max )
{
    if( max == 0 )
    {
        //  find inner item, and set max to its maxTrack
    }
    unsigned int toFull = max - this->item_num;
    if( toFull >= num )
    {
        this->item_num += num;
        return 0U;
    }
    this->item_num = max;
    return num - toFull;
}
bool PackageItem::consume( unsigned int num )
{
    if( this->item_num >= num )
    {
        this->item_num -= num;
        return true;
    }
    else
    {
        return false;
    }
}
bool PackageItem::onlyOne( void ) const
{
    return this->item_num == 1;
}

bool PackageItem::isFull( unsigned int max ) const
{
    if( max == 0 )
    {
        //  find inner item and set max with its maxTrack
    }
    return this->item_num >= max;
}

Package::Package( unsigned int _volumn )
{
    this->volumn = _volumn;
    for( unsigned int i = 1; i <= _volumn; i++ )
    {
        this->items[ i ] = NULL;
    }
    this->balance = 0;
}

void Package::each( ItemCallBack callback, ItemLevel filter )
{
    for( auto i : this->items )
    {
        if( filter != ItemLevel::ALL )
        {
            if( !i.second )
            {
                continue;
            }
            auto pTpl = InnerItems.find( i.second->getItemId() );
            if( ( pTpl == InnerItems.end() ) || ( pTpl->second.getLevel() != filter ) )
            {
                continue;
            }
        }
        callback( this, i.first, i.second );
    }
}

unsigned int Package::findFirstIndex( unsigned int itemId ) const
{
    for( auto item : this->items )
    {
        if( item.second && ( item.second->getItemId() == itemId ) )
        {
            return item.first;
        }
    }
    return 0;
}

unsigned int Package::findFirstNotFullIndex( unsigned int itemId, unsigned int max ) const
{
    if( max == 0 )
    {
        auto pTpl = InnerItems.find( itemId );
        const Item* tpl = (pTpl != InnerItems.end()) ? &pTpl->second : NULL;
        if( !tpl )
        {
            return 0;
        }
        max = tpl->getMaxTrack();
    }
    for( auto item : this->items )
    {
        if( item.second && (item.second->getItemId() == itemId) && !item.second->isFull( max ) )
        {
            return item.first;
        }
    }
    return 0;
}

unsigned int Package::firstEmptyIndex( void ) const
{
    for( auto item : this->items )
    {
        if( item.second == NULL )
        {
            return item.first;
        }
    }
    return 0;
}

unsigned int Package::countEmpty( void ) const
{
    unsigned int emptyNum = 0;
    for( auto item : this->items )
    {
        if( item.second == NULL )
        {
            emptyNum++;
        }
    }
    return emptyNum;
}

unsigned int Package::getVolumn( void ) const
{
    return this->volumn;
}

unsigned int Package::getBalance( void ) const
{
    return this->balance;
}

void Package::income( unsigned int amount )
{
    this->balance += amount;
}

bool Package::exec( PkgCmd& cmd )
{
    switch( cmd.getCommand() )
    {
    case CommandId::SHOW:
        return this->onShow( cmd.getItemLevelFilter() );
    case CommandId::BALANCE:
        return this->onBalance();
    case CommandId::APPEND:
        return this->onAppend( cmd.getFirstArg(), cmd.getSecondArg() );
    case CommandId::MOVE:
        return this->onMove( cmd.getFirstArg(), cmd.getSecondArg() );
    case CommandId::SELL:
        return this->onSell( cmd.getFirstArg() );
    case CommandId::TRASH:
        return this->onTrash();
    case CommandId::CONSUME:
        return this->onConsume( cmd.getFirstArg() );
    case CommandId::SORT:
        return this->onSort();
    default:
        return false;
    }
}

std::string Package::getLastError( void ) const
{
    return this->lastError;
}

bool Package::onShow( ItemLevel limit )
{
    this->each( []( Package* pkg, unsigned int index, PackageItem* &item ) {
        std::cout << index << ":\t";
        if( item )
        {
            std::cout << item->getItemId() << '\t' << item->getName() << "\t" << item->getNumber();
        }
        else
        {
            std::cout << "Nothing";
        }
        std::cout << std::endl;
    }, limit );
    return true;
}

bool Package::onAppend( unsigned int itemId, unsigned int num )
{
    auto pTpl = InnerItems.find( itemId );
    if( pTpl == InnerItems.end() )
    {
        this->lastError = "无效的物品ID";
        return false;
    }
    auto tpl = pTpl->second;
    while( num > 0 )
    {
        unsigned int tid = this->findFirstNotFullIndex( itemId, tpl.getMaxTrack() );
        if( tid == 0 )
        {
            tid = this->firstEmptyIndex();
            if( tid == 0 )
            {
                this->lastError = "物品栏已满，仍有" + num;
                this->lastError += "个物品未能加入物品栏";
                return false;
            }
            this->items[ tid ] = new PackageItem( itemId, tpl.getName() );
            num--;
        }
        num = this->items[ tid ]->addNumberTillMax( num, tpl.getMaxTrack() );
    }
    return true;
}

bool Package::onConsume( unsigned int itemId )
{
    unsigned int index = this->findFirstIndex( itemId );
    if( !index )
    {
        this->lastError = "背包中没有找到指定的物品";
        return false;
    }
    this->items[ index ]->consume();
    return true;
}

bool Package::onSort( void )
{
    this->lastError = "still in todo list";
    return false;
}

bool Package::onMove( unsigned int fromIndex, unsigned int toIndex )
{
    if( (fromIndex <= 0) || (toIndex <= 0) )
    {
        this->lastError = "指定了无效的物品格位置";
        return false;
    }
    if( (fromIndex > this->volumn) || (toIndex > this->volumn) )
    {
        this->lastError = "指定了无效的物品格位置（超过了背包格数上限）";
        return false;
    }
    PackageItem* t = this->items[ fromIndex ];
    this->items[ fromIndex ] = this->items[ toIndex ];
    this->items[ toIndex ] = t;
    return true;

}
bool Package::onBalance( void )
{
    std::cout << "余额：" << this->balance << std::endl;
    return true;
}

bool userInputConfirm( std::string confirmText )
{
    std::cout << confirmText << ": ";
    char in;
    do
    {
        in = std::cin.get();
        if( (in >= 'A') && (in <= 'Z') )
        {
            in += 32;
        }
    } while( (in != 'y') && (in != 'n') );
    return in == 'y';
}

bool Package::onSell( unsigned int index )
{
    auto psell = this->items.find( index );
    if( psell == this->items.end() )
    {
        this->lastError = "无效的物品格编号";
        return false;
    }
    auto forsell = psell->second;
    if( psell->second == NULL )
    {
        this->lastError = "指定的格子是空的";
        return false;
    }
    auto pTpl = InnerItems.find( forsell->getItemId() );
    if( pTpl == InnerItems.end() )
    {
        this->lastError = "无法识别格子中的物品";
        return false;
    }
    auto ii = pTpl->second;
    if( ii.needConfirmSell() && !userInputConfirm( "您正在出售一件品质为[" + ii.getLevelName() + "]的物品，确认吗？y/n" ) )
    {
        return true;
    }
    this->income( ii.getSellPrice() * forsell->getNumber() );
    delete items[ index ];
    this->items[ index ] = NULL;
    return true;
}

bool Package::onTrash( void )
{
    this->each( []( Package *pkg, unsigned int index, PackageItem*& item ) {
        auto tpl = InnerItems.find( item->getItemId() );
        if( tpl == InnerItems.end() )
        {
            return;
        }
        const Item &ii = tpl->second;
        if( !ii.canAutoSell() )
        {
            return;
        }
        pkg->income( ii.getSellPrice() * item->getNumber() );
        delete item;
        item = NULL;
    } );
    return true;
}
