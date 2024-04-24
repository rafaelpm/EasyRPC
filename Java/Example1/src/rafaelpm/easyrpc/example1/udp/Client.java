package rafaelpm.easyrpc.example1.udp;

import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.client.EasyRPCBaseCall;
import rafaelpm.easyrpc.client.udp.EasyRPCClientUDP;
import rafaelpm.easyrpc.entities.TypeData;

/**
 *
 * @author Rafael
 */
public class Client {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {           
        System.out.println("Starting client UDP");        
        EasyRPCBaseCall easyRPCBaseCall = new EasyRPCBaseCall();
        //easyRPCBaseCall.connection = new EasyRPCClientUDP("127.0.0.1", 2000);
        easyRPCBaseCall.connection = new EasyRPCClientUDP("facial.local", 2000);
        //Linux
        //easyRPCBaseCall.connection = new EasyRPCClientUDP("192.168.15.2", 2000);
        try {
            boolean isOK = easyRPCBaseCall.setFunctionName("sum")
                    .setReturnType(TypeData.Integer)
                    .setParams(2, 3)
                    .send();
            
            if(isOK){
                System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
            }
                        
            isOK = easyRPCBaseCall.setFunctionName("subtract")
                    .setReturnType(TypeData.Float)
                    .setParams(9.5, 1.5)
                    .send();
            
            if(isOK){
                System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
            }
            
            isOK = easyRPCBaseCall.setFunctionName("print")
                    .setReturnType(TypeData.Void)
                    .setParams("Hello server!")
                    .send();
            
            isOK = easyRPCBaseCall.setFunctionName("binaryData")
                    .setReturnType(TypeData.Void)
                    .setParams(new byte[]{0x0A,0x0B,0x0C,0x0D})
                    .send();
            
            
        } catch (Exception ex) {
            Logger.getLogger(Client.class.getName()).log(Level.SEVERE, null, ex);
            System.exit(0);
        }
    }
        
}
