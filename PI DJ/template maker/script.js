var img;

var buttons = new Array();

function setup() {
  createCanvas(3508,2480);
  img = loadImage("bg.png"); 
  
  var xoffset = 234;
  var yoffest = 38;

  for (let i = 0; i < 36; i++) {
    buttons[i] = new Array();
    for (let j = 0; j < 50; j++) {
      buttons[i][j] = {
        xstart: xoffset + 90 * i,
        xend: xoffset + 90 * i + 80,
        ystart: yoffest + 47 * j,
        yend: yoffest + 47 * j + 40,
        set: false
      }
      noStroke();
      fill(255, 255, 255);
      rect(buttons[i][j].xstart, buttons[i][j].ystart, buttons[i][j].xend - buttons[i][j].xstart, buttons[i][j].yend - buttons[i][j].ystart);
    }
  }
}

function draw(){
  background(img);
}

function mousePressed(){
  noStroke();
  buttons.forEach(bs => {
    bs.forEach(b => {
      if(mouseX >= b.xstart && mouseX < b.xend && mouseY >= b.ystart && mouseY < b.yend){
        if(b.set){
          fill(255, 255, 255);
          b.set = false;
        }else{
          fill(255, 150, 0);
          b.set = true;
        }
        rect(b.xstart, b.ystart, b.xend - b.xstart, b.yend - b.ystart);
      }
    });
  });
}