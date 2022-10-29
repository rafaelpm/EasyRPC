package rafaelpm.easyrpc.server;

import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.packages_build.BuildPackageFromClient;
import rafaelpm.easyrpc.packages_build.BuildPackageToClient;
import rafaelpm.easyrpc.packages_build.StatePackage;

/**
 *
 * @author Rafael
 */
public class EasyRPCServer implements ReceiveDataFromClient {
    
    public List<EasyRPCBaseBindClass> classes = new ArrayList<>();
    public EasyRPCServerConnection connection;

    @Override
    public void onData(ChannelClientServer channel) {
        BuildPackageFromClient builderFromClient = new BuildPackageFromClient();
        while(builderFromClient.statePackage != StatePackage.Complete){
            try {
                delay_ms(100);
                builderFromClient.setData(channel.receive());
                if(builderFromClient.statePackage == StatePackage.Error){
                    System.out.println("EasyRPCServer Error Package");
                    return;
                }
            } catch (Exception ex) {
                Logger.getLogger(EasyRPCServer.class.getName()).log(Level.SEVERE, null, ex);
                return;
            }
        }
        
        for(EasyRPCBaseBindClass bindClass: classes){
            try {
                if(bindClass.process(builderFromClient.easyPackage, bindClass)){
                    System.out.println("EasyRPCServer Process OK");
                    break;
                }
            } catch (Exception ex) {
                Logger.getLogger(EasyRPCServer.class.getName()).log(Level.SEVERE, null, ex);
                return;            
            }
        }
        
        BuildPackageToClient buildPackageToClient = new BuildPackageToClient();
        System.out.println("EasyRPCServer Sending Answer");
        channel.send(buildPackageToClient.toBytes(builderFromClient.easyPackage));
        channel.close();
    }
    
    public void delay_ms(int time){
        try{
            Thread.sleep(time);
        }catch(Exception e){}
    }
    
}
