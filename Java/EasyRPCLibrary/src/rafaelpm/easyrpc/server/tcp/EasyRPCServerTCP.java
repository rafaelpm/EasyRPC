package rafaelpm.easyrpc.server.tcp;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.server.EasyRPCBaseBindClass;
import rafaelpm.easyrpc.server.EasyRPCServer;
import rafaelpm.easyrpc.server.EasyRPCServerConnection;
import rafaelpm.easyrpc.server.ReceiveDataFromClient;

/**
 *
 * @author Rafael
 */
public class EasyRPCServerTCP extends EasyRPCServerConnection implements Runnable {
    
    private ServerSocket socket;
    public ReceiveDataFromClient receiveDataFromClient;
    public EasyRPCServer easyRPCServer;
    
    public EasyRPCServerTCP(){
        
    }
    
    public static EasyRPCServerTCP build(){
        EasyRPCServerTCP easyRPCServerTCP = new EasyRPCServerTCP();
        return easyRPCServerTCP;
    }
    
    public EasyRPCServerTCP setPort(int port) throws IOException {
        socket = new ServerSocket(port);
        easyRPCServer = new EasyRPCServer();
        easyRPCServer.connection = this;
        receiveDataFromClient = easyRPCServer;
        return this;
    }
    
    public EasyRPCServerTCP addClass(EasyRPCBaseBindClass classObj) {
        easyRPCServer.classes.add(classObj);
        return this;
    }
    
    public EasyRPCServer finish(){
        listen();
        return easyRPCServer;
    }

    @Override
    public void listen() {
        Thread thread = new Thread(this);
        thread.start();
    }

    @Override
    public void close() {
        if(!socket.isClosed()){
            try {
                socket.close();
            } catch (IOException ex) {
                Logger.getLogger(EasyRPCServerTCP.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    
    @Override
    public void run(){
        System.out.println("EasyRPCServer TCP started at port "+socket.getLocalPort());
        Socket newClient;
        while(!socket.isClosed()){
            try {                
                newClient = socket.accept();
                System.out.println("New client: "+newClient.getInetAddress().toString());
                if(receiveDataFromClient != null){
                    ChannelClientServerTCP channelClientServerTCP = new ChannelClientServerTCP(newClient);
                    receiveDataFromClient.onData(channelClientServerTCP);
                }            
            } catch (IOException ex) {
                Logger.getLogger(EasyRPCServerTCP.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
}
