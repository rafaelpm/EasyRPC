package rafaelpm.easyrpc.server.tcp;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.server.EasyRPCServerConnection;
import rafaelpm.easyrpc.server.ReceiveDataFromClient;
import sun.audio.AudioPlayer;

/**
 *
 * @author Rafael
 */
public class EasyRPCServerTCP extends EasyRPCServerConnection implements Runnable {
    
    private ServerSocket socket;
    public ReceiveDataFromClient receiveDataFromClient;
    
    public EasyRPCServerTCP(int port) throws IOException {
        socket = new ServerSocket(port);
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
        
        try {
            Socket newClient;
            newClient = socket.accept();            
            if(receiveDataFromClient != null){
                ChannelClientServerTCP channelClientServerTCP = new ChannelClientServerTCP(newClient);
                receiveDataFromClient.onData(channelClientServerTCP);
            }            
        } catch (IOException ex) {
            Logger.getLogger(EasyRPCServerTCP.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
}
