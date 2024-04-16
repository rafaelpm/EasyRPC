package rafaelpm.easyrpc.example1.udp;

import rafaelpm.easyrpc.example1.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.server.tcp.EasyRPCServerTCP;
import rafaelpm.easyrpc.server.udp.EasyRPCServerUDP;

/**
 *
 * @author Rafael
 */
public class Server {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("Starting server UDP");
        try {
            EasyRPCServerUDP.build(2000, false)                    
                    .addClass(new Calcule())
                    .finish();            
        } catch (Exception ex) {
            Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
            System.exit(0);
        }
    }
    
}
