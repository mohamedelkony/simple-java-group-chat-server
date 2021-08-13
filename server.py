from socket import *
myscoket=socket(AF_INET,SOCK_DGRAM)
myscoket.bind(('',12000))
print ("ready")
x=1;
while 1:
    x=x+1
    msg,add=myscoket.recvfrom(2048);
    msg=msg.upper();
    myscoket.sendto(msg,add);