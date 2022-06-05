/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package theball;

import java.util.Random;
import org.newdawn.slick.geom.Circle;

/**
 *
 * @author bryan.cancel01
 */
public class ball {
    
    Circle circular; //its a circle for now but this should be allowed to be any shape
    int xPos;
    int yPos;
    int xDir;
    int yDir;
    static int radius;
    int rndPos;

    int LOR=1,UOD=1;
    
    //random object
    Random random = new Random();
    
    //NOT USED YET
    int lastPaddle; //the last paddle that it was hit by, this allows the ball to carry scores, not sure if this is the most efficient method to do this but a method nevertheless
    
    public ball(int x, int y, int r, int xD, int yD, player[] p)
    {
        
        
        xPos= x;
        yPos= y;
        
        xDir= xD;
        yDir= yD;
        radius=r;
        
        circular = new Circle(xPos,yPos,radius);   
    }
    
    public void move()
    {   
        //A PREREABLE OPTION WOULD BE TO MAKE THIS take of at random Degrees each time but let the player that has the ball on their paddle to atleast control to what side it goes (l or r) 
        //Gets the ball to go randomly off of the paddle at 45 degrees (If xDir and yDir starts of even) L or R 
        if(random.nextInt(2) == 1)//somehow only happen the first time
            {
                if(rndPos == 4)
                {
                    LOR = 1; // so that the ball goes towards the player at the beginning
                }
                else
                {
                    LOR = -1;
                }
            }
        if(random.nextInt(2) == 1)
            {
                if(rndPos == 2)
                {
        	UOD = 1; // so that the ball goes towards the player at the beginning
                }
                else
                {
                    UOD = -1;
                }
            }
        circular.setX(xPos+= xDir*LOR);
        circular.setY(yPos+= yDir*UOD);  
    }
    
    //collisions with wall
    public void collide()
    {        
        //TOP AND BOTTOM BALL COLLISION
        if(yPos <= 0 || yPos >= TheBall.gCSize-2*radius)
        {
            if(yPos >= TheBall.gCSize-2*radius)
                yPos-=3;
            else
                yPos+=3;
            yDir *= -1;
        }
      
        //LEFT AND RIGHT BALL COLLISION
        if(xPos <= 0 || xPos >= TheBall.gCSize-2*radius)
        {
            if(xPos >= TheBall.gCSize-2*radius)
                xPos -= 3;
            else
                xPos += 3;
            xDir *= -1;
        }
        
    }
    
    public int BallXPosition(player p[])
    {
        
        //----------FIX THIS--------- (sorry... merge conflict messed this up)
          if(TheBall.numOfPlayers > 0)
         { 
        rndPos = random.nextInt(TheBall.numOfPlayers) + 1; //MAKE SURE this is random 1-(ammount of players currently playing or in array)
                
        //SHOULD ADJUST FOR PADDLE WIDTH SOON, for now it works only this is it start in the middle of the paddle
        //we need to do this because if not the ball will bounce inside the paddle and people will see that as a bug and freakin sage is gonna bash us over for that... trust me
        xPos = p[rndPos-1].xPos;
        
        //changes the X position of the ball so it starts in the middle of the paddle
        if(rndPos == 2)
            {
                xPos += 100;
            }
        if(rndPos == 3)
            {
                xPos += 200;
            }
        if(rndPos == 4)
            {
                xPos -= 200;
            }
        
         }
          return rndPos; // used for BallYPosition
    }
    
    public int BallYPosition( player[] p)
    {
        
        if(TheBall.numOfPlayers > 0)
         { 
        
        //MAKE SURE TO SPAWN THE BALL WHERE THERE ACTUALLY ARE PLAYERS for this you can check if a players paddle height or width is 0 or not
        
        //SHOULD ADJUST FOR PADDLE WIDTH SOON, for now it works only this is it start in the middle of the paddle
        //we need to do this because if not the ball will bounce inside the paddle and people will see that as a bug and freakin sage is gonna bash us over for that... trust me
        yPos = p[rndPos-1].yPos;
        

        // USED TO SET STARTING POINT ACCORDING TO VALUES PASSED
        if(rndPos == 1)
        {
            yPos += 200;
        }
        if(rndPos == 2)
        {
            yPos -= 200;
            UOD *= -1;
        }
        
        
         }
        return UOD;

    }
    

}
