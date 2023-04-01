Deimantas Gilys #A20434583
Francisco Barba Cuellar #A20121767
Nathan Cook #A20458336

this readme refers to how to run part 1.

I have generated the output programs of each of my test cases in the form of memLeaker_X. The last made program is memLeaker.c.

To run you need to use either builtin GDB... so command:

"gdb ./memLeaker_X"

A prompt will open with (gdb) and you will run this command to test using gdb:

"run" *There are other miscelanious commands showcased in the doc but this is not important

Next we assume you have Valgrind setup and ready to use systemwide. To test each program with Valgrind we will use this command:

"valgrind ./memLeaker_X" 

This will run valgrind on the program and output with the overview and results

To get more detailed errors we can run this:

"valgrind --leak-check=full ./memLeaker_X"

This completes part 1.