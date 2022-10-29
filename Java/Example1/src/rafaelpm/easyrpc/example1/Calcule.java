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
    
    public int subtract(int a, int b){
        return a-b;
    }
}
