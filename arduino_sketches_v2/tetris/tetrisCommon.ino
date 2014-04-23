/* tetrisCommon
 * Tetris game for the LED table
 *
 * Klaas De Craemer 2013
 *
 */
 
//Maximum size of bricks. Individual bricks can still be smaller (eg 3x3)
#define  MAX_BRICK_SIZE    4
#define  BRICKOFFSET       -1 // Y offset for new bricks

#define  INIT_SPEED        1000//Initial delay in ms between brick drops
#define  SPEED_STEP        100  // Factor for speed increase between levels, default 10
#define  LEVELUP           2 //Number of rows before levelup, default 5

/* *** Game commonly used defines ** */
#define  DIR_UP    1
#define  DIR_DOWN  2
#define  DIR_LEFT  3
#define  DIR_RIGHT 4

// Playing field
struct Field{
  uint8_t pix[FIELD_WIDTH][FIELD_HEIGHT+1];//Make field one larger so that collision detection with bottom of field can be done in a uniform way
};
Field field;

//Structure to represent active brick on screen
struct Brick{
  boolean enabled;//Brick is disabled when it has landed
  int xpos,ypos;
  int yOffset;//Y-offset to use when placing brick at top of field
  uint8_t siz;
  uint8_t pix[MAX_BRICK_SIZE][MAX_BRICK_SIZE];
};
Brick activeBrick;

//Struct to contain the different choices of blocks
struct AbstractBrick{
  int yOffset;//Y-offset to use when placing brick at top of field
  uint8_t siz;
  uint8_t pix[MAX_BRICK_SIZE][MAX_BRICK_SIZE];
};

//Brick "library"
AbstractBrick brickLib[7] = {
  {
      1,//yoffset when adding brick to field
      4,
      { {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
      }
  },
  {
      0,
      4,
      { {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0}
      }
  },
  {
      1,
      3,
      { {0,0,0,0},
        {1,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
      }
  },
  {
      1,
      3,
      { {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
      }
  },
  {
      1,
      3,
      { {0,0,0,0},
        {1,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
      }
  },
  {
      1,
      3,
      { {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
      }
  },
  {
      1,
      3,
      { {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
      }
  }
};

