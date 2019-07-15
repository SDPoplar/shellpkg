#pragma once

#include "Items.h"

enum class CommandId : unsigned int
{
    QUIT = 1,
    HELP = 2,
    DICT = 3,

    APPEND = 11,
    CONSUME = 12,
    SORT = 13,
    MOVE = 14,
    SHOW = 15,
    BALANCE = 16,
    SELL = 17,
    TRASH = 18,
};

class PkgCmd
{
public:
    bool isQuit( void ) const;
    bool isHelp( void ) const;

    PkgCmd& setCommand( CommandId cmd );
    PkgCmd& setFirstArg( unsigned int arg );
    PkgCmd& setFirstArg( ItemLevel arg );
    PkgCmd& setSecondArg( unsigned int arg );

    CommandId getCommand( void ) const;
    unsigned int getFirstArg( void ) const;
    ItemLevel getItemLevelFilter( void ) const;
    unsigned int getSecondArg( void ) const;
    bool needArgument( void ) const;
    bool needSecondArgument( void ) const;

    bool useDefaultArgument( void );
protected:
    CommandId command_id;
    union _unsigned_int
    {
        unsigned int val;
        ItemLevel level;
    } firstArg;
    unsigned int secendArg;
};

class CommandListener
{
public:
    CommandListener( class Package* pkg ) : bindedPackage( pkg ), running( true ) {}
    int listen( void );

protected:
    bool parseInput( PkgCmd &cmd );
    void printUsage( void );
    void printWelcome( void );
    void printInnerItems( void );

    class Package* bindedPackage;
    bool running;
};
