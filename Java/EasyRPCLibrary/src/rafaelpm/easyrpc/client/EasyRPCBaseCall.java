package rafaelpm.easyrpc.client;

import rafaelpm.easyrpc.entities.DataInfo;
import rafaelpm.easyrpc.entities.EasyRPCError;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.TypeData;
import rafaelpm.easyrpc.packages_build.BuildPackageFromServer;
import rafaelpm.easyrpc.packages_build.BuildPackageToServer;
import rafaelpm.easyrpc.packages_build.StatePackage;

public class EasyRPCBaseCall {
        
    public EasyRPCPackage easyRPCPackageSend;
    public EasyRPCPackage easyRPCPackageReceived;
    public EasyRPCClientConnection connection;
    /**
     * Delay before read in milliseconds
     */
    public int delayBeforeRead = 0;
    /**
     * Delay between each read (milliseconds)
     */
    private int delayBetweenRead = 5;        
    private int maxEmptyBufferTest = 3;
    
    public EasyRPCBaseCall(){
        easyRPCPackageSend = new EasyRPCPackage();
    }
    
    public boolean send() throws Exception{
        easyRPCPackageReceived = null;
        if(connection == null){
            return false;
        }
        if(!connection.isConnected()){
            for(int i=0; i < 3; i++){
                if(connection.connect()){
                    break;
                }
                if(connection.getDelayToConnect() > 0){
                    delay_ms(connection.getDelayToConnect());
                }
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
        
        BuildPackageFromServer builderFromServerAck = new BuildPackageFromServer();
        builderFromServerAck.setMaxEmptyBufferTest(maxEmptyBufferTest);
        while(builderFromServerAck.statePackage != StatePackage.Complete){            
            delay_ms(delayBetweenRead);
            builderFromServerAck.setData(connection.receive());
            if(builderFromServerAck.statePackage == StatePackage.Error){                
                finishSend();
                return false;
            }
        }
        
        if(delayBeforeRead > 0){
            delay_ms(delayBeforeRead);
        }
                        
        BuildPackageFromServer builderFromServer = new BuildPackageFromServer();
        builderFromServer.setMaxEmptyBufferTest(maxEmptyBufferTest*3);
        while(builderFromServer.statePackage != StatePackage.Complete){            
            delay_ms(delayBetweenRead);
            builderFromServer.setData(connection.receive());
            if(builderFromServer.statePackage == StatePackage.Error){                
                finishSend();
                return false;
            }                
        }
        
        easyRPCPackageReceived = builderFromServer.easyPackage;    
        finishSend();
        return true;
    }
    
    private void finishSend(){
        easyRPCPackageSend = new EasyRPCPackage();        
        //connection.disconnect();
    }
    
    public void delay_ms(int time){
        try{
            Thread.sleep(time);
        }catch(Exception e){}
    }
    
    public EasyRPCBaseCall setMaxEmptyBufferTest(int max){
        if(max < 1){
            return this;
        }
        maxEmptyBufferTest = max;
        return this;
    }
    
    public EasyRPCBaseCall setDelayBeforeRead(int delay){
        delayBeforeRead = delay;
        return this;
    }
    
    public EasyRPCBaseCall setDelayBetweenRead(int delay){
        if(delay <= 0){
            return this;
        }
        delayBetweenRead = delay;
        return this;
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
            if(obj.getClass() == Integer.class || obj.getClass() == int.class){
                dataInfo.type = TypeData.Integer.id;
                dataInfo.value = String.valueOf(obj);
            }else if(obj.getClass() == Float.class || obj.getClass() == float.class){
                dataInfo.type = TypeData.Float.id;
                dataInfo.value = String.valueOf(obj);
            }else if(obj.getClass() == Double.class || obj.getClass() == double.class){
                dataInfo.type = TypeData.Float.id;
                dataInfo.value = String.valueOf(obj);
            }else if(obj.getClass() == Boolean.class || obj.getClass() == boolean.class){
                dataInfo.type = TypeData.Boolean.id;
                dataInfo.value = String.valueOf(obj);
            }else if(obj.getClass() == String.class || obj.getClass() == String.class){
                dataInfo.type = TypeData.String.id;
                dataInfo.value = String.valueOf(obj);
            }else if(obj.getClass() == byte[].class){
                dataInfo.type = TypeData.BinaryArray.id;
                dataInfo.value_bin = (byte[])obj;                
                
            }else{
                throw new Exception(EasyRPCError.TypeNotFound.description);
            }
            easyRPCPackageSend.params.add(dataInfo);
        }
        return this;
    }
        
}
