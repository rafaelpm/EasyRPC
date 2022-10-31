package rafaelpm.easyrpc.example1;

import rafaelpm.easyrpc.server.EasyRPCBaseBindClass;

/**
 *
 * @author Rafael
 */
public class Calcule extends EasyRPCBaseBindClass {
    
    public Calcule() throws Exception {
        
    }
    
    public int sum(int a, int b){
        return a+b;
    }
    
    public float subtract(float a, float b){
        return a-b;
    }
    
    public void print(String message){
        System.out.println("Message from client: "+message);
    }
    
    public void binaryData(byte[] data){
        String hexData = "";
        for(int i=0; i < data.length; i++){
            hexData += String.format("%02x", (data[i] & 0xFF))+" ";
        }
        System.out.println("Received binary data: "+data.length+" bytes\n"+hexData.trim());
    }
    
}
