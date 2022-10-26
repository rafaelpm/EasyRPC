package test_package;


import java.util.logging.Level;
import java.util.logging.Logger;
import org.junit.Assert;
import org.junit.Test;
import rafaelpm.easyrpc.packages_build.BuildPackageFromServer;
import rafaelpm.easyrpc.packages_build.BuildPackageToClient;
import rafaelpm.easyrpc.entities.EasyRPCPackage;


/**
 *
 * @author Rafael
 */
public class TestBuildToClient {
    
    @Test
    public void test(){        
        EasyRPCPackage pkg = HelperFunction.buildSumResult(5);        
        BuildPackageToClient buildPackageToClient = new BuildPackageToClient();
        BuildPackageFromServer buildPackageFromServer = new BuildPackageFromServer();
        byte[] bufferToClient = buildPackageToClient.toBytes(pkg);
        
        try {
            Assert.assertTrue(buildPackageFromServer.setData(bufferToClient));
        } catch (Exception ex) {
            Logger.getLogger(TestBuildToClient.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
}
