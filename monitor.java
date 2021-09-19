import java.util.*;
import java.net.*;
import java.io.*;
import org.json.*;
public class monitor {
    static int num = 0;

    public static void main(String[] args) {
        ServerSocket s = null;
        Socket c = null;
        PrintWriter sender = null;
            Scanner reader = null;
     
        try {
            s = new ServerSocket(45000);
            System.out.println("Monitor started");
            c = s.accept();
            System.out.println("Connected to server");
            sender = new PrintWriter(new OutputStreamWriter(c.getOutputStream(), "UTF-8"), true);
            reader = new Scanner(c.getInputStream(), "UTF-8");
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return;
        }
        helper.clearConsole();
        while (true) {
            List<clientData> data=new ArrayList<clientData>();;
            try {
                var injson = new JSONObject(reader.next()).getJSONArray("clients");
                
                for (int i = 0; i < injson.length(); i++) {
                    var ob = injson.getJSONObject(i);
                    data.add(new clientData(ob.getInt("id"), ob.getInt("port")));
                }
            } catch (Exception e) {
                System.out.println(e.getMessage() + "12");
                return;
            }
            processData(data);
            try {
                JSONArray arr = new JSONArray();
                for (var x : data) {
                    var ob = new JSONObject().put("id", x.id()).put("port",x.port());
                    arr.put(ob);
                }
                String json = new JSONObject().put("clients", arr).toString();
                sender.println(json);
                sender.flush();
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
