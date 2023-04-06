package rafaelpm.easyrpc.example1;

import rafaelpm.easyrpc.client.EasyRPCBaseCall;
import rafaelpm.easyrpc.client.tcp.EasyRPCClientTCP;
import rafaelpm.easyrpc.entities.TypeData;

/**
 *
 * @author Rafael
 */
public class Temp {
    
    public static String host = "192.168.10.8";//"127.0.0.1";
    public static int port = 2000;
    
    public static void main(String[] args) {           
        System.out.println("Starting client");   
        //subtract();
        sum();
        //printMessage();
        //binaryData();
    }
    
    public static void sum(){
        EasyRPCBaseCall easyRPCBaseCall = new EasyRPCBaseCall();
        easyRPCBaseCall.connection = new EasyRPCClientTCP(host, port);
        try{
            boolean isOK = easyRPCBaseCall.setFunctionName("sum")
                    .setReturnType(TypeData.Integer)
                    .setParams(2, 3)
                    .send();
            
            if(isOK){
                System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
            }else{
                System.out.println("Error");
            }
        }catch(Exception e){
            
        }
    }
    
    public static void printMessage(){
        EasyRPCBaseCall easyRPCBaseCall = new EasyRPCBaseCall();
        easyRPCBaseCall.connection = new EasyRPCClientTCP(host, port);
        try{
            boolean isOK = easyRPCBaseCall.setFunctionName("print")
                    .setReturnType(TypeData.Void)
                    .setParams("Hello server!")
                    .send();
            if(isOK){
                System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
            }else{
                System.out.println("Error");
            }
        }catch(Exception e){
            
        }
    }
    
    public static void binaryData(){
        EasyRPCBaseCall easyRPCBaseCall = new EasyRPCBaseCall();
        easyRPCBaseCall.connection = new EasyRPCClientTCP(host, port);
        try{
            boolean isOK = easyRPCBaseCall.setFunctionName("binaryData")
                    .setReturnType(TypeData.Void)
                    .setParams(new byte[]{0x0A,0x0B,0x0C,0x0D})
                    .send();
            
            if(isOK){
                System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
            }else{
                System.out.println("Error");
            }
        }catch(Exception e){
            
        }
    }
    
    
    
    public static void subtract(){
        EasyRPCBaseCall easyRPCBaseCall = new EasyRPCBaseCall();
        easyRPCBaseCall.connection = new EasyRPCClientTCP(host, port);
        try{
            boolean isOK = easyRPCBaseCall.setFunctionName("subtract")
                    .setReturnType(TypeData.Float)
                    .setParams(9.5, 1.5)
                    .send();
            
            if(isOK){
                System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
            }else{
                System.out.println("Error");
            }
        }catch(Exception e){
            
        }
    }
    
    
}
