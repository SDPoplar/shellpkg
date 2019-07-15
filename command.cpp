#include "ShellPackage.h"

bool PkgCmd::isQuit( void ) const
{
    return this->command_id == CommandId::QUIT;
}

bool PkgCmd::isHelp( void ) const
{
    return this->command_id == CommandId::HELP;
}

PkgCmd& PkgCmd::setCommand( CommandId cmd )
{
    this->command_id = cmd;
    return *this;
}
PkgCmd& PkgCmd::setFirstArg( unsigned int arg )
{
    this->firstArg.val = arg;
    return *this;
}
PkgCmd& PkgCmd::setFirstArg( ItemLevel _level )
{
    this->firstArg.level = _level;
    return *this;
}
PkgCmd& PkgCmd::setSecondArg( unsigned int arg )
{
    this->secendArg = arg;
    return *this;
}

CommandId PkgCmd::getCommand( void ) const
{
    return this->command_id;
}
unsigned int PkgCmd::getFirstArg( void ) const
{
    return this->firstArg.val;
}
ItemLevel PkgCmd::getItemLevelFilter( void ) const
{
    return this->firstArg.level;
}
unsigned int PkgCmd::getSecondArg( void ) const
{
    return this->secendArg;
}
bool PkgCmd::needArgument( void ) const
{
    return this->needSecondArgument()
        || ( this->command_id == CommandId::CONSUME )
        || ( this->command_id == CommandId::SHOW )
        || ( this->command_id == CommandId::SELL )
        || false;
}
bool PkgCmd::needSecondArgument( void ) const
{
    return false
        || ( this->command_id == CommandId::APPEND )
        || ( this->command_id == CommandId::MOVE )
        || false;
}

bool PkgCmd::useDefaultArgument( void )
{
    if( this->command_id == CommandId::SHOW )
    {
        this->setFirstArg( ItemLevel::ALL );
        return true;
    }
    return false;
}
