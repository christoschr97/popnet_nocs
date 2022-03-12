CC = g++

CFLAGS = -O3

LINKFLAGS = -lm
.SUFFIXES: .o .cc

HEADERS = index.h SString.h SStd.h SRGen.h Svector.h configuration.h mess_queue.h mess_event.h flit.h sim_foundation.h sim_router.h

TARGET = isim

SIM_SRCS = SString.cc SStd.cc SRGen.cc configuration.cc flit.cc sim_router.cc sim_routing.cc sim_vcarb.cc  mess_event.cc sim_foundation.cc mess_queue.cc main.cc 
SIM_OBJS = $(SIM_SRCS:.cc=.o)


$(TARGET):$(SIM_OBJS) 
	$(CC) -o isim $(SIM_OBJS) $(LINKFLAGS)

configuration.o: configuration.cc configuration.h SStd.h SRGen.h index.h \
  SString.h
flit.o: flit.cc flit.h index.h SStd.h
main.o: main.cc index.h SStd.h SRGen.h configuration.h sim_foundation.h \
  sim_router.h flit.h mess_event.h mess_queue.h
mess_event.o: mess_event.cc mess_event.h index.h flit.h SStd.h
mess_queue.o: mess_queue.cc mess_queue.h SStd.h index.h configuration.h \
  SRGen.h sim_foundation.h sim_router.h flit.h mess_event.h
RGen.o: RGen.cc RGen.h
sim_foundation.o: sim_foundation.cc sim_foundation.h sim_router.h flit.h \
  index.h SStd.h SRGen.h configuration.h mess_event.h mess_queue.h 
sim_router.o: sim_router.cc sim_router.h flit.h index.h SStd.h SRGen.h \
  configuration.h sim_foundation.h mess_event.h mess_queue.h
sim_routing.o: sim_routing.cc sim_router.h flit.h index.h SStd.h SRGen.h \
  configuration.h sim_foundation.h mess_event.h mess_queue.h
sim_vcarb.o: sim_vcarb.cc sim_router.h flit.h index.h SStd.h SRGen.h \
  configuration.h sim_foundation.h mess_event.h mess_queue.h
SRGen.o: SRGen.cc SRGen.h
SStd.o: SStd.cc SStd.h
SString.o: SString.cc SString.h index.h SStd.h
Svector.o: Svector.cc

.cc.o:
	$(CC) $(CFLAGS) -c $< > gcc-warning_filter 2>&1

clean:
	rm -f *.o isim gcc-warning_filter

