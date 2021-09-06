import java.util.*;
import java.net.*;
import java.io.*;

public class monitor {
    static int num = 0;

    public static void main(String[] args) {
        ServerSocket s = null;
        Socket c = null;
        ObjectInputStream reader = null;
        ObjectOutputStream oos = null;

        try {
            s = new ServerSocket(45000);
            System.out.println("Monitor started");
            c = s.accept();
            System.out.println("Connected to server");
            reader = new ObjectInputStream(c.getInputStream());
            oos = new ObjectOutputStream(c.getOutputStream());
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return;
        }
        helper.clearConsole();
        while (true) {
            List<clientData> data;
            try {
                data = (List<clientData>) reader.readObject();
            } catch (Exception e) {
                System.out.println(e.getMessage() + "12");
                return;
            }
            processData(data);
            try {
                var d = new ArrayList<clientData>();
                for (var x : data) {
                    d.add(x);
                }
                oos.writeObject(d);
            } catch (Exception e) {
                System.out.println(e.getMessage());
            }
        }
    }

    static void processData(List<clientData> data) {
        var removed = new ArrayList<clientData>();
        for (int i = 0; i < data.size(); i++) {
            try {
                var w = new ServerSocket(data.get(i).port());
                System.out
                        .println("client id#" + data.get(i).id() + ",port #" + data.get(i).port() + " disconnected!!");
                removed.add(data.get(i));
                w.close();
            } catch (Exception e) {
            }
        }
        for (var i : removed)
            data.remove(i);
        System.out.println(java.time.LocalTime.now().getMinute() + ":" + java.time.LocalTime.now().getSecond() + " "
                + data.size() + " clients connected:");
        for (var x : data) {
            System.out.println("client id#" + x.id() + ",port #" + x.port());
        }
        num++;

    }
}
