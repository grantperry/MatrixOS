set history filename ~/.gdb_history
set history save
file bin/MatrixOS.bin
target remote localhost:1234
break kernel_main
continue
