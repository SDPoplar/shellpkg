#include "ShellPackage.h"

int main( void )
{
    return (new CommandListener( new Package( 16 ) ))->listen();
}
