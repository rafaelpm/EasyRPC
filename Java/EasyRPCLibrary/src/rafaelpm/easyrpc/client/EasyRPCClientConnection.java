package rafaelpm.easyrpc.client;

import java.io.DataInputStream;

/**
 *
 * @author Rafael
 */
public abstract class EasyRPCClientConnection {
    
    public abstract boolean isConnected();
    public abstract boolean connect();
    public abstract void disconnect();
    public abstract boolean send(byte[] data);
    public abstract DataInputStream receive();
    
    public void printHex(String title, byte[] data){        
        String out = "";
        for(int i=0; i < data.length; i++){
            out += "0x"+String.format("%02X", data[i])+" ";
        }
        System.out.print(title+" "+out);
    }
    
}


