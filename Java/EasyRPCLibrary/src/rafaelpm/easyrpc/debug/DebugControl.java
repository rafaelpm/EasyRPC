package rafaelpm.easyrpc.debug;

public class DebugControl {
    
    public static boolean enableDebug = true;
    
    public static void printDebug(String message){
        if(!enableDebug){
            return;
        }
        System.out.println(message);
    }
    
    public static void printDebugError(String message){
        if(!enableDebug){
            return;
        }
        System.err.println("******************");
        System.err.println(message);
        System.err.println("******************");
    }
    
}
