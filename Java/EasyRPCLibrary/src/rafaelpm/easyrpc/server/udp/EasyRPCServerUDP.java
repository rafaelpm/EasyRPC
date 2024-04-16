package rafaelpm.easyrpc.server.udp;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.server.EasyRPCBaseBindClass;
import rafaelpm.easyrpc.server.EasyRPCServerConnection;
import rafaelpm.easyrpc.server.EasyRPCServerParser;

public class EasyRPCServerUDP extends EasyRPCServerConnection implements Runnable {
    
    private DatagramSocket server = null;
    private boolean enableBroadcast = false;
    private int port;
    private List<ChannelClientServerUDP> clients = new ArrayList<>();
    public EasyRPCServerParser easyRPCServerMatriz;
    
    public EasyRPCServerUDP(int port, boolean enableBroadcast){
        this.port = port;
        this.enableBroadcast = enableBroadcast;
    }
    
    public static EasyRPCServerUDP build(int port, boolean enableBroadcast){
        EasyRPCServerUDP easyRPCServer = new EasyRPCServerUDP(port, enableBroadcast);
        return easyRPCServer;
    }
    
    public EasyRPCServerUDP addClass(EasyRPCBaseBindClass classObj) {
        if(easyRPCServerMatriz == null){
            easyRPCServerMatriz = new EasyRPCServerParser();
        }
        easyRPCServerMatriz.classes.add(classObj);
        return this;
    }
    
    public EasyRPCServerParser finish(){
        listen();
        return easyRPCServerMatriz;
    }

    @Override
    public void listen() {
        try{
            server = new DatagramSocket(port);
            server.setBroadcast(enableBroadcast);
            
            Thread thread = new Thread(this);
            thread.start();
        }catch(Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public void close() {
        if(server != null){
            server.close();
            server = null;
        }
    }

    @Override
    public void run() {
        if(server == null){
            return;
        }
        byte[] buffer = new byte[4096];
        while(server != null){
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
            try {
                //Aguarda receber
                server.receive(packet);
                addClient(packet);
                /*ChannelClientServerUDP channelClientServerUDP = new ChannelClientServerUDP(packet);
                easyRPCServerMatriz.startThread(channelClientServerUDP);*/
            } catch (IOException ex) {
                Logger.getLogger(EasyRPCServerUDP.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
    
    private void addClient(DatagramPacket packet){
        for(ChannelClientServerUDP client: clients){
            if(client.isEqual(packet)){
                client.addData(packet);
                return;
            }
        }
        
        //Not found
        ChannelClientServerUDP channelClientServerUDP = new ChannelClientServerUDP(this, packet);        
        easyRPCServerMatriz.startThread(channelClientServerUDP);
        clients.add(channelClientServerUDP);        
    }
    
    protected void removeClient(ChannelClientServerUDP channelClientServerUDP){
        
        for(int i=0; i < clients.size(); i++){
            ChannelClientServerUDP client = clients.get(i);
            if(client.isEqual(channelClientServerUDP)){
                clients.remove(i);
                i -= 1;
            }
        }
    }
}
