#ifndef GLOBAL_H
#define GLOBAL_H

//#include "/home/cgomez/SIMULADOR/libcppsim-0.2.1/src/cppsim.hh"
#include "../../libcppsim-0.2.5/src/cppsim.hh"
#include <stdio.h>
#include <map>
#include <sstream>
#include <fstream>
#include <set>
#include <vector>
#include <list>
#include <assert.h>
#include <string>
#include <cstring>
#include <limits>
#include <queue>
#include <math.h>
#include <algorithm>

using namespace std;

#define MIDE_ESTADISTICAS

#define ASSERT(x) assert(x)

#define PAG_CACHE 64  // tamanho en bytes pagina de cache

#define LEVELCACHE 3
#define SIZE_L1 (32*1024)
#define SIZE_L2 (256*1024)
#define SIZE_L3 (8*1024*1024)
//#define SIZE_L1 32*1024;
//#define SIZE_L2 256*1024;


#define READ 0
#define WRITE 1

#define TOTAL 2
#define ACTIVE 1
#define INACTIVE 0

#define CPU 0
#define CL1 1
#define CL2 2
#define CL3 3

#define NORMAL_TIME 500.0 //segundos con tasa normal		
//#define NORMAL_RATE 0.011 //
#define NORMAL_RATE 0.000009
//#define NORMAL_RATE 0.000000001 //

#define PU_X_CORE 2
#define NCORE_X_L2 1
#define NCORE_X_L3 4
#define NCORE 8

#define BLOQUE_NO_LLENO 0
#define BLOQUE_LLENO 1

#define HASH_ENTRIES 4099

#define MAX_BLOQUES 10 // bloques por tid


#endif
