#include "ShellPackage.h"

#include <iostream>
std::map<std::string, CommandId> cmdmap = {
    { "exit", CommandId::QUIT },
    { "help", CommandId::HELP },
    { "usage", CommandId::HELP },
    { "get", CommandId::APPEND },
    { "use", CommandId::CONSUME },
    { "sort", CommandId::SORT },
    { "move", CommandId::MOVE },
    { "show", CommandId::SHOW },
    { "money", CommandId::BALANCE },
    { "sell", CommandId::SELL },
    { "trash", CommandId::TRASH },
    { "dict", CommandId::DICT },
};

int CommandListener::listen( void )
{
    this->printWelcome();
    PkgCmd cmd;
    while( this->running )
    {
        if( !this->parseInput( cmd ) )
        {
            continue;
        }
        if( cmd.isQuit() )
        {
            this->running = false;
            continue;
        }
        if( cmd.isHelp() )
        {
            this->printUsage();
            continue;
        }
        if( cmd.getCommand() == CommandId::DICT )
        {
            this->printInnerItems();
            continue;
        }
        if( !this->bindedPackage->exec( cmd ) )
        {
            std::cout << this->bindedPackage->getLastError() << std::endl;
        }
    }
    return 0;
}

bool parseCommand( std::string line, PkgCmd &cmd )
{
    auto cmditem = cmdmap.find( line );
    if( cmditem == cmdmap.end() )
    {
        return false;
    }
    cmd.setCommand( ( *cmditem ).second );
    return true;
}

bool setShowArg( std::string in, PkgCmd& cmd )
{
    char filter = in[ 0 ];
    if( (filter >= 'a') && (filter <= 'z') )
    {
        //  filter -= ('a' - 'A');
        filter -= 32;
    }
    switch( filter )
    {
    case 'T':
        cmd.setFirstArg( ItemLevel::TRASH );
        return true;
    case 'N':
        cmd.setFirstArg( ItemLevel::NORMAL );
        return true;
    case 'E':
        cmd.setFirstArg( ItemLevel::EXCELLENT );
        return true;
    case 'L':
        cmd.setFirstArg( ItemLevel::LEGEND );
        return true;
    }
    return false;
}

bool CommandListener::parseInput( PkgCmd &cmd )
{
    std::cout << "package>";

    std::string buff;
    std::cin >> buff;
    if( buff == "" )
    {
        return false;
    }
    if( !parseCommand( buff, cmd ) )
    {
        std::cout << "δ֪����" << buff << std::endl;
        this->printUsage();
        return false;
    }
    if( !cmd.needArgument() )
    {
        return true;
    }
    if( std::cin.peek() == '\n' )
    {
        if( cmd.useDefaultArgument() )
        {
            return true;
        }
        else
        {
            std::cout << "ȱ�ٲ���" << std::endl;
            return false;
        }
    }
    std::cin >> buff;
    if( cmd.getCommand() == CommandId::SHOW )
    {
        if( setShowArg( buff, cmd ) )
        {
            return true;
        }
        else
        {
            std::cout << "��Ч�Ĳ���" << std::endl;
            return false;
        }
    }
    int tmparg = atoi( buff.c_str() );
    if( tmparg <= 0 )
    {
        std::cout << "��Ч�Ĳ���" << std::endl;
        return false;
    }
    cmd.setFirstArg( (unsigned int)tmparg );
    if( !cmd.needSecondArgument() )
    {
        return true;
    }
    if( std::cin.peek() == '\n' )
    {
        std::cout << "ȱ�ٲ���" << std::endl;
        return false;
    }
    std::cin >> buff;
    tmparg = atoi( buff.c_str() );
    if( tmparg <= 0 )
    {
        std::cout << "��Ч�Ĳ���" << std::endl;
        return false;
    }
    cmd.setSecondArg( (unsigned int)tmparg );
    return true;
}

void CommandListener::printUsage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\texit                : �˳�����" << std::endl;
    std::cout << "\thelp or usage       : ��ʾ������Ϣ" << std::endl;
    std::cout << "\tdict                : ��ʾ���õ�������Ʒ" << std::endl;
    std::cout << "\tget item_id number  : �򱳰��м���number��ָ��ID����Ʒ" << std::endl;
    std::cout << "\tuse item_id         : ����һ��ָ��ID����Ʒ" << std::endl;
    std::cout << "\tsort                : ������" << std::endl;
    std::cout << "\tmove from to        : ��from�����ڵ���Ʒ�ƶ���to" << std::endl;
    std::cout << "\tshow [T|N|E|L]      : ��ʾ��Ʒ��������/ָ��Ʒ����Ʒ" << std::endl;
    std::cout << "\tmoney               : ��ʾ���н����" << std::endl;
    std::cout << "\tsell index          : �۳�ָ������(index)�ڵ���Ʒ" << std::endl;
    std::cout << "\ttrash               : �Զ��۳���������Ʒ����Ʒ" << std::endl;
}

void CommandListener::printWelcome( void )
{
    std::cout << "Welcome to package v1.0, use 'help' command if you need help" << std::endl;
}

void CommandListener::printInnerItems( void )
{
    for( auto item : InnerItems )
    {
        std::cout << "ID��" << item.first << "\t[" << item.second.getLevelName() << "]" << item.second.getName()
            << "\t���ѵ���" << item.second.getMaxTrack() << "\t�ۼۣ�" << item.second.getSellPrice() << std::endl;
    }
}
