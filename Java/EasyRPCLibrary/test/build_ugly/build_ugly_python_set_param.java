package build_ugly;

import org.junit.Test;

/**
 *
 * @author Rafael
 */
public class build_ugly_python_set_param {
    
    @Test
    public void build(){
        final int total = 50;
        String content = "", params="";
        for(int total_params=0; total_params < total; total_params++){
            params="";
            for(int p=0; p < total_params; p++){
                if(!params.isEmpty()){
                    params += ",";
                }
                params += "params["+p+"].get_value_param()";
            }
            if(!content.isEmpty()){
                content += "el";
            }            
            content += "if total == "+total_params+":\r\n";
            content += "    res = obj("+params.trim()+")\r\n";            
        }
        System.out.println(content);
    }
}
