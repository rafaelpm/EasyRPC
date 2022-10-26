package rafaelpm.easyrpc.server;

import java.io.DataInputStream;

/**
 *
 * @author Rafael
 */
public abstract class EasyRPCServerConnection  {
        
    public abstract void listen();
    public abstract void close();
    
    
}
