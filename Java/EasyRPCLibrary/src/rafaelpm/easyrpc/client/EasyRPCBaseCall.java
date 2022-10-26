package rafaelpm.easyrpc.client;

import rafaelpm.easyrpc.entities.DataInfo;
import rafaelpm.easyrpc.entities.EasyRPCError;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.TypeData;
import rafaelpm.easyrpc.packages_build.BuildPackageFromServer;
import rafaelpm.easyrpc.packages_build.BuildPackageToServer;
import rafaelpm.easyrpc.packages_build.StatePackage;

/**
 *
 * @author Rafael
 */
public class EasyRPCBaseCall {
        
    public EasyRPCPackage easyRPCPackageSend;
    public EasyRPCPackage easyRPCPackageReceived;
    public EasyRPCClientConnection connection;
    
    public EasyRPCBaseCall(){
        easyRPCPackageSend = new EasyRPCPackage();
    }
    
    public boolean send() throws Exception{
        if(connection == null){
            return false;
        }
        if(!connection.isConnected()){
            for(int i=0; i < 3; i++){
                if(connection.connect()){
                    break;
                }
                delay_ms(100);
                if(connection.isConnected()){
                    break;
                }                
            }
            if(!connection.isConnected()){
                return false;
            }
        }
        
        BuildPackageToServer builderToServer = new BuildPackageToServer();
        
        if(!connection.send(builderToServer.toBytes(easyRPCPackageSend))){
            return false;
        }
        
        BuildPackageFromServer builderFromServer = new BuildPackageFromServer();
        while(builderFromServer.statePackage != StatePackage.Complete){
            builderFromServer.setData(connection.receive());
            if(builderFromServer.statePackage == StatePackage.Error){
                return false;
            }
        }
        
        return true;
    }
    
    public void delay_ms(int time){
        try{
            Thread.sleep(time);
        }catch(Exception e){}
    }
    
    public EasyRPCBaseCall setFunctionName(String name){
        easyRPCPackageSend.functionName = name;
        return this;
    }
    
    public EasyRPCBaseCall setReturnType(TypeData returnType){
        easyRPCPackageSend.setReturnType(returnType);
        return this;
    }
    
    public EasyRPCBaseCall setParams(Object... args) throws Exception {
        DataInfo dataInfo;
        for(Object obj: args){
            dataInfo = new DataInfo();
            if(obj.getClass() == Integer.class){
                dataInfo.type = TypeData.Integer.id;
                dataInfo.value = String.valueOf(obj);
            }else{
                throw new Exception(EasyRPCError.TypeNotFound.description);
            }
            easyRPCPackageSend.params.add(dataInfo);
        }
        return this;
    }
        
}
