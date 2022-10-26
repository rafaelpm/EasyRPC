package rafaelpm.easyrpc.packages_build;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import rafaelpm.easyrpc.entities.DataInfo;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.EasyRPCPackageBase;
import rafaelpm.easyrpc.entities.EasyRPCError;

/**
 *
 * @author Rafael
 */
public class BuildPackageFromClient extends EasyRPCPackageBase {
    
    public EasyRPCPackage easyPackage = new EasyRPCPackage();
    
    @Override
    protected boolean processData(DataInputStream dis) throws Exception {
        easyPackage.functionName = getString(dis);
        if(easyPackage.functionName.isEmpty()){
            throw new Exception(EasyRPCError.NameFunctionEmpty.description);                            
        }

        easyPackage.returnInfo = new DataInfo();
        easyPackage.returnInfo.type = dis.readByte();

        DataInfo dataInfo;
        while(dis.available() > 0){
            dataInfo = new DataInfo();
            dataInfo.type = dis.readByte();
            dataInfo.value = getString(dis);
            easyPackage.params.add(dataInfo);
        }
        return true;
    }
    
    
}
