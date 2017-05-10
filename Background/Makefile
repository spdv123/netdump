CC = g++
PROG = netdump
# CCOPT = -g -xansi -signed -g3
DEFS =

# Standard CFLAGS
CFLAGS = $(CCOPT) $(DEFS) $(INCLS) -g -O0

# Standard LDFLAGS
LDFLAGS =  -L/usr/local/lib

# Standard LIBS
LIBS = -lpcap

.c.o:
	@rm -f $@
	$(CC) $(CFLAGS) -c ./$*.cpp ./$*.h

CSRC =	netdump.cpp \
	util.cpp setsignal.cpp Pack.cpp \
	Protocol/Protocol.cpp \
	Protocol/ARP.cpp \
	Protocol/TCP.cpp \
	Protocol/UDP.cpp \
	Protocol/ICMP.cpp \
	Protocol/OSPF.cpp \
	Protocol/OSPF/base.cpp \
	Protocol/OSPF/hello.cpp \
	Protocol/OSPF/DD.cpp \
	Protocol/OSPF/LSR.cpp \
	Protocol/OSPF/LSU.cpp \
	Protocol/OSPF/LSAck.cpp \
	socket/socklib.cpp \
	socket/sockexc.cpp

SRC =	$(CSRC) $(LOCALSRC)

OBJ =	$(CSRC:.cpp=.o) $(GENSRC:.cpp=.o) $(LOCALSRC:.cpp=.o)
HDR =

CLEANFILES = $(PROG) $(OBJ)

all: $(PROG)

$(PROG): $(OBJ)
	@rm -f $@
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)

clean:
	rm -f netdump
	find . -name "*.o"  | xargs rm -f
