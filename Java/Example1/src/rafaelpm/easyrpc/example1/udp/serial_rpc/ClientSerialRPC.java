package rafaelpm.easyrpc.example1.udp.serial_rpc;

import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.client.EasyRPCBaseCall;
import rafaelpm.easyrpc.client.udp.EasyRPCClientUDP;
import rafaelpm.easyrpc.entities.TypeData;

/**
 *
 * @author Rafael
 */
public class ClientSerialRPC {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {           
        System.out.println("Starting client UDP");        
        EasyRPCBaseCall easyRPCBaseCall = new EasyRPCBaseCall();
        
        easyRPCBaseCall.connection = new EasyRPCClientUDP("facial.local", 3000);
        
        try {
            boolean isOK = easyRPCBaseCall.setFunctionName("open_serial")
                    .setReturnType(TypeData.Boolean)
                    .setParams("/dev/ttyAMA0", 115200)
                    .send();
            
            if(isOK){
                System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
            }
            
            isOK = easyRPCBaseCall.setFunctionName("send_serial")
                    .setReturnType(TypeData.Boolean)
                    .setParams("ATI9\r\n".getBytes())
                    .send();
            
            if(isOK){
                System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
            }
            
            /*isOK = easyRPCBaseCall.setFunctionName("close_serial")
                    .setReturnType(TypeData.Boolean)                    
                    .send();
            
            if(isOK){
                System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
            }*/
        } catch (Exception ex) {
            Logger.getLogger(ClientSerialRPC.class.getName()).log(Level.SEVERE, null, ex);
            System.exit(0);
        }
    }
        
}
