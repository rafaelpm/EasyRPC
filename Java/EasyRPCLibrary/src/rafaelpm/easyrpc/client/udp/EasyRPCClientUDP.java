package rafaelpm.easyrpc.client.udp;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.client.EasyRPCClientConnection;

public class EasyRPCClientUDP extends EasyRPCClientConnection {
    
    private String host = "";
    private int port = 2000;
    
    private InetAddress address = null;
    private DatagramSocket socket;
    
    
    public EasyRPCClientUDP(String host, int port){
        this.host = host;
        this.port = port;
    }

    @Override
    public boolean isConnected() {
        return (socket != null);
    }

    @Override
    public boolean connect() {
        try{
            address = InetAddress.getByName(host);                            
            socket = new DatagramSocket();          
            socket.setSoTimeout(5);
            return true;
        }catch(Exception e){
            e.printStackTrace();
            return false;
        }        
    }
    
    @Override
    public int getDelayToConnect(){
        return 0;
    }

    @Override
    public void disconnect() {
        address = null;
        if(socket != null){
            socket.close();
            socket = null;
        }
    }
    
    private boolean sent = false;

    @Override
    public boolean send(byte[] data) {
        sent = false;
        
        /*Thread thread = new Thread(this);
        thread.start();*/
        try {            
            DatagramPacket request = new DatagramPacket(data, data.length, address, port);
            
            socket.send(request);
            sent = true;
            return true;
        } catch (IOException ex) {
            Logger.getLogger(EasyRPCClientUDP.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
    }

    @Override
    public DataInputStream receive() {
        try{
            byte[] buffer = new byte[4096];
            DatagramPacket response = new DatagramPacket(buffer, buffer.length);
            
            socket.receive(response);            
            byte[] data = new byte[response.getLength()];
            System.arraycopy(buffer, response.getOffset(), data, 0, data.length);            
            return new DataInputStream(new ByteArrayInputStream(data));
        }catch(Exception e){
            return new DataInputStream(new ByteArrayInputStream(new byte[0]));
        }
    }
    
}
