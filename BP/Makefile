
SHELL = /bin/sh
#-------------------------------------------------------------------------------
top_srcdir = ../../libcppsim-0.2.5
#top_srcdir =/home/cgomez/SIMULADOR/libcppsim-0.2.1
srcdir = $(top_srcdir)/src
#-------------------------------------------------------------------------------

CXX = c++
CXXFLAGS = -g -Wall -O2 -static
LDADD = $(srcdir)/libcppsim.a
LIBS = 

#all: Makefile simulador

all:
	make simulador

todo:
	mkdir obj
	make simulador

simulador_OBJECTS = obj/simulador.o obj/sistema.o obj/lru.o obj/locks.o \
		    obj/lector.o obj/metodos.o obj/index.o

#-------------------------------------------------------------------------------
obj/simulador.o: simulador.cc $(srcdir)/cppsim.hh $(srcdir)/process.hh \
	$(srcdir)/coroutine.hh $(top_srcdir)/config.h $(srcdir)/handle.hh \
	$(srcdir)/logger.hh $(srcdir)/sqs.hh $(srcdir)/simulation.hh \
	$(srcdir)/sqsDll.hh $(srcdir)/sqsPrio.hh $(srcdir)/mean.hh \
	$(srcdir)/statistic.hh $(srcdir)/var.hh $(srcdir)/repmean.hh \
	$(srcdir)/trremoval.hh $(srcdir)/accum.hh $(srcdir)/bmeans.hh \
	$(srcdir)/bmeans2.hh $(srcdir)/counter.hh $(srcdir)/dump.hh \
	$(srcdir)/histogram.hh $(srcdir)/jain.hh $(srcdir)/quantile.hh \
	$(srcdir)/welch.hh $(srcdir)/rng.hh $(srcdir)/assertions.hh \
        \
        chip.h core.h estadisticas.h \
        pthreads.h sistema.h
	g++ -O2 -c -o obj/simulador.o simulador.cc -Wno-deprecated -Wall
#-------------------------------------------------------------------------------

#--- objetos de la simulacion

obj/sistema.o:  sistema.cc sistema.h 
	g++ -O2 -c -o obj/sistema.o sistema.cc -lm -Wno-deprecated -Wall

obj/lru.o:  lru.cc lru.h
	g++ -O2 -c -o obj/lru.o lru.cc -Wno-deprecated -Wall


obj/locks.o:  locks.cc locks.h
	g++ -O2 -c -o obj/locks.o locks.cc -Wno-deprecated -Wall


obj/lector.o:  lector.cc lector.h
	g++ -O2 -c -o obj/lector.o lector.cc -Wno-deprecated -Wall


obj/metodos.o:  metodos.cc metodos.h
	g++ -O2 -c -o obj/metodos.o metodos.cc -Wno-deprecated -Wall

obj/index.o:  index.cc index.h
	g++ -O2 -c -o obj/index.o index.cc -Wno-deprecated -Wall

#------------------------------------------------------------------------------

simulador: $(simulador_OBJECTS) 
	@rm -f simulador
	$(CXX) $(CXXFLAGS) -o simulador $(simulador_OBJECTS) $(LDADD) $(LIBS)


clean: 
	rm -f obj/*
	rm -f simulador

