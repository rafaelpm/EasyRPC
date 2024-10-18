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
    private int forceResponsePort = 0;
    public static boolean printDebug = false;
        
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
    
    public void setForceResponsePort(int port){
        this.forceResponsePort = port;
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
        /*if(socket != null){
            return true;
        }*/
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
        if(printDebug){            
            if(forceResponsePort != 0){
                System.out.println("SendUDP ("+host+":"+forceResponsePort+"): "+data.length+" bytes");
            }else{
                System.out.println("SendUDP ("+host+":"+port+"): "+data.length+" bytes");
            }
        }
        try {
            InetAddress address = InetAddress.getByName(host);
            DatagramPacket datagram;
            if(forceResponsePort != 0){
                datagram = new DatagramPacket(data, data.length, address, forceResponsePort); 
            }else{
                datagram = new DatagramPacket(data, data.length, address, port); 
            }
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
    
    private void printHex(String title, byte[] data){
        String lastCommand = "";
        for(int i=0; i < data.length; i++){
            lastCommand += String.format("%02d ", 0xFF & (int)data[i]);
        }
        System.out.println(title+lastCommand);
    }
    
}
