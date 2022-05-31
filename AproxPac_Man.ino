//  library to handle LEDs
#include <Adafruit_NeoPixel.h>

#include <AxisJoystick.h>

#define N 8*8   // number of pixels
#define PIN 9 //  matrix pin  

#define COLOR_WALL 0x0F0F0F
#define COLOR_PLAYER 0x0F000F
#define COLOR_ENEMY 0X0F0000
#define COLOR_DEFAULT 0x000F00


int starting_position = 2; // 0-1-2
bool passible_terrain = 0;
bool lost = false;
int won = 0;
//int count = 1 ;
int nr_of_maps = 2;


//  array containg the color of the LEDs in order
long ledarray[N] ;

class Led //clasa LED - fiecare pixel este un led
{
    long _color;
public:
    Led(long color)
    {
        _color = color;
    }
};

class Player : public Led 
{
    Joystick *joystick;
    int _joystick_pin_x = 14;
    int _joystick_pin_y = 15;
    int _joystick_pin_sw = 10;

public:
    int _position;

public:
    Player() : Led(COLOR_PLAYER) {}
    Player(int position) : Led(COLOR_PLAYER) {
        _position = position;
    }

    int getPosition() const {
        return _position;
    }

    void setPosition(int position) {
        _position = position;
    }

    void setup()  //  se consideră că player-ul are un joystick, acesta fiind construit în metoda de mai jos
    {
        joystick = new AxisJoystick(_joystick_pin_sw, _joystick_pin_x, _joystick_pin_y);
    }

    void moveDown() {
        _position += 8;
    }

    void moveLeft() {
        _position -= 1;
    }

    void moveUp() {

        _position -= 8;
    }

    void moveRight() {
        _position += 1;
    }

    void movement() // modul de mișcare al player-ului cu ajutorul librăriei 'AxisJoistick.h' care are metode de isPress, isDown, isUp, isLeft, isRight ce detectează mișcările joistick-ului 
    {
        if(joystick->isPress()) //  la apăsarea pe buton se resetează harta curentă, player-ul mai având o șansă de câștig
        {
            lost = false;
            setPosition(starting_position);
            Serial.println("Button");
        }
        if(joystick->isDown())
        {
            if((ledarray[getPosition()-8] == COLOR_DEFAULT)&&(getPosition() >= starting_position))  //  boundaries
            {
              moveUp();
              //RenderFrame();
            }
            else
            {
              Serial.println("Can't go up ");
            }
            Serial.println("Interrupt - up");
        }
        else if(joystick->isUp())   //  boundaries
        {
            if(ledarray[getPosition()+8] == COLOR_DEFAULT)
            {
              moveDown();
              //RenderFrame();
            }
            else
            {
              Serial.println("Can't go down ");
            }
            Serial.println("Interrupt - down");
        }
        else if(joystick->isLeft())   //  boundaries
        {
            //p.moveLeft();
            if(ledarray[getPosition()-1] == COLOR_DEFAULT)
            {
              moveLeft();
              //RenderFrame();
            }
            else
            {
              Serial.println("Can't go left ");
            }
            Serial.println("Interrupt - left");
        }
        else if(joystick->isRight())   //  boundaries
        {
            if(ledarray[getPosition()+1] == COLOR_DEFAULT)
            {
              moveRight();
              //RenderFrame();
            }
            else
            {
              Serial.println("Can't go right ");
            }
            Serial.println("Interrupt - right");
        }
    }

};

class Enemy : Led {
public:
    int _position;
    int _last_position;
    int count ;
    int _moves ;  //  nr of moves
public:
    Enemy() : Led(COLOR_ENEMY) {}
    Enemy(int position) : Led(COLOR_ENEMY) {
        _position = position;
        count = 1;
        _moves = 3;
    }

    int getMoves() const {
        return _moves;
    }

    void setMoves(int moves) {
        _moves = moves;
    }

    int getPosition() const {
        return _position;
    }

    void setPosition(int position) {
        _position = position;
    }

    int getLastposition() const {
        return _last_position;
    }

    void setLastposition(int last_position) {
        _last_position = last_position;
    }

    int getCount() const {
        return count;
    }

    void setCount(int count) {
        count = count;
    }

    void moveDown() {
        _last_position = _position;
        _position += 8;
    }

    void moveLeft() {
        _last_position = _position;
        _position -= 1;
    }

    void moveUp() {
        _last_position = _position;
        _position -= 8;
    }

    void moveRight() {
        _last_position = _position;
        _position += 1;
    }

    //  se folosește count pentru a reține numărul de mișcări făcute de enemy - la sfârșit de loop revine la 0
    void movement1()  //  primul tip de mișcare al enemy-ului -> 3 mișcări stânga + 3 dreapta în loop
    {
      if(count <= _moves)
      {
        moveLeft();
        delay(500);
      }
      else if((count >= _moves+1)&&(count <= 2*_moves))
      {
        moveRight();
        delay(500);
        if (count == 2*_moves)
        count = 0;
      }
      count ++;  
    }
    
    void movement2()  //  al doilea tip de mișcare al enemy-ului -> 3 mișcări sus + 3 jos în loop
    {
      if(count <= _moves)
      {
        moveUp();
        delay(500);
      }
      else if((count >= _moves+1)&&(count <= 2*_moves))
      {
        moveDown();
        delay(500);
        if (count == 2*_moves)
        count = 0;
      }
      count ++;  
    }
};

/// partea de layout-uri a hărților + poziția de finish(+o eventuală poziție de start dacă se dorește a fi diferită)
int map1[N] = {
                           1,1,0,1,1,1,1,1,
                           1,0,0,0,0,0,0,1,
                           1,0,1,1,1,1,0,1,
                           1,0,0,0,0,1,0,1,
                           1,0,1,0,0,0,0,1,
                           1,0,0,1,1,0,0,1,
                           1,0,0,0,0,0,0,1,
                           1,1,1,1,1,1,0,1,
                          };
int map1_end = 62;
int map2[N] = {
                           1,1,0,0,1,1,1,1,
                           1,1,0,0,0,0,0,1,
                           1,1,1,0,0,0,1,1,
                           1,1,1,0,1,0,0,1,
                           1,1,1,0,1,1,0,1,
                           1,0,0,0,0,0,0,1,
                           1,0,1,0,1,1,1,1,
                           1,0,1,1,1,1,1,1,
                          };
int map2_end = 57;

Player p;
Enemy e,e1,e2 ;
///

void clearArray()
{
  for (int i = 0 ; i < N ; i ++)
    ledarray[i] = COLOR_DEFAULT;
}

void setupMap(int map[], Player player, Enemy enemy)  //  se încarcă harta în funcție de layout-ul de pereți/path(1-pereti,0-path) după care se adaugă Player-ul și Enemy-ul
{
    for(int i = 0 ; i < N ; i ++)
    {
            if (map[i] == 1)
            {
                ledarray[i] = COLOR_WALL;
            }
            else if (map[i] == 0)
            {
                if( i == player.getPosition())
                    ledarray[i] = COLOR_PLAYER;
                else if( i == enemy.getPosition())
                    {
                        ledarray[i] = COLOR_ENEMY;
                        ledarray[enemy.getLastposition()] = COLOR_DEFAULT;
                    }
                      
                else
                if(ledarray[i] != COLOR_ENEMY)
                      ledarray[i] = COLOR_DEFAULT;  
                
            }
    }
    RenderFrame();
}

/// partea de declarare a hărților
void Map1()
{
      p = Player(2);
      
      e = Enemy(28);
      e1 = Enemy(38);
      e2 = Enemy(54);
      e2.setMoves(5);
}
void Map2()
{
      p = Player(3);
      e = Enemy(51);
      e1 = Enemy(15);
      e.setMoves(4);
      e1.setMoves(4);
      clearArray();
}
void Map3()
{
      p = Player(2);
      e = Enemy(46);
}
///


//  initializare matrice led 
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel ledMatrix = Adafruit_NeoPixel(N, PIN, NEO_GRB + NEO_KHZ800);  //  se declară efectiv matricea

void setup()
{
  ledMatrix.begin();
  ledMatrix.show();
  Serial.begin(9600);

  Map1(); //  incepe de la harta 1

  p.setup();  //  player-ul isi face setup
  RenderFrame();
}


//  function setting the LEDs to certain collors 
void RenderFrame()  //  refreshes the matrix LEDs
{
    if((lost == false)&&(won!=nr_of_maps))  //  daca player-ul nu a pierdut sau nu a câștigat, matricea își dă refresh
    {
      for (int t = 0; t < N; t++) //  setează LED-urile matricii pe culorile transmise
      {
        ledMatrix.setPixelColor(t, ledarray[t]); 
      }
      ledMatrix.show(); //  afisează matricea
    }   
}

void wonGame()  //  arată că player-ul a câștigat
{
  for(int i = 0 ; i < N ; i ++ )
  {
    ledMatrix.setPixelColor(i, COLOR_PLAYER);
    delay(25);
    ledMatrix.show();
  }
  won = nr_of_maps;
  Serial.println("You Won !!!");
}

void lostGame()  //  arată că player-ul a pierdut
{
  for(int i = 0 ; i < N ; i ++ )
  {
    ledMatrix.setPixelColor(i, COLOR_ENEMY);
    delay(25);
    ledMatrix.show();
  }
  lost = true;
  Serial.println("You LOST !!!");
  Serial.println("For another chance, push the joystick button !!!");
}

void loop()
{
  p.movement();
  if( lost == false )
  {
    if(won == 0)  //  dacă player-ul nu a câștigat, este încărcată prima hartă
    {
      //enemyMovement1();
      e.movement1();
      e1.movement1();
      e2.movement1();
      //e1.movement1();
     setupMap(map1,p,e);
     setupMap(map1,p,e1);
     setupMap(map1,p,e2);
    }
    else if(won == 1)  //  dacă player-ul a câștigat o singură dată , este încărcată a doua hartă
    { 
      e.movement2();
      e1.movement1();
      setupMap(map2,p,e);
      setupMap(map2,p,e1);
    }
  }  

  if((p.getPosition() == e.getPosition()) || (p.getPosition() == e1.getPosition()) || (p.getPosition() == e2.getPosition()) ) // dacă player-ul se află în același loc cu enemy-ul, jocul este pierdut
  {
    lostGame();
  }
  if ((p.getPosition() == map1_end)&&(won==0))  //  dacă player-ul a ajuns în poziția de final al primei hărți se setează flag-ul de won pe 1 adică că a câștigat pe prima hartă și se trece la a doua
  { 
    e1.setCount( 1 );
    won=1;
    Map2();
  } else

  if ((p.getPosition() == map2_end)&&(won==1)) //  dacă player-ul a ajuns în poziția de final a hărții 2, se setează flag-ul de won pe 2 adică că a câștigat și pe a doua hartă și se trece la a următoarea în același mod ca la punctul precedent sau se arată că jocul a fost câștigat
  { 
    won=2;
    wonGame();
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }  
}
