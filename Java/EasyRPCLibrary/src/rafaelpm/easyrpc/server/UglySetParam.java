package rafaelpm.easyrpc.server;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import rafaelpm.easyrpc.entities.DataInfo;
import rafaelpm.easyrpc.entities.EasyRPCPackage;

/**
 * Help me do it better
 * @author Rafael
 */
public class UglySetParam {
    
    protected DataInfo param1 = null, param2 = null, param3 = null, param4 = null, param5 = null;
    
    public Object setParam(EasyRPCPackage easyRPCPackage, Object objClass, Method method) throws IllegalAccessException, IllegalArgumentException, InvocationTargetException {
        Object res = null;
        
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
        return res;
    }
    
}
