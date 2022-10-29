package rafaelpm.easyrpc.example1;

import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.server.tcp.EasyRPCServerTCP;

/**
 *
 * @author Rafael
 */
public class Server {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("Starting server");
        try {
            EasyRPCServerTCP.build()
                    .setPort(2000)
                    .addClass(new Calcule())
                    .finish();            
        } catch (Exception ex) {
            Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
            System.exit(0);
        }
    }
    
}
