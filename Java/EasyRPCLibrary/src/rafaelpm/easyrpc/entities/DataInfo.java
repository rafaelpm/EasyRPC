package rafaelpm.easyrpc.entities;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

/**
 *
 * @author Rafael
 */
public class DataInfo {
    
    public String value;
    public byte[] value_bin;
    public int type;
    
    public Object getValue(){
        if(type == TypeData.Integer.id){
            return Integer.parseInt(value);
        }else if(type == TypeData.Float.id){
            return Float.parseFloat(value);
        }else if(type == TypeData.Boolean.id){
            return Boolean.parseBoolean(value);
        }else if(type == TypeData.BinaryArray.id){
            return value_bin;
        }
        return value;
    }
    
    protected byte[] getBinary(DataInputStream dis) throws IOException{
        int size = dis.readShort();
        byte[] buffer = new byte[size];
        dis.read(buffer);
        return buffer;
    }
    
    protected void setBinary(byte[] buffer, DataOutputStream dos) throws IOException{
        dos.writeShort(buffer.length);
        dos.write(buffer);
    }
    
}
