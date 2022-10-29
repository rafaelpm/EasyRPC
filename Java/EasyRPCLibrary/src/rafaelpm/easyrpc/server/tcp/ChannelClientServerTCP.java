package rafaelpm.easyrpc.server.tcp;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.server.ChannelClientServer;

/**
 *
 * @author Rafael
 */
public class ChannelClientServerTCP extends ChannelClientServer {
    
    private Socket socket;
    private DataOutputStream dos;
    private DataInputStream dis;
    
    public ChannelClientServerTCP(Socket socket){
        this.socket = socket;
        try {
            dos = new DataOutputStream(socket.getOutputStream());
            dis = new DataInputStream(socket.getInputStream());
        } catch (IOException ex) {
            Logger.getLogger(ChannelClientServerTCP.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }

    @Override
    public boolean send(byte[] data) {
        try {
            System.out.println("=> "+data.length+" bytes");
            if(socket == null){
                return false;
            }
            dos.write(data);
            dos.flush();
            return true;
        } catch (IOException ex) {
            Logger.getLogger(ChannelClientServerTCP.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }

    @Override
    public DataInputStream receive() {
        if(socket == null){
            return null;
        }
        try {
            System.out.println("<= "+dis.available()+" bytes");
            return dis;
        } catch (IOException ex) {
            Logger.getLogger(ChannelClientServerTCP.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }

    @Override
    public void close() {
        
        if(dis != null){
            try {
                dis.close();
            } catch (IOException ex) {
                Logger.getLogger(ChannelClientServerTCP.class.getName()).log(Level.SEVERE, null, ex);
            }
            dis = null;
        }
        
        if(dos != null){
            try {
                dos.close();
            } catch (IOException ex) {
                Logger.getLogger(ChannelClientServerTCP.class.getName()).log(Level.SEVERE, null, ex);
            }
            dos = null;
        }
        if(socket != null){
            try {
                socket.close();
                socket = null;
            } catch (IOException ex) {
                Logger.getLogger(ChannelClientServerTCP.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
}
