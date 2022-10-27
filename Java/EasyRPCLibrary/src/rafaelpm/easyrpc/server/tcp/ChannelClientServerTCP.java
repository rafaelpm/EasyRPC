package rafaelpm.easyrpc.server.tcp;

import java.io.DataInputStream;
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
    
    public ChannelClientServerTCP(Socket socket){
        this.socket = socket;
    }

    @Override
    public boolean send(byte[] data) {
        try {
            System.out.println("=> "+data.length+" bytes");
            socket.getOutputStream().write(data);
            return true;
        } catch (IOException ex) {
            Logger.getLogger(ChannelClientServerTCP.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }

    @Override
    public DataInputStream receive() {
        try {
            System.out.println("<= "+socket.getInputStream().available()+" bytes");
            return new DataInputStream(socket.getInputStream());
        } catch (IOException ex) {
            Logger.getLogger(ChannelClientServerTCP.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }
    
}
