package rafaelpm.easyrpc.server.tcp;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.server.EasyRPCBaseBindClass;
import rafaelpm.easyrpc.server.EasyRPCServerParser;
import rafaelpm.easyrpc.server.EasyRPCServerConnection;

/**
 *
 * @author Rafael
 */
public class EasyRPCServerTCP extends EasyRPCServerConnection implements Runnable {
    
    private ServerSocket socket;    
    public EasyRPCServerParser easyRPCServerMatriz;
    
    public EasyRPCServerTCP(){
        
    }
    
    public static EasyRPCServerTCP build(){
        EasyRPCServerTCP easyRPCServerTCP = new EasyRPCServerTCP();
        return easyRPCServerTCP;
    }
    
    public EasyRPCServerTCP setPort(int port) throws IOException {
        socket = new ServerSocket(port);
        socket.setReceiveBufferSize(65000);
        if(easyRPCServerMatriz == null){
            easyRPCServerMatriz = new EasyRPCServerParser();
        }        
        return this;
    }
    
    public EasyRPCServerTCP addClass(EasyRPCBaseBindClass classObj) {
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
            socket = null;
        }
    }

    
    @Override
    public void run(){
        System.out.println("EasyRPCServer TCP started at port "+socket.getLocalPort());
        Socket newClient;
        while(!socket.isClosed()){
            try {                
                newClient = socket.accept();
                System.out.println("New client: "+newClient.getInetAddress().toString()+":"+newClient.getPort());
                ChannelClientServerTCP channelClientServerTCP = new ChannelClientServerTCP(newClient);
                easyRPCServerMatriz.startThread(channelClientServerTCP);
                          
            } catch (IOException ex) {
                Logger.getLogger(EasyRPCServerTCP.class.getName()).log(Level.SEVERE, null, ex);
            }
            if(socket == null){
                break;
            }
        }
    }
    
}
