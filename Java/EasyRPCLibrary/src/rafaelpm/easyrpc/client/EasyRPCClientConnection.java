package rafaelpm.easyrpc.client;

import java.io.DataInputStream;

/**
 *
 * @author Rafael
 */
public abstract class EasyRPCClientConnection {
    
    public abstract boolean isConnected();
    public abstract boolean connect();
    public abstract void disconnect();
    public abstract boolean send(byte[] data);
    public abstract DataInputStream receive();
    
}


