#ifndef GLOBAL_H
#define GLOBAL_H

//#include "/home/cgomez/SIMULADOR/libcppsim-0.2.1/src/cppsim.hh"
#include "../../libcppsim-0.2.5/src/cppsim.hh"
#include <stdio.h>
#include <map>
#include <sstream>
#include <set>
#include <vector>
#include <list>
#include <assert.h>
#include <string>
#include <limits>
#include <queue>
#include <math.h>
#include <algorithm>

using namespace std;

#define MIDE_ESTADISTICAS

/*
#define Factor_MakeRanking  1.0
#define Factor_Sort         (Factor_MakeRanking/5.0)
#define Factor_ListaWrite   (Factor_MakeRanking/10.0)
*/

#define ASSERT(x) assert(x)

#define PAG_CACHE 64 // tamanho en bytes pagina de cache

#define READ 0
#define WRITE 1

#define ACTIVE 1
#define INACTIVE 0

#define BLOQUE_NO_LLENO 0
#define BLOQUE_LLENO 1

#define HASH_ENTRIES 4099

#define MAX_BLOQUES 10 // bloques por tid


#endif

