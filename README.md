# Client-Server-BoF
A buffer overflow exercise for a client-server program

was assigned by Dr. Tassos Dimitrio in Kuwait University and was done as group with the peer Eng. Amna Jasser 

This exercise has an overflow vulnerability in “server.c”, specifically in handleConnection method at the line where the function memcpy is called. The memcpy function copies the memory area from to another memory area, in our case will copy content of inputBuffer [1024] to buf [80], this can produce a buffer overflow since the inputBuffer which size is 1024 could exceed the size of the buf which is 80 bytes. The buffer to be used to produce the buffer overflow is the buffer inputBuffer of size 1024 bytes sent by “client.c”.
