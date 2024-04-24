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
public class EasyRPCServerParser implements Runnable {
    
    public List<EasyRPCBaseBindClass> classes = new ArrayList<>();        
    public ChannelClientServer channel;
    
    public EasyRPCServerParser(){
        
    }
    
    public void startThread(ChannelClientServer channel){
        EasyRPCServerParser newThread = new EasyRPCServerParser();
        newThread.classes.addAll(classes);
        newThread.channel = channel;
        Thread thread = new Thread(newThread);
        thread.start();
    }
    
    private void onData() {
        BuildPackageFromClient builderFromClient = new BuildPackageFromClient();
        while(builderFromClient.statePackage != StatePackage.Complete){
            try {                
                builderFromClient.setData(channel.receive());
                if(builderFromClient.statePackage == StatePackage.Error){
                    System.out.println("EasyRPCServer Error Package");
                    return;
                }                
            } catch (Exception ex) {
                Logger.getLogger(EasyRPCServerParser.class.getName()).log(Level.SEVERE, null, ex);
                return;
            }
            delay_ms(100);
        }
        
        //System.out.println("Server->ACK");
        BuildPackageToClient buildPackageToClient = new BuildPackageToClient();        
        channel.send(buildPackageToClient.getAck());   
        
        resetTimeout();
        delay_ms(100);
        
        //System.out.println("Server->Answer");
        for(EasyRPCBaseBindClass bindClass: classes){
            try {
                if(bindClass.process(builderFromClient.easyPackage, bindClass)){                    
                    break;
                }
            } catch (Exception ex) {
                builderFromClient = null;
                Logger.getLogger(EasyRPCServerParser.class.getName()).log(Level.SEVERE, null, ex);
                return;            
            }
        }
        
        resetTimeout();
        
        buildPackageToClient = new BuildPackageToClient();
        //System.out.println("EasyRPCServer Sending Answer");
        channel.send(buildPackageToClient.toBytes(builderFromClient.easyPackage));
        //channel.close();
        buildPackageToClient = null;
        
    }
    
    private void delay_ms(int time){
        try{
            Thread.sleep(time);
        }catch(Exception e){}
    }
    
    private long timeoutConnection = 0;
    private long timeout = 5000;
    
    public void setTimeout(long time){
        if(time < 100){
            return;
        }
        timeout = time;
    }
    
    private void resetTimeout(){
        timeoutConnection = System.currentTimeMillis();
    }
    
    private boolean isTimeout(){
        return System.currentTimeMillis() - timeoutConnection >= timeout;
    }

    @Override
    public void run() {
        resetTimeout();        
        while(channel.isConnected()){
            if(channel.hasData()){
                onData();
            }
            delay_ms(100);
            if(isTimeout()){
                //System.out.println("Timeout");
                break;
            }
        }
        channel = null;
        System.gc();
    }
    
}
