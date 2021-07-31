# sacak_la_implementation

Implementation of the SACA-K+LA algorithm for a bachelor thesis.

Usage:\
Following options are implemented:\
<ul> -h &emsp;&emsp;&emsp;&nbsp; : how this text </br> 
-i &ltarg>&nbsp;&nbsp;&nbsp; : specify the path &ltarg> to the input file </br>
-a &ltarg>&nbsp;&nbsp; : specify the path &ltarg> to the alphabet file if wanted, default is ascii </br>
-d &emsp;&emsp;&emsp;&nbsp; : show debug output, not recommended for inputs larger than 200 byte </br>
-o &emsp;&emsp;&emsp;&nbsp; : show the output on the terminal, not recommended for large inputs </br>
-g &ltarg>&nbsp; : generate input with size &ltarg> (integer), only used for testing - might have bugs </br>
-m &ltarg> : specify the algorithm (1 = inplace, 2 = single aux. array, 3 = two aux. array, 4 = only sacak), </br>
&ensp;&ensp;&ensp;&emsp;&emsp;&emsp;          default is all three versions of SACA-K+LA </br>
-t &emsp;&emsp;&emsp;&nbsp; : test result (sa, la) for correctness, may take a while for large inputs </ul>
	
Combined options (eg. -doh) do not work, they have to separated by spaces

To compile the source code use the following line inside the ...\src folder (GNU make required).\
The binary files can then be found in the ...\src\bin folder.

	C:\...\src> make	

To redo the experiments from the thesis use the following line inside the ...\src\bin folder (where \<arg\> has to be replaced accordingly):

	C:\...\bin> sacak_la_implementation -m <arg> -i <arg>

The input string can not contain '$' (0x24) or 'SUB' (0x1A) symbols, otherwise the program does not work as intended.
