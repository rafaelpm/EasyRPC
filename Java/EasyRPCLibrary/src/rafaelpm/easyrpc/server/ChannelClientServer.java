package rafaelpm.easyrpc.server;

import java.io.DataInputStream;

/**
 *
 * @author Rafael
 */
public abstract class ChannelClientServer {
    public abstract boolean send(byte[] data);
    public abstract boolean hasData();
    public abstract DataInputStream receive();
    public abstract void close();
    public abstract boolean isConnected();
}
