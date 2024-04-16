package rafaelpm.easyrpc.packages_build;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import rafaelpm.easyrpc.entities.DataInfo;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.EasyRPCPackageBase;
import rafaelpm.easyrpc.entities.TypeData;

/**
 *
 * @author Rafael
 */
public class BuildPackageToServer extends EasyRPCPackageBase {
    
    public byte[] toBytes(EasyRPCPackage easyPackage){
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        try{
            setString(easyPackage.functionName, dos);
            dos.writeByte(easyPackage.returnInfo.type);
            
            //Total Params
            dos.writeByte(easyPackage.params.size());
            
            for(DataInfo dataInfo: easyPackage.params){
                dos.writeByte(dataInfo.type);
                if(dataInfo.type == TypeData.BinaryArray.id){
                    setBinary(dataInfo.value_bin, dos);
                }else if(dataInfo.type != TypeData.Void.id){
                    setString(dataInfo.value, dos);
                }
            }
            
        }catch(Exception e){
            e.printStackTrace();
        }
        return wrapData(baos.toByteArray());
    }
    
}
