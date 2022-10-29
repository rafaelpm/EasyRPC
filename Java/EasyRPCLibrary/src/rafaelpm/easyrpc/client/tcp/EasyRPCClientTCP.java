package rafaelpm.easyrpc.client.tcp;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.client.EasyRPCClientConnection;

/**
 *
 * @author Rafael
 */
public class EasyRPCClientTCP extends EasyRPCClientConnection {
    
    private Socket socket = null;
    
    private String host = "";
    private int port = 2000;
    
    private DataOutputStream dos;
    private DataInputStream dis;
    
    public EasyRPCClientTCP(String host, int port){
        this.host = host;
        this.port = port;
    }
    
    @Override
    public boolean isConnected() {
        if(socket == null){
            return false;
        }
        return socket.isConnected();
    }

    @Override
    public boolean connect() {
        if(host.isEmpty()){
            return false;
        }
        try {
            SocketAddress endpoint = new InetSocketAddress(host, port);
            socket = new Socket();
            socket.connect(endpoint, 5000);//Era 3000
            dos = new DataOutputStream(socket.getOutputStream());    
            dis = new DataInputStream(socket.getInputStream());
            return true;
        } catch (IOException ex) {
            Logger.getLogger(EasyRPCClientTCP.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }

    @Override
    public void disconnect() {
        if(socket != null){
            try {
                socket.close();
                socket = null;
            } catch (IOException ex) {
                Logger.getLogger(EasyRPCClientTCP.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        if(dis != null){
            try {
                dis.close();
                dis = null;
            } catch (IOException ex) {
                Logger.getLogger(EasyRPCClientTCP.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        if(dos != null){
            try {
                dos.close();
                dos = null;
            } catch (IOException ex) {
                Logger.getLogger(EasyRPCClientTCP.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    @Override
    public boolean send(byte[] data) {
        try {
            System.out.println("=> "+data.length);
            dos.write(data);
            dos.flush();
            return true;
        } catch (IOException ex) {
            Logger.getLogger(EasyRPCClientTCP.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }

    @Override
    public DataInputStream receive() {
        try {
            System.out.println("<= "+dis.available());
            return dis;
        } catch (IOException ex) {
            Logger.getLogger(EasyRPCClientTCP.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }
    
}

