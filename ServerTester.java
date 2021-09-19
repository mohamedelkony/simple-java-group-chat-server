
import java.util.*;
import java.net.*;
import java.io.*;
import org.json.*;



public class ServerTester {
    public static void main(String[] args) {
        ServerSocket s = null;
        try {
            s = new ServerSocket(50000);
            System.out.println("Server started");
            server serv = new server();
            new Thread(serv.new cLientsHandler(s)).start();
            Socket ms = new Socket("localhost", 45000);
            System.out.println("Connected to monitor");
            new Thread(serv.new serverMonitor(ms)).start();
        } catch (Exception e) {
            System.out.println("failed to connect!!\n" + e.getMessage());
        }
    }
}

class server {
    Scanner localScanner;

    public server() {
        this.localScanner = new Scanner(System.in);
    }

    List<clientData> data = new ArrayList<clientData>();
    List<Socket> clients = new ArrayList<Socket>();

    class serverMonitor implements Runnable {
        Socket s;

        public serverMonitor(Socket s) {
            this.s = s;
        }

        public void run() {
            PrintWriter sender = null;
            Scanner reader = null;
            try {
                sender = new PrintWriter(new OutputStreamWriter(s.getOutputStream(), "UTF-8"), true);
                reader = new Scanner(s.getInputStream(), "UTF-8");
            } catch (Exception e) {
                System.out.println(e.getMessage());
                return;
            }
            while (true) {
                try {

                    JSONArray arr = new JSONArray();
                    for (var x : data) {
                        var ob = new JSONObject().put("id", x.id()).put("port",x.port());
                        arr.put(ob);
                    }
                    String json = new JSONObject().put("clients", arr).toString();
                    sender.println(json);
                    sender.flush();

                    var injson = new JSONObject(reader.next()).getJSONArray("clients");
                    var indata=new ArrayList<clientData>();
                    for (int i = 0; i < injson.length(); i++) {
                        var ob = injson.getJSONObject(i);
                        indata.add(new clientData(ob.getInt("id"), ob.getInt("port")));
                    }
                    data.clear();
                    for (var x : indata) {
                        data.add(x);
                    }
                    Thread.sleep(3000);
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                }
            }
        }
    }

    class cLientsHandler implements Runnable {
        int id = 0;
        ServerSocket s;

        public cLientsHandler(ServerSocket s) {
            this.s = s;
        }

        public void run() {
            try {
                while (true) {
                    var a = s.accept();
                    data.add(new clientData(id, a.getPort()));
                    clients.add(a);
                    Thread t = new Thread(new clientThread(a, id));
                    t.start();
                    id++;
                }
            } catch (Exception e) {
                System.out.println(e.getMessage());
            }
        }
    }

    class clientThread implements Runnable {

        Socket s;
        int id;
        Scanner reciver;
        PrintWriter sender;

        private void startLisetning() {
            new Thread(() -> {
                while (true) {
                    try {
                        String line = reciver.nextLine();
                        line = "#" + id + " :" + line;
                        for (Socket c : clients) {
                            if (c.getPort() == s.getPort())
                                continue;
                            try {
                                var dsender = new PrintWriter(new OutputStreamWriter(c.getOutputStream(), "UTF-8"),
                                        true);
                                dsender.println(line);
                                dsender.flush();
                            } catch (Exception e) {
                            }
                        }
                        System.out.println(line);

                    } catch (NoSuchElementException e) {

                    }
                }
            }).start();
            ;
        }

        void sendTo() {
            new Thread(() -> {
                while (true) {
                    synchronized (localScanner) {
                        sender.println(localScanner.nextLine());
                        sender.flush();
                    }
                }
            }).start();
            ;
        }

        public clientThread(Socket s, int id) {
            this.s = s;
            this.id = id;
            try {
                this.reciver = new Scanner(s.getInputStream(), "UTF-8");
                this.sender = new PrintWriter(new OutputStreamWriter(s.getOutputStream(), "UTF-8"), true);
            } catch (Exception e) {
            }
        }

        @Override
        public void run() {
            System.out.println("client " + id + " connected");
            startLisetning();

        }
    }
}