Installing MikeNet

Place zipped MikeNet file in home directory

Unzip it using:
	-> unzip mikenet_v8.zipped
	
This will create a directory called Mikenet-v8.0

To set Environment Variable (so system knows where Mikenet libraries are located):

- If you are already in csh compatible shell. Try the below command first, if you are not in csh it will throw an error (to switch to csh (if installed) type 'tsch', then return):
	-> setenv MIKENET_DIR ${HOME}/Mikenet-v8.0
	
- If you are in bash:
	-> export MIKENET_DIR=${HOME}/Mikenet-v8.0
	or -> export MIKENET_DIR=~/Mikenet-v8.0
	
To build the MikeNet libraries run the following lines (pressing return after each)*:

	-> cd Mikenet-v8.0/src
	-> make clean
	-> make gcc
	
* If you are using a Mac then it is possible some files will not be transferred to the relevant folders automatically after running the command 'make gcc'. You will therefore need to copy and paste from the folder Mikenet-8.0/src the file'libmikenet.a' to folder ~/mikenet/lib/, and all '.o' and '.h' files from folder Mikenet-8.0/src to the folder ~mikenet/lib/.

To see if this has worked lets try running the xor demo:

Change directories to:

-> ~/mikenet/demos/xor

Compile the xor script:
-> make xor

Try running the xor script:
-> ./xor

This will build and train a mini network, printing to screan the iteration number and sum squared error (which should decrease). The script will end when the error is below 0.01. 

If you get this output then its all working! CONGRATULATIONS, we're all set to go!!!

