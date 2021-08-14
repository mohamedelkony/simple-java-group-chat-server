import java.util.*;
import java.net.*;
import java.io.*;

public class client2
{
    public static void main(String[] args)
    {
        try(Socket s=new Socket("localhost",50000))
        {
            var reciver=new Scanner(s.getInputStream(), "UTF-8");
            var sender=new PrintWriter(new OutputStreamWriter(s.getOutputStream(),"UTF-8"),true);
            var localScanner=new Scanner(System.in);
            System.out.println("client connected");
            boolean x=false;
            while(true)
            {
                if(x)
                {
                    System.out.println("waiting for server");
                    System.out.println("Server: "+reciver.nextLine());
                    localScanner.reset();
                }
                    else
                {
                    sender.println(localScanner.nextLine());
                    sender.flush();
                }
                x=!x;
            }
         } 
   
        catch(Exception e)
        {
            System.out.println("Client failed to boot!!");
        }
        
    }
}
