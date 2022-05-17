import controlP5.*;
import processing.net.*;

ControlP5 p5;
Client arduino;
ControlFont font;
Button disText;
//PImage img;
char data;

// Road graphics
int x_pos = 0;
int x1, x2, x3;


void setup() {

 size(500,500);
 noStroke();
 //img = loadImage("background.jpg");
 //img.resize(500,500);
 
 p5 = new ControlP5(this);
 arduino = new Client(this, "192.168.43.54" ,5200);    // "127.0.0.1" (laptop) // 192.168.1.234 (home)  // "192.168.43.54" (hotspot)   // 192.168.43.47 (hotspot - buggy 2)    // 192.168.43.80
 font = new ControlFont(createFont("Ebrima",15,true),20);
 
 if (arduino.active()) {    // Displays in console
   println("Server Connected");
 }
 else {
   println("Not Connected");
 }
 
 p5.addButton("Go")
   .setValue(0)
   .setPosition(150,170)
   .setSize(200,40)
   .setColorBackground(color(#58CE00)) 
   .setColorForeground(color(#4BB402))
   .setColorActive(color(#3E9303))
   .setFont(font);
   
   
 p5.addButton("Stop")
      .setValue(0)
      .setPosition(150,290)
      .setSize(200,40)
      .setColorBackground(color(#E30A02))
      .setColorForeground(color(#C90902))
      .setColorActive(color(#AF0600))
      .setFont(font);
      
 disText = p5.addButton("Buggy Control")    // Prints buggy status using a button that changes the button name
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
  //background(img);
  
  // Moving white lines of a road
  background(color(0,0,0));
  noStroke();
  fill(255,255,255);
  
  x1 = x_pos + 0;
  x2 = x_pos + 200;
  x3 = x_pos + 400;

  rect(x1, 240, 160, 20);
  rect(x2, 240, 160, 20);
  rect(x3, 240, 160, 20);
  
  rect (x1 - 200, 240, 160, 20);
  rect (x1 - 400, 240, 160, 20);
  rect (x1 - 600, 240, 160, 20);
  
  x_pos += 22;
  if (x_pos > width) {
  x_pos = 0;
  }
  
    
  SendRead();
  if (data == 'O') {
    disText.setCaptionLabel("Obstacle Detected");  // This function changes the title of the button
    println("Obstacle Detected");
  }
  else if (data == 'C') {
    disText.setCaptionLabel("Obstacle Cleared");
    println("Obstacle Cleared");
  }
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

void SendRead() { 
  // Periodically sends 't' to keep the server engaged to allow data to be continuously received by the client
  arduino.write('T');
  // Reads in character 'O' or 'C' to indicate what message to print into GUI
  data = arduino.readChar();
  delay(250);    // Prevents arduino to be overwhelemed by characters being sent over
}
