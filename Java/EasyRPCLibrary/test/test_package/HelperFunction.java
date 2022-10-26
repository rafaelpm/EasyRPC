package test_package;


import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.TypeData;


/**
 *
 * @author Rafael
 */
public class HelperFunction {
    
    public static EasyRPCPackage buildSum(int a, int b){
        EasyRPCPackage easyRPCPackage = new EasyRPCPackage();
        easyRPCPackage.functionName = "sum";
        easyRPCPackage.setReturnType(TypeData.Integer);
        easyRPCPackage.addParam(TypeData.Integer, String.valueOf(a));
        easyRPCPackage.addParam(TypeData.Integer, String.valueOf(b));
        return easyRPCPackage;
    }
    
    public static EasyRPCPackage buildSumResult(int result){
        EasyRPCPackage easyRPCPackage = new EasyRPCPackage();
        easyRPCPackage.functionName = "sum";
        easyRPCPackage.setReturnType(TypeData.Integer);
        easyRPCPackage.returnInfo.value = String.valueOf(result);
        return easyRPCPackage;
    }
    
}
