#include <allegro.h>

volatile int sc=0;
void sc_add(){sc++;}END_OF_FUNCTION (sc_add);

typedef struct
{
  int x, y;
  int type;
} ALIEN;

typedef struct
{
  int x, y;
} BULLET;

typedef struct
{
  int player_x, player_y;
  int bullet_x, bullet_y;
  int abullet_x, abullet_y;
  ALIEN alien[50];
} GAME;

BITMAP *alien[3];
BITMAP *player_bmp;
BITMAP *barrier_bmp;

GAME game=
{
  0, 0, 0, 0,
};

int ao;
int alien_orientation= 1;

int score=0;

int value[3]= { 100, 400, 200  };

void init_aliens ()
{
  int x, y;

  for (y=0;y<50;y+=10)
  {
    for (x=0;x<10;x++)
    {
      game.alien[y+x].x= x*20;
      game.alien[y+x].y= (y+10)*2;
      if (y==0)
      {
        game.alien[y+x].type= 1;
      }
      else if (y>0 && y<30)
      {
        game.alien[y+x].type= 2;
      }
      else
      {
        game.alien[y+x].type= 0;
      }
    }
  }
}

int g ()
{
  int end_game= 0;
  int x, y;
  BITMAP *swap= create_bitmap (320, 240);

  clear (swap);

  game.player_x=160-7;
  game.player_y=190;

  score= 0;

  init_aliens ();

  while (!end_game)
  {
    while (sc>0 && !end_game)
    {
      if (key[KEY_ESC])
      {
        end_game = 1;
      }
      if (key[KEY_LEFT])
      {
        game.player_x--;
      }
      if (key[KEY_RIGHT])
      {
        game.player_x++;
      }
      if (key[KEY_SPACE])
      {
        if (game.bullet_y<0)
        {
          game.bullet_x= game.player_x+7;
          game.bullet_y= game.player_y;
        }
      }
      ao= alien_orientation;  
      for (x=0;x<50;x++)
      {
        if (game.alien[9].x==320-16)
        {
          alien_orientation= 0;
        }
        if (game.alien[0].x==0)
        {
          alien_orientation= 1;
        }
      }
      if (ao!=alien_orientation)
      {
        for (y=0;y<50;y++)
        {
          game.alien[y].y++;
        }
      }
      for (x=0;x<50;x++)
      {
        if (alien_orientation)
        {
          game.alien[x].x++;
        }
        else
        {
          game.alien[x].x--;
        }
        if (game.alien[x].type!=-1)
        {
          if (game.bullet_x>=game.alien[x].x && game.bullet_y>=game.alien[x].y && 
              game.bullet_x<=game.alien[x].x+16 && game.bullet_y<=game.alien[x].y+16)
          {
            score+= value[game.alien[x].type];
            game.alien[x].type=-1;
            game.bullet_y= -10;
          }
        }
      } 
      if (game.abullet_x>=game.player_x && game.bullet_y>=game.player_y && 
          game.abullet_x<=game.player_x+16 && game.bullet_y<=game.player_y+16)
      {
        exit (0);
      } 
      game.bullet_y-=2;
      game.abullet_y+=2;

      sc--;
    }

    for (x=0;x<50;x++)
    {
      if (game.alien[x].type!=-1)
      {
        draw_sprite (swap, alien[game.alien[x].type], game.alien[x].x, game.alien[x].y);
      }
    }

    draw_sprite (swap, player_bmp, game.player_x, game.player_y);

    putpixel (swap, game.bullet_x, game.bullet_y, 255);

    text_mode (16);
    textprintf (swap, font, 0, 0, 255, "Score: %d", score);

    draw_sprite (screen, swap, 0, 0);
    clear_to_color (swap, 16);
  }

  destroy_bitmap (swap);

  return (0);
}

int main ()
{
  if (!allegro_init ()) if (!install_keyboard ()) if (!install_timer ()) if (!set_gfx_mode (GFX_AUTODETECT, 320, 240, 0, 0))
  {
    LOCK_VARIABLE (sc);
    LOCK_FUNCTION (sc_add);
    if (!install_int_ex (sc_add, BPS_TO_TIMER (60)))
    {
      alien[0]= load_bitmap ("bmp/alien1.bmp", NULL);
      alien[1]= load_bitmap ("bmp/alien2.bmp", NULL);
      alien[2]= load_bitmap ("bmp/alien3.bmp", NULL);
      player_bmp= load_bitmap ("bmp/tank.bmp", NULL);
      barrier_bmp= load_bitmap ("bmp/barrier.bmp", NULL);
      g ();
      allegro_exit ();
      return 0;
    }
  }

  allegro_message ("Error: %s\n", allegro_error);
  return (-1);
}
END_OF_MAIN ();
