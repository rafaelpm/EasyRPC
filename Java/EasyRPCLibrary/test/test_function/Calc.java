package test_function;

import rafaelpm.easyrpc.server.EasyRPCBaseBindClass;

/**
 *
 * @author Rafael
 */
public class Calc extends EasyRPCBaseBindClass {
    
    public Calc() throws Exception {
        
    }
    
    public int sum(int a, int b){
        return a+b;
    }
    
}
