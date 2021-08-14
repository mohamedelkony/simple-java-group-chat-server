import java.util.*;
import java.net.*;
import java.io.*;

public class client2
{
    public static void main(String[] args)
    {
        try(Socket s=new Socket("localhost",50000))
        {
            var reciver=new Scanner(s.getInputStream());
        var sender=new PrintWriter(s.getOutputStream());
        var localScanner=new Scanner(System.in);
            System.out.println("client started");
            boolean x=false;
            while(true)
            {
                if(x)
                {
                    System.out.println("waiting for server");
                    System.out.println("Server: "+reciver.next());
                }
                    else
                {
                    sender.write(localScanner.nextLine());
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
