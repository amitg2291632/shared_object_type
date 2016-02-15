# shared_object_type
Find the type of shared object type from C program   
Different CPU architectures using different different shared object files , which in turn support different instruction sets. Application Binary Interface (ABI) is different for each combination of CPU and instruction set. 

lib: the directory containing the compiled code that is specific to a software layer of a processor, the directory is split into more directories within it:
armeabi: compiled code for all ARM based processors only
armeabi-v7a: compiled code for all ARMv7 and above based processors only
arm64-v8a: compiled code for all ARMv8 arm64 and above based processors only 
x86: compiled code for x86 processors only
x86_64: compiled code for x86_64 processors only 
mips: compiled code for MIPS processors only


So this program can detect type of any shared object type
