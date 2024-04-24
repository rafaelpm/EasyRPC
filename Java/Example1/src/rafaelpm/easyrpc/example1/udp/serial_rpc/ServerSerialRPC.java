package rafaelpm.easyrpc.example1.udp.serial_rpc;

import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.server.udp.EasyRPCServerUDP;

/**
 *
 * @author Rafael
 */
public class ServerSerialRPC {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("Starting server UDP");
        try {
            EasyRPCServerUDP.build(3000, false)                    
                    .addClass(new SerialRPC())
                    .finish();            
        } catch (Exception ex) {
            Logger.getLogger(ServerSerialRPC.class.getName()).log(Level.SEVERE, null, ex);
            System.exit(0);
        }
    }
    
}
