package test_package;


import java.util.logging.Level;
import java.util.logging.Logger;
import org.junit.Assert;
import org.junit.Test;
import rafaelpm.easyrpc.packages_build.BuildPackageFromClient;
import rafaelpm.easyrpc.packages_build.BuildPackageToServer;
import rafaelpm.easyrpc.entities.EasyRPCPackage;


/**
 *
 * @author Rafael
 */
public class TestBuildFromClient {
    
    @Test
    public void test(){
        
        EasyRPCPackage pkgToServer = HelperFunction.buildSum(2, 3);
        BuildPackageToServer buildPackageToServer = new BuildPackageToServer();
        BuildPackageFromClient buildPackageFromClient = new BuildPackageFromClient();
        byte[] bufferToServer = buildPackageToServer.toBytes(pkgToServer);
        
        try {
            Assert.assertTrue(buildPackageFromClient.setData(bufferToServer));
            Assert.assertEquals(buildPackageFromClient.easyPackage.params.size(),2);
        } catch (Exception ex) {
            Logger.getLogger(TestBuildFromClient.class.getName()).log(Level.SEVERE, null, ex);
        }        
        
    }
    
}
