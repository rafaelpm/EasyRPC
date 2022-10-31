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
    protected DataInfo param1 = null, param2 = null, param3 = null, param4 = null, param5 = null;
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
                
        Object res = null;
                
        //Load params
        if(easyRPCPackage.params.size() >= 1){
            param1 = easyRPCPackage.params.get(0);
        }
        if(easyRPCPackage.params.size() >= 2){
            param2 = easyRPCPackage.params.get(1);
        }
        if(easyRPCPackage.params.size() >= 3){
            param3 = easyRPCPackage.params.get(2);
        }
        if(easyRPCPackage.params.size() >= 4){
            param4 = easyRPCPackage.params.get(3);
        }
        if(easyRPCPackage.params.size() >= 5){
            param5 = easyRPCPackage.params.get(4);
        }
        
        switch(easyRPCPackage.params.size()){
            case 1:                
                res = method.invoke(objClass, param1.getValue());
                break;
            case 2:
                res = method.invoke(objClass, param1.getValue(), param2.getValue());
                break;
            case 3:                
                res = method.invoke(objClass, param1.getValue(), param2.getValue(), param3.getValue());
                break;
            case 4:                
                res = method.invoke(objClass, param1.getValue(), param2.getValue(), param3.getValue(), param4.getValue());
                break;
            case 5:                
                res = method.invoke(objClass, param1.getValue(), param2.getValue(), param3.getValue(), param4.getValue(), param5.getValue());
                break;
        }
        
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
