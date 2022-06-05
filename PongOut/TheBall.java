package theball;

import com.leapmotion.leap.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.BasicGame;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.geom.Circle;
import org.newdawn.slick.geom.Rectangle;

public class TheBall extends BasicGame {    
    
    //constructor for basic game
    public TheBall(String title) 
    {
        super(title);
    }
    
    //Abstract method, pretty sure this isnt being used properly but it works
    public void init(GameContainer arg0) throws SlickException 
    {

    }
    
    //WE COULD PLAY WITH ALL THESE THINGS AND MAKE THIS GAME BLOW PEOPLES MINDS :D
    static int gCSize = 750; //Used to determine the size of the game as a whole and to also set limits for paddles and the ball
    
    //ALL OF THE STATIC VAR BELLOW COULD ALL BE PUT INTO ONE SINGLE EQUATION
    static int distFromEdge = 20; //paddle distance from the edge
    static int paddleWidth = 20; //paddle width
    static int paddleLength = 100; //paddle length
    
    static int moveDist = 5;//distance the paddle moves per second when pressed
    static int numOfPlayers;
    static int timeToPre = 4000;
    //INFO FOR MAX AMMOUNT OF PLAYERS
    player p_1; player p_2; player p_3; player p_4; player p_5; player p_6; player p_7; player p_8;
    player allPlayerArr[] = {p_1,p_2,p_3,p_4,p_5,p_6,p_7,p_8};
    int keys1[]={0x4F,0x50,0x51,0x4B,0x4C,0x4D,0x47,0x48}; //NUMPAD_1 through NUMPAD_8
    int keys2[]={0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09}; //KEY_1 through KEY_8
    char pos[]={'t','b','l','r','5','6','7','8'};//AT THE MOMENT THIS IS SET TO ONLY A SQUARE EVENTUALLY THIS SHOULD JUST BE NUMBERS
    int moveKeys[]={0x10,0x11,0x12,0x13,0x16,0x17,0x18,0x19,0x1E,0x1F,0x20,0x21,0x23,0x24,0x25,0x26}; //All fo the preset move keys (they are used in pairs)
    Color colors[] = {Color.red, Color.blue, Color.yellow, Color.green, Color.orange, Color.pink, Color.gray, Color.white};

    long runningTime = 0; 
    
    //array for players for referancing and loops
    player playerArr[]=new player[0];//start at 0 with 0 players created

    //EVENTUALLY we want to have the ammount of balls be determine by a variable, and perhaps be also increased by + and decreased by -
    //this would be used for power up or to add more balls to make things harder
    //instantiates just one ball
    ball ball1 = new ball((gCSize/2),(gCSize/2),10,moveDist,moveDist,playerArr); // the ball and the paddle have to be the same speed for the collisions to work properly
     
    SampleListener stuff = new SampleListener();
    /*
    public boolean exsistantPlayer(player[]p, char c) //check if that player already exsist
    {
        for(int i=0;i<p.length;i++)
            if(p[i].wall==c)
                return true;
        return false;
    }
    
    public void addToP(int spot)
    {
        if(exsistantPlayer(playerArr,allPlayerArr[spot].wall)==false)//add a player
        {
            player temp[] = new player[playerArr.length+1];
            for(int i=0; i<playerArr.length; i++)
                temp[i]=playerArr[i];
            temp[playerArr.length]=allPlayerArr[spot];//add in the new player into the array
            playerArr=temp;
            numOfPlayers = playerArr.length;
            
        } 
    }
    
    public void subFromP(int spot)
    {
        if(exsistantPlayer(playerArr,pos[spot])==true)
        {
            player temp[] = new player[playerArr.length-1];
            for(int i=0,j=0; i<playerArr.length; i++)
                if(playerArr[i].wall!=pos[spot])
                {temp[j]=playerArr[i];j++;}
            playerArr=temp;
            
        }
           
    }
    */
    
    wall w1 = new wall(gCSize/2,gCSize/2,200,25);//this is just for testing purposes, eventually the walls should be added with button click, START AT 3 FOR NOW
    Vector hand = stuff.getVect();

    static float fps =60;
    
    public void update(GameContainer gc, int delta) throws SlickException
    { 
        
        //WE NEED A VARIABLE THAT STOPS ALL PLAYERS EXEPT THE ONE WITH THE BALL IN ORDER FOR THE PLAYER THAT HAS THE BALL TO BE ABLE TO "shoot" AND ONLY THEN WILL THE GAME START

        // Obtains keyboard inputs
        Input input = gc.getInput();
        //JUST A "slow mo" FOR TESTING PURPOSES
        try {
            if (input.isKeyDown(Input.KEY_T))
                Thread.sleep(100);
        } catch (InterruptedException ex) {
            Logger.getLogger(TheBall.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        //EXAMPLE USING KEY TO CONTROL SIZING AND ROTATION, use to test the paddle and make sure it lock on to the wall and that the bounds are also set accordingly       
        
        //GROW AND SHRINK WALLS
        if(input.isKeyDown(Input.KEY_ADD))
            w1.scale(1);
        if(input.isKeyDown(Input.KEY_SUBTRACT))
            w1.scale(-1);
        
        float[] test=new float[]{234,543}; //you can set any point as its pivot point, this will let us rotate the overall shape and or tranform it, strectch and shrink to make the game harder
        
        //SET PIVOT THEN ROTATE
        //set pivot
        if(input.isKeyPressed(Input.KEY_NUMPAD1))
            w1.setPivot(w1.getLeftPivot());   
        if(input.isKeyDown(Input.KEY_C))
            w1.setPivot(w1.getCenterPivot());            
        if(input.isKeyDown(Input.KEY_NUMPAD3))
            w1.setPivot(w1.getRightPivot());
        if(input.isKeyDown(Input.KEY_NUMPAD4))
            w1.setPivot(test);
        //rotate
        if(input.isKeyDown(Input.KEY_P))
            w1.rotate((-1/fps));
        if(input.isKeyDown(Input.KEY_NUMPAD9))
            w1.rotate((1/fps));
        
        
        //MOVE IN ALL DIRECTIONS
        if(input.isKeyDown(Input.KEY_UP))
            w1.move(0,(float)-1);
        if(input.isKeyDown(Input.KEY_DOWN))
            w1.move(0,(float)1);
        if(input.isKeyDown(Input.KEY_LEFT))
            w1.move((float)-1,0);
        if(input.isKeyDown(Input.KEY_RIGHT))
            w1.move((float)1,0);
        
        //if(hand != null)
            //w1.move(hand.getX(), hand.getZ());
        
        //EXAMPLE END
        
        /*
        //CREATES AND DESTROY PLAYERS
        //numbers 1 - 8 to add, numbers 1 - 8 and lshift to subtract || numpad 1 -8 and + to add and numpad 1-8 and - to subtract
        for(int i=0,j=0;i<allPlayerArr.length;i++,j=j+2)
        {
            //when creating an player it takes info from moveKeys Array, we could now change moveKeys in a Menu and the program will take care of the rest (ditto with color)+
            if( (input.isKeyDown(keys2[i])&&!input.isKeyDown(Input.KEY_LSHIFT)) || (input.isKeyDown(keys1[i])&&input.isKeyPressed(Input.KEY_ADD)) )
            {
                allPlayerArr[i] = new player(paddleWidth, paddleLength, pos[i],moveKeys[j],moveKeys[j+1],colors[i]);
                addToP(i);
            }
            else if( (input.isKeyDown(keys2[i])&&input.isKeyDown(Input.KEY_LSHIFT)) || (input.isKeyDown(keys1[i])&&input.isKeyPressed(Input.KEY_SUBTRACT)) )
                subFromP(i);
            
            
        }
                
        //INPUT AND LIMITS (currently each player has controls already assigned from the moveKeys array)
        for(int i=0,j=0;i<playerArr.length;i++, j=j+2)
        {
            if (input.isKeyDown(playerArr[i].lButton))
                playerArr[i].move(moveDist);
            else if (input.isKeyDown(playerArr[i].rButton))
                playerArr[i].move(-moveDist);             
        }
        
        //IF WE WANT TO KEEP THE COLLIDE WITH THE BALL in the player class we need to make sure that we have this collide for every ball that has been rendered, this can be done with a for loop
        //COLLIDERS WITH BALL for now
        for(int i=0;i<playerArr.length;i++)//this for loop needs to have a for loop inside of it if there are going to be multiple balls (that for loop will loop for all the balls
            playerArr[i].collide(ball1);
        
        runningTime += delta; // finds total running time
        
        if(runningTime >= timeToPre - 200 && runningTime <= timeToPre) //adds to the X and Y Position
            {
            ball1.BallXPosition(playerArr);
            ball1.BallYPosition(playerArr);
            }
        
        if(runningTime > timeToPre) //gives 4 second delay to choose number of players before ball moves
        {
            ball1.move(); //handles moving
        }
        
        ball1.collide(); //handles collisions with wall
        */
          
        
    }
    
    Rectangle one;
    Rectangle two;
    
    static int testing;
    
    public void render(GameContainer gc, Graphics g) 
    {
        
        /*
        //print outs all the players in the playerArr
        g.setColor(Color.white);
        for(int i=0;i<playerArr.length;i++)
            g.drawString("playerArr at pos"+i+": "+playerArr[i].wall,(gCSize/2)-100 ,(i*15)+50);
         
        
        //for loop that makes all exsistant players appear
        for(int i=0;i<playerArr.length;i++)
        {
            g.setColor(playerArr[i].color);
            g.draw(playerArr[i].paddle);
            g.fill(playerArr[i].paddle); 
        }
        
        
        //Using New Ball Class
        if(timeToPre < runningTime)
        {
            g.setColor(Color.pink); //SETTING COLOR FIRST ELIMINATED ARTIFACTS
            g.draw(ball1.circular); 
            g.fill(ball1.circular);
        }
        //creates 4 second timer
        String runTime = Long.toString(4000 - runningTime);
        if(4000 - runningTime > 0)
            g.drawString(runTime, 50, 50); 
        */
        
        //float[] pointsArr = w1.pointsArr;
        
        /*
        g.setColor(Color.white);
        for(int i=0,j=1;i<pointsArr.length;i+=2,j++)
        {
            g.drawString("Vertices "+(j)+" X: "+Float.toString(pointsArr[i]),(gCSize/2)-100 , i * 15);
            g.drawString("           Y: "+Float.toString(pointsArr[i+1]),(gCSize/2)-100 , (i+1) * 15);
        }
        */
            
        
        //experimental wall
        g.setColor(Color.red);
        g.draw(w1.rect);//base rectangle
        g.setColor(Color.gray);
        g.draw(new Circle ((int)w1.wall.getCenterX(),(int)w1.wall.getCenterY(),5));//center point
        g.setColor(Color.white);
        g.draw(w1.wall);//actuall wall with collisions
        g.draw(new Circle ((int)w1.currPivot[0],(int)w1.currPivot[1],5));//current pivot point       
        
        //Rectangle one = w1.rect;
        //Rectangle two = w1.rect;
        
        //THIS SHIT IS SCALING FROM THE TOP LEFT
        /*
        Rectangle one  = new Rectangle(w1.xPos,w1.yPos,200,25);       
        Rectangle two  = new Rectangle(w1.xPos,w1.yPos,200,25);
        
        one.setCenterX(w1.xPos);
        one.setCenterY(w1.yPos);
        one.setWidth(one.getWidth()+10);
        
        two.setCenterX(w1.xPos);
        two.setCenterY(w1.yPos);  
        two.setWidth(one.getWidth()+25);
        
        g.setColor(Color.yellow);
        g.draw(one);
        g.setColor(Color.cyan);
        g.draw(two);
        */
        g.setColor(Color.white);
        g.drawString("left(1) other(0) right(2): "+w1.refPivotIndex,(gCSize/2)-100 , 500);
        g.drawString("curr pivot x: "+w1.currPivot[0],(gCSize/2)-100 , 515);
        g.drawString("curr pivot y: "+w1.currPivot[1],(gCSize/2)-100 , 530);
        g.drawString("rotVal: "+w1.rotVal,(gCSize/2)-100 , 545);
        g.drawString("testing: "+testing,(gCSize/2)-100 , 560);
    }
    
    static AppGameContainer app;
            
    public static void main(String[] args) throws SlickException 
    {
        //gives the display the name pongout and created a box of the size 1000 by 1000
        app = new AppGameContainer(new TheBall("PongOut"));
        //container here is a perfect square MIGHT BE GOOD TO ALLOW RESIZING EVENLY
        app.setDisplayMode(gCSize, gCSize, false);
        //framerate affects how fast the objects are moving because it it how many times the update function is iterated per second
        app.setTargetFrameRate((int)fps);
        //start the update function
        app.start();
    }
			   
}