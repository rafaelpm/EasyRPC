package rafaelpm.easyrpc.server;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import rafaelpm.easyrpc.entities.DataInfo;
import rafaelpm.easyrpc.entities.EasyRPCError;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.TypeData;

/**
 *
 * @author Rafael
 */
public abstract class EasyRPCBaseBindClass {
    
    protected EasyRPCPackage answer;
    protected Method method;
    
    public final int maxParams = 5;
    
    public EasyRPCBaseBindClass() throws Exception {
        testMaxParams();
    }
    
    private void testMaxParams() throws Exception {
        Method[] methods = this.getClass().getDeclaredMethods();
        for(Method m: methods){
            if(m.getParameterCount() > maxParams){
                throw new Exception(String.format(EasyRPCError.LimitParams.description, maxParams));
            }
        }
    }
        
    public boolean process(EasyRPCPackage easyRPCPackage, Object objClass) throws IllegalAccessException, IllegalArgumentException, InvocationTargetException{
        method = hasMethod(easyRPCPackage.functionName);
        if(method == null){
            return false;
        }
        answer = new EasyRPCPackage();
        answer.setReturnType(easyRPCPackage.returnInfo.type);
        
        UglySetParam ugly = new UglySetParam();
                
        Object res = ugly.setParam(easyRPCPackage, objClass, method);                
        
        if(easyRPCPackage.returnInfo.type != TypeData.Void.id && res != null){            
            easyRPCPackage.returnInfo.value = res.toString();
        }
        
        return true;        
    }
            
    public Method hasMethod(String methodName){
        Method[] methods = this.getClass().getDeclaredMethods();
        for(Method m: methods){
            if(m.toString().contains("hasMethod")){
                continue;
            }
            //System.out.println(m.toString());
            if(m.toString().contains(methodName)){             
                return m;
            }
        }
        return null;
    }
        
}
