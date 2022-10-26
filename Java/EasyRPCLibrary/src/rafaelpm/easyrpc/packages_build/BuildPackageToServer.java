package rafaelpm.easyrpc.packages_build;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import rafaelpm.easyrpc.entities.DataInfo;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.EasyRPCPackageBase;

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
            
            for(DataInfo dataInfo: easyPackage.params){
                dos.writeByte(dataInfo.type);
                setString(dataInfo.value, dos);
            }
            
        }catch(Exception e){
            e.printStackTrace();
        }
        return wrapData(baos.toByteArray());
    }
    
}
