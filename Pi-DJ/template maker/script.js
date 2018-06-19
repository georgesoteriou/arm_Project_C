var img;

var buttons = [];

function setup() {
  createCanvas(3508,2480);
  img = loadImage("bg.png");
  frameRate(1);
  var xoffset = 234;
  var yoffest = 38;

  for (let i = 0; i < 36; i++) {
    buttons[i] = [];
    for (let j = 0; j < 50; j++) {
      buttons[i][j] = {
        xstart: xoffset + 90 * i,
        xend: xoffset + 90 * i + 80,
        ystart: yoffest + 47 * j,
        yend: yoffest + 47 * j + 40,
        set: 0
      }
      if(i % 2 == 0){
        buttons[i][j].col = {
          r: 163,
          g: 192,
          b: 255
        }
      }else{
        buttons[i][j].col = {
          r: 255,
          g: 255,
          b: 255
        }
      }
      noStroke();
      fill(buttons[i][j].col.r, buttons[i][j].col.g, buttons[i][j].col.b);
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
        if(b.set == 0){
          fill(247, 143, 118);
          b.set = 1;
          rect(b.xstart, b.ystart, b.xend - b.xstart, b.yend - b.ystart);
        }else if(b.set == 1){
          fill(247, 143, 118);
          rect(b.xstart, b.ystart, b.xend - b.xstart, (b.yend - b.ystart)/2);
          fill(b.col.r, b.col.g, b.col.b);
          rect(b.xstart, b.ystart + ((b.yend - b.ystart)/2), b.xend - b.xstart, (b.yend - b.ystart)/2);
          b.set = 2;
        }else if(b.set == 2){
          fill(b.col.r, b.col.g, b.col.b);
          rect(b.xstart, b.ystart, b.xend - b.xstart, (b.yend - b.ystart)/2);
          fill(247, 143, 118);
          rect(b.xstart, b.ystart + ((b.yend - b.ystart)/2), b.xend - b.xstart, (b.yend - b.ystart)/2);
          b.set = 3;
        }else if(b.set == 3){
          fill(b.col.r, b.col.g, b.col.b);
          rect(b.xstart, b.ystart, b.xend - b.xstart, b.yend - b.ystart);
          b.set = 0;
        }
      }
    });
  });
}