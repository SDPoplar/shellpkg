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
        std::cout << "未知命令" << buff << std::endl;
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
            std::cout << "缺少参数" << std::endl;
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
            std::cout << "无效的参数" << std::endl;
            return false;
        }
    }
    int tmparg = atoi( buff.c_str() );
    if( tmparg <= 0 )
    {
        std::cout << "无效的参数" << std::endl;
        return false;
    }
    cmd.setFirstArg( (unsigned int)tmparg );
    if( !cmd.needSecondArgument() )
    {
        return true;
    }
    if( std::cin.peek() == '\n' )
    {
        std::cout << "缺少参数" << std::endl;
        return false;
    }
    std::cin >> buff;
    tmparg = atoi( buff.c_str() );
    if( tmparg <= 0 )
    {
        std::cout << "无效的参数" << std::endl;
        return false;
    }
    cmd.setSecondArg( (unsigned int)tmparg );
    return true;
}

void CommandListener::printUsage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\texit                : 退出背包" << std::endl;
    std::cout << "\thelp or usage       : 显示帮助信息" << std::endl;
    std::cout << "\tdict                : 显示可用的内置物品" << std::endl;
    std::cout << "\tget item_id number  : 向背包中加入number个指定ID的物品" << std::endl;
    std::cout << "\tuse item_id         : 消耗一个指定ID的物品" << std::endl;
    std::cout << "\tsort                : 整理背包" << std::endl;
    std::cout << "\tmove from to        : 将from格子内的物品移动到to" << std::endl;
    std::cout << "\tshow [T|N|E|L]      : 显示物品栏中所有/指定品质物品" << std::endl;
    std::cout << "\tmoney               : 显示现有金币数" << std::endl;
    std::cout << "\tsell index          : 售出指定格子(index)内的物品" << std::endl;
    std::cout << "\ttrash               : 自动售出所有垃圾品质物品" << std::endl;
}

void CommandListener::printWelcome( void )
{
    std::cout << "Welcome to package v1.0, use 'help' command if you need help" << std::endl;
}

void CommandListener::printInnerItems( void )
{
    for( auto item : InnerItems )
    {
        std::cout << "ID：" << item.first << "\t[" << item.second.getLevelName() << "]" << item.second.getName()
            << "\t最大堆叠：" << item.second.getMaxTrack() << "\t售价：" << item.second.getSellPrice() << std::endl;
    }
}
