package rafaelpm.easyrpc.server;

import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import rafaelpm.easyrpc.debug.Chronometer;
import rafaelpm.easyrpc.debug.DebugControl;
import rafaelpm.easyrpc.packages_build.BuildPackageFromClient;
import rafaelpm.easyrpc.packages_build.BuildPackageToClient;
import rafaelpm.easyrpc.packages_build.StatePackage;
import rafaelpm.easyrpc.server.udp.ChannelClientServerUDP;

/**
 *
 * @author Rafael
 */
public class EasyRPCServerParser implements Runnable {
    
    public List<EasyRPCBaseBindClass> classes = new ArrayList<>();        
    public ChannelClientServer channel;
    private static int serialGlobal = 0;
    private int serial = 0;
    
    public int timeToBuildPackage = 100;
    public int timeAfterSendACK = 100;
        
    public EasyRPCServerParser(){                
        serial = serialGlobal;
        serialGlobal++;
    }
    
    public void configTimesToUDP(){
        timeToBuildPackage = 10;
        timeAfterSendACK = 10;
    }
            
    public void startThread(ChannelClientServer channel){
        EasyRPCServerParser newThread = new EasyRPCServerParser();
        if(channel instanceof ChannelClientServerUDP){
            newThread.configTimesToUDP();
        }
        newThread.classes.addAll(classes);
        newThread.channel = channel;
        Thread thread = new Thread(newThread);
        thread.start();
    }
    
    private void onData() {
        Chronometer chronometer = new Chronometer();
        DebugControl.printDebug(serial+") EasyRPCServerParser->onData");
        BuildPackageFromClient builderFromClient = new BuildPackageFromClient();
        while(builderFromClient.statePackage != StatePackage.Complete){
            try {                
                builderFromClient.setData(channel.receive());
                if(builderFromClient.statePackage == StatePackage.Error){
                    DebugControl.printDebugError(serial+") EasyRPCServerParser->builderFromClient.setData: Error");
                    return;
                }                
            } catch (Exception ex) {
                Logger.getLogger(EasyRPCServerParser.class.getName()).log(Level.SEVERE, null, ex);
                return;
            }
            delay_ms(timeToBuildPackage);
        }
        
        DebugControl.printDebug(serial+") EasyRPCServerParser->Send ACK");        
        BuildPackageToClient buildPackageToClient = new BuildPackageToClient();        
        channel.send(buildPackageToClient.getAck());   
        
        resetTimeout();
        delay_ms(timeAfterSendACK);
        
        DebugControl.printDebug(serial+") EasyRPCServerParser->Find Function");
        for(EasyRPCBaseBindClass bindClass: classes){
            try {
                if(bindClass.process(builderFromClient.easyPackage, bindClass)){                    
                    DebugControl.printDebug(serial+") EasyRPCServerParser->Function found");
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
        DebugControl.printDebug(serial+") EasyRPCServerParser->Send answer function");
        channel.send(buildPackageToClient.toBytes(builderFromClient.easyPackage));
        //channel.close();
        buildPackageToClient = null;
        chronometer.stop("EasyRPCServerParser");
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
                DebugControl.printDebugError("Timeout");
                break;
            }
        }
        channel = null;
        System.gc();
    }
    
}
