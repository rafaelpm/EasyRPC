package rafaelpm.easyrpc.debug;

public class Chronometer {
    
    private long begin = System.currentTimeMillis();
    
    public void start(){
        begin = System.currentTimeMillis();
    }
    
    public void stop(String message){
        if(!DebugControl.enableDebug){
            return;
        }
        long time = System.currentTimeMillis() - begin;
        if(time < 1000){
            System.out.println(time+" ms -> "+message);
            return;
        }
        time /= 1000;
        if(time < 60){
            System.out.println(time+" s -> "+message);
            return;
        }
        time /= 60;
        if(time < 60){
            System.out.println(time+" min -> "+message);
            return;
        }
    }
    
}
