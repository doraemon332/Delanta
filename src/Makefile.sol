# Makefile for Solaris 2.4 for x86
# Changes contributed by Izuskin Kromosoeto and The Maniac

CC      = gcc
PROF    =
DEBUG   =
NOCRYPT =
O_FLAGS = -O
C_FLAGS = $(O_FLAGS) -Wall -DSYSV $(DEBUG) $(PROF) $(NOCRYPT)
L_FLAGS =                  $(DEBUG) $(PROF) -lelf -lucb -lsocket -lnsl

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o comm.o const.o \
          db.o fight.o handler.o interp.o magic.o save.o ssm.o special.o      \
          update.o mob_commands.o mob_prog.o id.o bet.o bit.o mem.o olc.o     \
	  olc_act.o olc_save.o string.o quest.o skills.o

delanta: $(O_FILES)
	rm -f delanta
	$(CC) $(L_FLAGS) -o delanta $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

clean:
	rm -f *.o delanta