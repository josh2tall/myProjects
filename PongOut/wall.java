/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package theball;

import org.newdawn.slick.geom.Rectangle;
import org.newdawn.slick.geom.Shape;
import static org.newdawn.slick.geom.Transform.createRotateTransform;

/**
 *
 * @author Bryan
 */
public class wall {
  
    //maybe might want to shift these to use 0,0 top left corner... maybe
    int xPos;//center point x
    int yPos;//center point y
    
    int width;
    int length;
    
    Rectangle rect;//referance rectangle use only to grow and shrink AND translate
    Shape wall;//the object that is actually being displayed and rotated and moved etc
     
    //array of all the vertices in the wall, used to get rotation axis
    float[] pointsArr;//gets them in this order topleft (x,y), topright (x,y), bottomright (x,y), bottomleft (x,y)
    
    float rotVal=0;//total rotation value of off center point rotation
    float currRotVal;
    
    float[] currPivot= new float[2];
    
    int refPivotIndex;
    
    float[] saved;
    
    //constructor
    wall(int x, int y, int w, int l)
    {
        xPos=x;
        yPos=y;
        width=w;
        length=l;
        
        //create rect BASE with x and y center and passed width and length
        rect = new Rectangle(xPos, yPos, width, length);
        rect.setCenterX(xPos);
        rect.setCenterY(yPos);
        
        wall = rect;//wall pointer points to same data a rect (VERY IMPORTANT)
    }
    
    public void updateC()//updates center and size
    {
        //update wall with rect size and re-aply rotation
        wall = rect; //passes, x and y mid point and new width and height        
        wall = wall.transform(createRotateTransform(rotVal, wall.getCenterX(), wall.getCenterY()));
    }
    
    public void updateP() //updates pivots
    {
        //update pivot
        if(refPivotIndex==1)
            setPivot(getLeftPivot());
        else if(refPivotIndex==2)
            setPivot(getRightPivot());
        else if(refPivotIndex==0)
            setPivot(getCenterPivot());
        else
            setPivot(currPivot);
    }

    //RELATIVELY SMALL BUT BOTHERSOME ISSUE IS THE THE PIVOT POINTS ARE STILL FIDGETY, we just cant see them fidgeting anymore since we are only showing the currpivot point and not the actually pivots found from the shape methods
    //SOMETHING TO DO WITH HAVING THIS LINE IN OTHER LOCATIONS pointsArr=wall.getPoints();
    
    public void move(float x, float y)
    {
        rect.setCenterX(rect.getCenterX()+x);
        rect.setCenterY(rect.getCenterY()+y);
        //SHOULD NOT NEED TO DO THIS, when i updat the pivots it should do this automatically
        currPivot[0]+=x;//SHOULD ONLY BE FOR RANDOM PIVOT POINTS
        currPivot[1]+=y;
        updateC();
        updateP();
    }   
    
    public void scale(float val)//right now we are only scaling width but we want to scale height also soon... maybe..
    {   
        saved = currPivot;//save the current pivot point since we are growing relative to it

        rect.setWidth(rect.getWidth()+(val));//grow the rect GROWS FROM THE LEFT TOP        
                
        if(!(refPivotIndex==1 || refPivotIndex==2))
            rect.setCenterX(wall.getCenterX());
        else if(refPivotIndex==1)//adjust for growing from left CENTER POINT STILL FIDGETY-UNKOWN
        {
            updateC();
            updateP();
            float changeY=saved[1]-currPivot[1];//get the changes in x and y from the starting pivot point to the current pivot point
            float changeX=saved[0]-currPivot[0];
            rect.setCenterY(rect.getCenterY()+changeY);//move centerpoint of the rect
            rect.setCenterX(rect.getCenterX()+changeX);
            //SHOULD NOT NEED TO DO THIS, when i update the pivots it should do this automatically
            currPivot[1]+=changeY;//shift centerpoint equidistance
            currPivot[0]+=changeX;
        }
        else if(refPivotIndex==2)//adjust for growing from right CENTER POINT STILL FIDGETY-UNKOWN
        {
            updateC();
            updateP();
            float changeY=currPivot[1]-saved[1];//get the changes in x and y from the starting pivot point to the current pivot point
            float changeX=currPivot[0]-saved[0];
            rect.setCenterY(rect.getCenterY()-changeY);//move centerpoint of the rect
            rect.setCenterX(rect.getCenterX()-changeX);
            //SHOULD NOT NEED TO DO THIS, when i updat the pivots it should do this automatically
            currPivot[1]+=changeY;//shift centerpoint equidistance
            currPivot[0]+=changeX;
        }
        updateC();//updates the wall with the new rectangle width

    }
    
    public void rotate(float val)//rotate along any point passed
    {
        wall = wall.transform(createRotateTransform(val, currPivot[0], currPivot[1]));
        rotVal=rotVal+val;
        rect.setCenterX(wall.getCenterX());
        rect.setCenterY(wall.getCenterY());    
    }
        
    public void setPivot(float[] arr)//set current pivot and refPivotIndex exception
    {
        int r=refPivotIndex;
        if( ((arr[0]!=getLeftPivot()[0] && arr[1]!=getLeftPivot()[1])&&(arr[0]!=getRightPivot()[0] && arr[1]!=getRightPivot()[1])&&(arr[0]!=getCenterPivot()[0] && arr[1]!=getCenterPivot()[1])) )
            refPivotIndex=-1;
        else 
            refPivotIndex=r;//since in order to check whethe its a random pivot point we need to check if the current pivot point matches our preset pivot point, and since those pivot points change the reference value we need to reset the value once we are finished if it isnt a random pivot point
        currPivot= arr;
    }
      
    //Get Functions-important when we need the left or right point of one paddle to make that the other paddles pivot point
    public float[] getLeftPivot()
    {
        pointsArr=wall.getPoints();
        refPivotIndex=1;
        float[] pivot=new float[]{((pointsArr[0]+pointsArr[6])/2),((pointsArr[1]+pointsArr[7])/2)};
        return pivot;
    }
    
    public float[] getCenterPivot()
    {
        pointsArr=wall.getPoints();
        refPivotIndex=0;
        float[] pivot=new float[]{wall.getCenterX(),wall.getCenterY()};
        return pivot;
    }

    public float[] getRightPivot()
    {
        pointsArr=wall.getPoints();
        refPivotIndex=2;
        float[] pivot=new float[]{((pointsArr[2]+pointsArr[4])/2),((pointsArr[3]+pointsArr[5])/2)};
        return pivot;
    }
}