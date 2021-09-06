import java.util.*;
import java.net.*;
import java.io.*;

public class clientTester {
    public static void main(String[] args) {
        new client().run();
    }
}

class client {
    Scanner reciver;
    PrintWriter sender;
    Scanner localScanner;

    public client() {

    }

    private void getResponce() {
        new Thread(() -> {
            while (true) {
                System.out.println(reciver.nextLine());
            }
        }).start();
    }

    private void sendTo() {
        new Thread(() -> {
            while (true) {
                sender.println(localScanner.nextLine());
                sender.flush();
            }
        }).start();
    }

    public void run() {

        Socket s = tryConnect();
        while (s == null) {
            System.out.println("trying to reconnect...");
            s = tryConnect();
            try {
                Thread.sleep(4000);
            } catch (Exception e) {
            }
        }

        try {
            System.out.println("connected to server");
            reciver = new Scanner(s.getInputStream(), "UTF-8");
            sender = new PrintWriter(new OutputStreamWriter(s.getOutputStream(), "UTF-8"), true);
            localScanner = new Scanner(System.in);
            System.out.println("client connected");
        } catch (Exception e) {
            System.out.println("Error!!\n" + e.getMessage());
        }
        sendTo();
        getResponce();
    }

    Socket tryConnect() {
        try {
            Socket s = new Socket("localhost", 50000);
            return s;
        } catch (Exception e) {
            System.out.println("can't connect to server\n" + e.getMessage() + "\n");
            return null;
        }
    }
}