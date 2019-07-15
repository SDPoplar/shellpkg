#pragma once

#include "PackageCommand.h"
#include "Items.h"

#include <map>

class PackageItem
{
public:
    PackageItem( unsigned int itemId, std::string itemName )
        : item_id( itemId ), item_name( itemName ), item_num( 1 ) {}
    unsigned int getItemId() const;
    std::string getName( void ) const;
    unsigned int getNumber() const;
    unsigned int addNumber( unsigned int num );
    unsigned int addNumberTillMax( unsigned int num, unsigned int max = 0U );
    bool consume( unsigned int num = 1 );
    bool onlyOne( void ) const;
    bool isFull( unsigned int max = 0U ) const;
protected:
    unsigned int item_id;
    std::string item_name;
    unsigned int item_num;
};

class Package
{
public:
    Package( unsigned int _volumn );
    unsigned int getBalance( void ) const;
    bool exec( PkgCmd &cmd );
    std::string getLastError( void ) const;
    unsigned int getVolumn( void ) const;
    void income( unsigned int amount );
    unsigned int countEmpty( void ) const;

protected:
    bool onShow( ItemLevel limit );
    bool onAppend( unsigned int itemId, unsigned int num );
    bool onConsume( unsigned int itemId );
    bool onSort( void );
    bool onMove( unsigned int fromIndex, unsigned int toIndex );
    bool onBalance( void );
    bool onSell( unsigned int index );
    bool onTrash( void );

    typedef void(*ItemCallBack)(Package *pkg, unsigned int index, PackageItem*& item);
    void each( ItemCallBack callback, ItemLevel filter = ItemLevel::ALL );
    std::map<unsigned int,PackageItem*> items;
    unsigned int findFirstIndex( unsigned int itemId ) const;
    unsigned int firstEmptyIndex( void ) const;
    unsigned int findFirstNotFullIndex( unsigned int itemId, unsigned int max = 0U ) const;
    

    unsigned int balance;
    unsigned int volumn;

    std::string lastError;
};
