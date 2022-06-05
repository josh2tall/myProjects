/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package theball;

import com.leapmotion.leap.Controller;
import com.leapmotion.leap.Frame;
import com.leapmotion.leap.Gesture;
import com.leapmotion.leap.Hand;
import com.leapmotion.leap.HandList;
import com.leapmotion.leap.Listener;
import com.leapmotion.leap.Vector;

/**
 *
 * @author joshuaacosta
 */

    public class SampleListener extends Listener {
    
    public static Vector handCenter;

    public void onConnect(Controller controller) {
        System.out.println("Connected");
        controller.enableGesture(Gesture.Type.TYPE_CIRCLE);
        controller.enableGesture(Gesture.Type.TYPE_KEY_TAP);
        controller.enableGesture(Gesture.Type.TYPE_SWIPE);
        controller.enableGesture(Gesture.Type.TYPE_SCREEN_TAP);
    }

    public void onFrame(Controller controller) {
        Frame frame = controller.frame();
        /*System.out.println("Frame id: " + frame.id()
                   + ", timestamp: " + frame.timestamp()
                   + ", hands: " + frame.hands().count()
                   + ", fingers: " + frame.fingers().count()
                   + ", tools: " + frame.tools().count()
                  + ", gestures " + frame.gestures().count());*/  
        
        
        for(Gesture g : frame.gestures())
        {
            if(g.type() == Gesture.Type.TYPE_SWIPE)
            {
                System.out.println("A");
            }
            else if (g.type() == Gesture.Type.TYPE_CIRCLE)
            {
                System.out.println("B");
            }
            
        }
        Hand firstHand;
        HandList hands = frame.hands();
        if(hands.get(0) != null)
        {
            firstHand = hands.get(0);
            handCenter = firstHand.palmPosition();
        }
        
        
    }
    void enableGesture(Gesture.Type type, boolean enable)
    {
        
    }
    public Vector getVect()
    {
       return handCenter;
                
    }
    
    
    
}

    

