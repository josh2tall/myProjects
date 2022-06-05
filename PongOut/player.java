/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package theball;

import org.newdawn.slick.Color;
import org.newdawn.slick.geom.Rectangle;

//FIX COLLISION IN HERE AS WELL, YOU EVEN WANT TO FIX SETXANDYPOS SINCE IT USES paddle/2 which counts on the x and y pos to be in the center
//WHATEVER HAS (paddleLength/2) OR (paddleHeight/2) NEEDS TO BE UPDATED
//BY UPDATE I MEAN = We recently learn that the standard x and y position is at the left corner of the object so we need to adjust to this (we though it was in the center)

/**
 *
 * @author Bryan
 */
public class player {

    //VARIABLES
    //xPos and yPos are set depending on the wall that they are assigned to
    public int xPos;
    public int yPos;
    
    //these adjust to what orientation the wall the paddles are attached to is in
    public int lButton;
    public int rButton;
    
    Color color;
    
    //what shape is the paddle FOR LATER IMPLEMENTATION start with Rectangle
    Rectangle paddle;
    
    int score=0;//IMPLEMENT SOON AND DISPLAY THESE ON THE CENTERPOINTS OF EACH WALL IN THE GAME AREA
    
    player()
    {
        
    } 
    
    player(int w, int l, int lb, int rb, Color c) //ONLY ONCE Constructor
    {
        color=c;
        lButton=lb;
        rButton=rb;
        
    }   
    
    
    public void move(int in) //FIX COLLISOIN with new understanding
    {   
    
    }
}