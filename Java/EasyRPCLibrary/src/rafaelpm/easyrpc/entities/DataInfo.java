package rafaelpm.easyrpc.entities;

/**
 *
 * @author Rafael
 */
public class DataInfo {
    
    public String value;
    public int type;
    
    public Object getValue(){
        if(type == TypeData.Integer.id){
            return Integer.parseInt(value);
        }
        return value;
    }
    
}
