package rafaelpm.easyrpc.entities;

/**
 *
 * @author Rafael
 */
public enum TypeData {
    Void(0,"Void"),
    Integer(1,"Integer");
    
    public int id;
    public String name;

    TypeData(int id, String name){
        this.id = id;
        this.name = name;
    }
    
}
