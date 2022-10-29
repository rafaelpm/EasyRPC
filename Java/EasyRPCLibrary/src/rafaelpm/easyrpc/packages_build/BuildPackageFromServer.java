package rafaelpm.easyrpc.packages_build;

import java.io.DataInputStream;
import rafaelpm.easyrpc.entities.DataInfo;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.EasyRPCPackageBase;
import rafaelpm.easyrpc.entities.EasyRPCError;
import rafaelpm.easyrpc.entities.TypeData;

/**
 *
 * @author Rafael
 */
public class BuildPackageFromServer extends EasyRPCPackageBase {
    
    public EasyRPCPackage easyPackage = new EasyRPCPackage();
    
    @Override
    protected boolean processData(DataInputStream dis) throws Exception {
        easyPackage.functionName = getString(dis);
        if(easyPackage.functionName.isEmpty()){
            throw new Exception(EasyRPCError.NameFunctionEmpty.description);                            
        }

        easyPackage.returnInfo = new DataInfo();
        easyPackage.returnInfo.type = dis.readByte();
        if(easyPackage.returnInfo.type == TypeData.BinaryArray.id){
            easyPackage.returnInfo.value_bin = getBinary(dis);
        }else{
            easyPackage.returnInfo.value = getString(dis);
        }

        return true;
    }
    
    
}
