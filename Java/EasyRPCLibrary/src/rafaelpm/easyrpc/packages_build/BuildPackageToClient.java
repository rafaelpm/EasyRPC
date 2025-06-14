package rafaelpm.easyrpc.packages_build;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import rafaelpm.easyrpc.entities.DataInfo;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.EasyRPCPackageBase;
import rafaelpm.easyrpc.entities.TypeData;

/**
 * Result of function
 * @author Rafael
 */
public class BuildPackageToClient extends EasyRPCPackageBase {
    
    public byte[] getAck(){
        reserved = 0x01;
        return wrapData(new byte[0]);
    }
    
    
    public byte[] toBytes(EasyRPCPackage easyPackage){
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        try{
            setString(easyPackage.functionName, dos);
            dos.writeByte(easyPackage.returnInfo.type); 
            if(easyPackage.returnInfo.type == TypeData.BinaryArray.id){
                setBinary(easyPackage.returnInfo.value_bin, dos);
            }else if(easyPackage.returnInfo.type != TypeData.Void.id){
                if(easyPackage.returnInfo.value != null){
                    setString(easyPackage.returnInfo.value, dos);
                }else{
                    setString("", dos);
                }
            }
        }catch(Exception e){
            e.printStackTrace();
        }
        return wrapData(baos.toByteArray());
    }
    
}
