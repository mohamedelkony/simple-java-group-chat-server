import java.util.*;
import java.net.*;
import java.io.*;

public class Server2
{
    public static void main(String[] args)
    {
        int i=0;
        try(ServerSocket s=new ServerSocket(50000))
        {
            System.out.println("Server started");
            while(true)
            {
                var a=s.accept();
                Thread t=new Thread(new task(a));
                t.start();
                System.out.println("Client"+String.valueOf(i)+" connected!");
                i++;
            }
        }
        catch(Exception e)
        {
            System.out.println("Server failed to boot!!");
        }
        
    }
}
class task implements Runnable
{
    Socket s;
    public task(Socket s)
    {
        this.s=s;
    }
    @Override
    public void run()
    {
        try {    
            var reciver=new Scanner(s.getInputStream(), "UTF-8");
            var sender=new PrintWriter(new OutputStreamWriter(s.getOutputStream(),"UTF-8"),true);
            var localScanner=new Scanner(System.in);
            System.out.println("client connected");
            boolean x=true;
            while(true)
            {
                if(x)
                {
                    System.out.println("waiting for client");
                    System.out.println("Client: "+reciver.nextLine());
                    localScanner.reset();
                }
                    else
                {
                    sender.println(localScanner.nextLine());
                    sender.flush();
                }
                x=!x;
            }
       
        } catch (Exception e)
        { 
        }
        
    }
}