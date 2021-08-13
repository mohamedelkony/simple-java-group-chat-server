from socket import *;
mysocket=socket(socket.AF_INET,socket.SOCK_DGRAM);

msg="a7a";
mysocket.sendto(msg,("localhost",12000));
msg,ser=mysocket.recvfrom(2018);
print(msg);
