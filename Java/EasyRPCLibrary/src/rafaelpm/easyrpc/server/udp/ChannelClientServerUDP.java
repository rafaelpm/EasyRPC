package rafaelpm.easyrpc.server.udp;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.server.ChannelClientServer;

public class ChannelClientServerUDP extends ChannelClientServer {
    
    private String host;
    private int port;    
    private DatagramSocket socket;
    private ByteArrayOutputStream baos = new ByteArrayOutputStream();
    private EasyRPCServerUDP server;
    
    public ChannelClientServerUDP(EasyRPCServerUDP server, DatagramPacket packet){
        try {
            this.server = server;
            setHost(packet.getAddress().toString());
            this.port = packet.getPort();           
            addData(packet);
        } catch (Exception ex) {
            Logger.getLogger(ChannelClientServerUDP.class.getName()).log(Level.SEVERE, null, ex);
        }
        startSocket();
    }
    
    private void setHost(String value){
        this.host = value;
        if(host.startsWith("/")){
            host = host.replace("/", "").trim();
        }
    }
        
    public void addData(DatagramPacket packet){
        try {                        
            baos.write(packet.getData(), packet.getOffset(), packet.getLength());
        } catch (Exception ex) {
            Logger.getLogger(ChannelClientServerUDP.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public boolean isEqual(DatagramPacket packet){
        if(!packet.getAddress().toString().equals(host)){
            return false;
        }
        return true;
    }
    
    public boolean isEqual(ChannelClientServerUDP client){
        if(!client.host.equals(host)){
            return false;
        }
        return true;
    }
    
    private boolean startSocket(){
        if(socket != null){
            return true;
        }
        try {
            socket = new DatagramSocket();            
            return true;
        } catch (Exception ex) {            
            Logger.getLogger(ChannelClientServerUDP.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
    }
    
    @Override
    public boolean send(byte[] data) {
        if(!startSocket()){
            return false;
        }
        try {
            InetAddress address = InetAddress.getByName(host);
            DatagramPacket datagram = new DatagramPacket(data, data.length, address, port); 
            socket.send(datagram);
            return true;
        } catch (Exception ex) {
            Logger.getLogger(ChannelClientServerUDP.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }

    @Override
    public boolean hasData() {
        if(baos == null){
            return false;
        }
        return baos.size() > 0;
    }

    @Override
    public DataInputStream receive() {
        if(baos == null){
            return new DataInputStream(new ByteArrayInputStream(new byte[0]));
        }
        byte[] buffer = baos.toByteArray();
        baos.reset(); 
        baos = null;
        baos = new ByteArrayOutputStream();
        return new DataInputStream(new ByteArrayInputStream(buffer));
    }

    @Override
    public void close() {
        if(socket != null){
            socket.close();
            socket = null;
        }
        if(server != null){
            server.removeClient(this);
        }
    }

    @Override
    public boolean isConnected() {
        return socket != null;
    }
    
}
