package rafaelpm.easyrpc.entities;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Rafael
 */
public class EasyRPCPackage {
    
    public String functionName;
    public DataInfo returnInfo;
    public List<DataInfo> params = new ArrayList<>();
    
    public void setReturnType(TypeData typeData){
        returnInfo = new DataInfo();
        returnInfo.type = typeData.id;
    }
    
    public void setReturnType(int typeData){
        returnInfo = new DataInfo();
        returnInfo.type = typeData;
    }
    
    public void addParam(TypeData typeData, String value){
        DataInfo dataInfo = new DataInfo();
        dataInfo.type = typeData.id;
        dataInfo.value = value;
        params.add(dataInfo);
    }
            
}
