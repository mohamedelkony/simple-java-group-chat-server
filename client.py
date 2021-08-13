from socket import *;
mysocket=socket(AF_INET,SOCK_DGRAM);

msg=input('Input txt').encode();
mysocket.sendto(msg,("localhost",12000));
msg,ser=mysocket.recvfrom(2018);
print(msg);
