package rafaelpm.easyrpc.packages_build;

/**
 *
 * @author Rafael
 */
public enum StatePackage {
    None(0,"None"),    
    Incomplete(1,"Incomplete"),
    Complete(2,"Complete"),
    Error(3,"Error");
    
    public int id;
    public String description;

    StatePackage(int id, String description){
        this.id = id;
        this.description = description;
    }
    
}
