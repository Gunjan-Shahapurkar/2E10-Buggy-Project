import controlP5.*;
import processing.net.*;
import processing.serial.*;

ControlP5 p5;
Client arduino;
ControlFont font;
Button disText1;
Button disText2;

char speed;
char distance;
int distance_int = 0;    // For converting char distance to int
int speed_int = 0;      // For converting char speed to int
boolean data = true;   // Used to alternate between incoming sopeed data and distance data

// Road graphics
int x_pos = 0;
int x1, x2, x3;


void setup() {

 size(500,500);
 noStroke();    // Removes outline around shapes 
 
 p5 = new ControlP5(this);
 arduino = new Client(this, "192.168.1.234", 5200);    // "127.0.0.1" (laptop) // 192.168.1.234 (home)  // "192.168.43.54" (hotspot)   // 192.168.43.47 (hotspot - buggy 2)    // 192.168.43.80
 font = new ControlFont(createFont("Ebrima", 15, true), 20);
 

 if (arduino.active()) {    // Displays in console
   println("Server Connected");
 }
 else {
   println("Not Connected");
 }
 
 // Buttons to control buggy and display data
 
 p5.addButton("Go")    // Moves the buggy
   .setValue(0)
   .setPosition(150,170)
   .setSize(200,40)
   .setColorBackground(color(#58CE00)) 
   .setColorForeground(color(#4BB402))
   .setColorActive(color(#3E9303))
   .setFont(font);
   
   
 p5.addButton("Stop")    // Stops the buggy
      .setValue(0)
      .setPosition(150,290)
      .setSize(200,40)
      .setColorBackground(color(#E30A02))
      .setColorForeground(color(#C90902))
      .setColorActive(color(#AF0600))
      .setFont(font);
      
 disText1 = p5.addButton("Distance")    // Prints buggy distance using a button that changes the button name
             .setValue(0)
             .setPosition(150,70)
             .setSize(200,40)
             .setColorBackground(color(255,255,255,1))
             .setColorForeground(color(255,255,255,1))
             .setColorActive(color(255,255,255,1))
             .setFont(font);
             ;
 disText2 = p5.addButton("Speed")    // Prints buggy speed using a button that changes the button name
             .setValue(0)
             .setPosition(150,390)
             .setSize(200,40)
             .setColorBackground(color(255,255,255,1))
             .setColorForeground(color(255,255,255,1))
             .setColorActive(color(255,255,255,1))
             .setFont(font);
             ;
                       
}
void draw() {
  background(color(0,0,0));
  
  // Moving white lines of a road
  background(color(0,0,0));
  noStroke();    // Removes outline around shapes
  fill(255,255,255);    // White rectangles
  
  x1 = x_pos + 0;
  x2 = x_pos + 200;
  x3 = x_pos + 400;

  rect(x1, 240, 160, 20);
  rect(x2, 240, 160, 20);
  rect(x3, 240, 160, 20);
  
  rect (x1 - 200, 240, 160, 20);
  rect (x1 - 400, 240, 160, 20);
  rect (x1 - 600, 240, 160, 20);
  
  x_pos += 5;    // Continuously changes position of the rectangles
  if (x_pos > width) {    // Returns position of rectangles to starting position once they reach the end
  x_pos = 0;
  }
  

  // Receive data from arduino of speed and distance
  if (arduino.available() > 0) {
    if (data) {    // Alternate between receiving distance and speed
      distance = arduino.readChar();
      data = false;
    }
    else {
      speed = arduino.readChar();
      data = true;
    }
 }
  
  distance_int = int(distance);    // Convert char (distance) to int (distance_int)
  disText1.setCaptionLabel("Distance: " + distance_int);
  speed_int = int(speed);    // Convert char (speed) to int (speed_int)
  disText2.setCaptionLabel("Speed: " + speed_int + " cm");  // This function changes the title of the button
  
}


public void Go(){    // Motors are turned on
 if (frameCount > 0){
   arduino.write('f');    
 }
}

public void Stop() {    // Motors are turned off
  if (frameCount > 0){
    arduino.write('s');
 }
}
