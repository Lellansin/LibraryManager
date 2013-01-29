#  执行的目标目标 ： 所依赖的目标
#	执行这段，所执行的指令
CFLAGS = -g
LDFLAGS = -g

main : login.o utils.o main.o sys_main.o \
	line_item.o list.o sale.o ctrl.o getsale.o \
	store.o

clean :
	rm *.o *~ -rf
