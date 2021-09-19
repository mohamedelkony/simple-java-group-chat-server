
import java.io.*;
class clientData  {
    private Integer id;
    private int port;

    public clientData(int id, int port) {
        this.id = id;
        this.port = port;
    }

    public int port() {
        return port;
    }

    public int id() {
        return id;
    }

}