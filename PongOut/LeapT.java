/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package theball;



import com.leapmotion.leap.Controller;
import java.io.IOException;

/**
 *
 * @author joshuaacosta
 */
public class LeapT {

    
    public void LeapT() {
        
        Controller controller = new Controller();
        SampleListener listener = new SampleListener();
        
        // Have the sample listener receive events from the controller

        controller.addListener(listener);
        
        
        
    // Keep this process running until Enter is pressed
        System.out.println("Press Enter to quit...");
        try {
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        
        // Remove the sample listener when done
        controller.removeListener(listener);
        
        
    }
    
}

