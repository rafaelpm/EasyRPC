package rafaelpm.easyrpc.example1.udp.serial_rpc;

import rafaelpm.easyrpc.server.EasyRPCBaseBindClass;

public class SerialRPC extends EasyRPCBaseBindClass {
    
    public boolean readSerial(byte[] data){
        String hexData = "";
        for(int i=0; i < data.length; i++){
            hexData += String.format("%02x", (data[i] & 0xFF))+" ";
        }
        System.out.println("readSerial: "+data.length+" bytes\n"+hexData.trim());
        System.out.println(new String(data));
        return true;
    }
    
}
