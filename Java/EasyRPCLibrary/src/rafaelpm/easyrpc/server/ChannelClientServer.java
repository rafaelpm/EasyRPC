package rafaelpm.easyrpc.server;

import java.io.DataInputStream;

/**
 *
 * @author Rafael
 */
public abstract class ChannelClientServer {
    public abstract boolean send(byte[] data);
    public abstract DataInputStream receive();
}
