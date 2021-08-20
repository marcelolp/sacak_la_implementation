Following options are implemented:
-h  	 : show this text
-i <arg> : specify the path <arg> to the input file 
-a <arg> : specify the path <arg> to the alphabet file if wanted, default is ascii 
-d  	 : show debug output, not recommended for inputs larger than 200 byte 
-o  	 : show the output on the terminal, not recommended for large inputs 
-m <arg> : specify the algorithm (1 = inplace, 2 = single aux. array, 3 = two aux. array, 4 = only sacak), 
           default is all three versions of SACA-K+LA
-t  	 : test result (sa, la) for correctness, may take a while for large inputs 
-w <arg> : write the output (Lyndon array only) into the file at <arg> in CSV format
	
Combined options (eg. -doh) do not work, they have to separated by spaces


To compile the source code using GNU make, run the following line inside the main project folder. 
The binary files can then be found in the ...\bin folder.

	C:\...> make	

To compile the source code without the makefile, run the following line inside the ...\src folder 
using the Visual Studio Command Prompt (normal command prompt will not work) 

	C:\...\src> cl sacak_la_main.c file_io.c test.c sacak_la.c symbols.c

and move the resulting .exe file to ...\bin


To redo the experiments from the thesis use the following line inside the ...\bin folder 
(where <arg> has to be replaced accordingly):

	C:\...\bin> sacak_la_implementation -m <arg> -i <arg>

The input string can not contain '$' (0x24) or 'SUB' (0x1A) symbols, otherwise the program does not work as intended.
