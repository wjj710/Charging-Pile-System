#include "../headers/Pile.h"
#include <stdlib.h>

Pile::Pile(char *argv[])
{
    pileNo=argv[1];
    power=atof(argv[2]);
    chargingQueueLen=atoi(argv[3]);
}

