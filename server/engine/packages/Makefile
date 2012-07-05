
SRCDIR=.
SRC:=$(wildcard $(SRCDIR)/*.c)
OBJDIR=obj
OBJ=$(addprefix $(OBJDIR)/,$(subst .c,.o,$(SRC)))

# gamed需要的packeages
GAMESRCDIR=$(SRCDIR)/game
GAME_SRC:=$(wildcard $(GAMESRCDIR)/*.c)
GAMEOBJDIR=$(OBJDIR)/game
GAME_OBJ=$(addprefix $(OBJDIR)/,$(subst .c,.o,$(GAME_SRC)))
GAME_PACKAGE=game_packages.a

# dbd需要的packages
DBSRCDIR=$(SRCDIR)/db
DB_SRC:=$(wildcard $(DBSRCDIR)/*.c)
DBOBJDIR=$(OBJDIR)/db
DB_OBJ=$(addprefix $(OBJDIR)/,$(subst .c,.o,$(DB_SRC)))
DB_PACKAGE=db_packages.a

# netd需要的packages
NETDSRCDIR=$(SRCDIR)/netd
NETD_SRC:=$(wildcard $(NETDSRCDIR)/*.c)
NETDOBJDIR=$(OBJDIR)/netd
NETD_OBJ=$(addprefix $(OBJDIR)/,$(subst .c,.o,$(NETD_SRC)))
NETD_PACKAGE=netd_packages.a


# include目录
INCLUDES = -I../include/ -I../ -I./ -I../Berkeley/include -I/usr/local/include -I../grammar/ -I../lib -I/usr/local/include/apr-1 -I/usr/local/include/mysql

_game:predo depend game_packages
_db:predo depend db_packages
_netd:predo depend netd_packages

all:predo depend game_packages db_packages netd_packages

predo:
	-test -d $(OBJDIR) || mkdir $(OBJDIR)
	-test -d $(GAMEOBJDIR) || mkdir $(GAMEOBJDIR)
	-test -d $(DBOBJDIR) || mkdir $(DBOBJDIR)
	-test -d $(NETDOBJDIR) || mkdir $(NETDOBJDIR)
depend: 
	-rm -f tmpdepend
	for i in $(SRCDIR)/*.c; do $(CC) $(INCLUDES) -MM -DDEPEND $$i >>tmpdepend; done
	sed -e "s!^[^ ]!$(OBJDIR)/&!" <tmpdepend >.Dependencies
	for i in $(GAMESRCDIR)/*.c; do $(CC) $(INCLUDES) -MM -DDEPEND $$i >>tmpdepend; done
	sed -e "s!^[^ ]!$(GAMEOBJDIR)/&!" <tmpdepend >>.Dependencies
	for i in $(DBSRCDIR)/*.c; do $(CC) $(INCLUDES) -MM -DDEPEND $$i >>tmpdepend; done
	sed -e "s!^[^ ]!$(DBOBJDIR)/&!" <tmpdepend >>.Dependencies
	for i in $(NETDSRCDIR)/*.c; do $(CC) $(INCLUDES) -MM -DDEPEND $$i >>tmpdepend; done
	sed -e "s!^[^ ]!$(NETDOBJDIR)/&!" <tmpdepend >>.Dependencies
	-rm -f tmpdepend


-include .Dependencies
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTIMIZE) -o $@ -c $<
$(GAMEOBJDIR)/%.o: $(GAMESRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTIMIZE) -o $@ -c $<
$(DBOBJDIR)/%.o: $(DBSRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTIMIZE) -o $@ -c $<
$(NETDOBJDIR)/%.o: $(NETDSRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTIMIZE) -o $@ -c $<

game_packages:$(OBJ) $(GAME_OBJ) 

	ar rcu $(GAME_PACKAGE) $(OBJ) $(GAME_OBJ) 
	$(RANLIB) $(GAME_PACKAGE)

db_packages:$(OBJ) $(DB_OBJ) 
	ar rcu $(DB_PACKAGE) $(OBJ) $(DB_OBJ) 
	$(RANLIB) $(DB_PACKAGE)

netd_packages:$(OBJ) $(NETD_OBJ) 
	ar rcu $(NETD_PACKAGE) $(OBJ) $(NETD_OBJ)
	$(RANLIB) $(NETD_PACKAGE)

clean:
	-rm -rf $(OBJDIR) 
	-rm -f .Dependencies 
	-rm -f game_packages.a
	-rm -f db_packages.a
	-rm -f netd_packages.a
