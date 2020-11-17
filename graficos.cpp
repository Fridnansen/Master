#include "graficos.h"

//==========================================================================

GLfloat colWhite[] = {1.0f, 1.0f, 1.0f};
GLfloat colBlack[] = {0.3f, 0.3f, 0.3f}; //{0, 0, 0};
GLfloat colRed[] = {1.0f, 0.0f, 0.0f};
GLfloat colGreen[] = {0.0f, 1.0f, 0.0f};
GLfloat colBlue[] = {0.0f, 0.0f, 1.0f};
GLfloat colYellow[] = {1.0f, 1.0f, 0.0f};
GLfloat colCyan[] = {0.0f, 1.0f, 1.0f};
GLfloat colOrange[] = {1.0f, 0.6f, 0.0f};
GLfloat colGrey[] = {0.3f, 0.3f, 0.3f};
GLfloat colPink[] = {1.0f, 0.0f, 1.0f};
GLfloat colMarron[] = {0.5f, 0.3f, 0.0f};
GLfloat colbrown[] = {0.72, 0.47, 0.34, 1};
GLfloat colbrown_dark[] = {0.2, 0.22, 0.15, 1};

GLfloat pos[] = {5.0, 10.0, 5.0, 0.0};
GLfloat ref_difusa[] = {0.2, 0.9, 1.0, 1.0};
GLfloat ref_especular[] = {0.5, 1.0, 1.0, 1.0};
GLfloat ref_brillo[] = {100.0};

float seleccion[] = {0.8, 0.7, 0.3, 0};
float movimientoCorrecto[] = {0.3, 0.8, 0.3, 0};
float movimientoIncorrecto[] = {0.8, 0.3, 0.3, 0};
float colortablero[4] = {0.5, 0.5, 0.8, 0};

//==========================================================================

Graficos Proyeccion;

void Graficos::dibujarTablero() {
  int x = objMasterdrez.piezaSeleccionada / 16;
  int y = objMasterdrez.piezaSeleccionada % 16;
  int x2 = objMasterdrez.casillaSeleccionadaActualmente / 16;
  int y2 = objMasterdrez.casillaSeleccionadaActualmente % 16;

  glPushMatrix();
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      glLoadName(i * 16 + j + 1);
      glPushMatrix();

      if ((i + j) % 2 == 1) {
        if ((i < 4 && j < 4) || (i < 4 && j > 11) || (i > 11 && j < 4) ||
            (i > 11 && j > 11))
          glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                       colbrown_dark); // colBlack, colorcasilla
        else
          glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                       colorcasilla); // colorcasilla
      } else {
        if ((i < 4 && j < 4) || (i < 4 && j > 11) || (i > 11 && j < 4) ||
            (i > 11 && j > 11))
          glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                       colbrown_dark); // colBlack, colorcasilla
        else
          glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colWhite); // colWhite
      }

      glTranslatef(5 * i, 0, 5 * j);
      if (objMasterdrez.seleccionandoCasilla) {
        if (objMasterdrez.editando) {
          if ((i == x) && (j == y))
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, seleccion);
          else if ((i == x2) && (j == y2)) {
            if (objMasterdrez.casillaValida(
                    objMasterdrez.casillaSeleccionadaActualmente,
                    objMasterdrez.tablero))
              glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                           movimientoCorrecto);
            else
              glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                           movimientoIncorrecto);
          }
        } else {
          if ((i == x) && (j == y))
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, seleccion);
          else if ((i == x2) && (j == y2)) {
            if (objMasterdrez.casillaYTurnoValidos(
                    objMasterdrez.piezaSeleccionada,
                    objMasterdrez.casillaSeleccionadaActualmente,
                    objMasterdrez.blancas, objMasterdrez.negras,
                    objMasterdrez.amarillas, objMasterdrez.rojas,
                    objMasterdrez.tablero, objMasterdrez.turno, true))
              glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                           movimientoCorrecto);
            else
              glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
                           movimientoIncorrecto);
          }
        }
      }
      glBegin(GL_QUADS);
      {
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 5);
        glVertex3f(5, 0, 5);
        glVertex3f(5, 0, 0);
      }
      glEnd();
      glPopMatrix();
    }
  }

  // Base
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colbrown);
  glTranslatef(-2, -1, -2);
  cubo(84, 0.9, 84);
  glPopMatrix();
}

void Graficos::dibujarMasterdrez() {

  glPushMatrix();
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, ref_difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, ref_especular);
  glMaterialfv(GL_FRONT, GL_SHININESS, ref_brillo);

  glScalef(0.5, 1, 0.5);
  dibujarTablero();

  glScalef(1, 0.5, 1);
  if (objMasterdrez.dibujaPiezas == 1) {

    dibujarPiezas();
  }

  glPopMatrix(); // Desapila la transformacion geometrica
}

void Graficos::cubo(GLfloat x, GLfloat y, GLfloat z) {
  glBegin(GL_QUADS);
  { // Cuadrilateros
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(x, 0, 0);
    glVertex3f(x, y, 0);
    glVertex3f(x, y, z);
    glVertex3f(x, 0, z);
  }
  glEnd();
  glBegin(GL_QUADS);
  {
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, z);
    glVertex3f(0, y, z);
    glVertex3f(0, y, 0);
  }
  glEnd();
  glBegin(GL_QUAD_STRIP);
  { // Cinta de cuadrilateros
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(x, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(x, y, 0);
    glVertex3f(0, y, 0);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(x, y, z);
    glVertex3f(0, y, z);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(x, 0, z);
    glVertex3f(0, 0, z);
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(x, 0, 0);
    glVertex3f(0, 0, 0);
  }
  glEnd();
}

/**
* A partir un vector de puntos genera una malla de rotacion de los mismos.
*
* @param p
* @param ptos
* @param n
* @param v
*/
void Graficos::generarMallaPorRotacion(float p[6][3], float ptos[6][6][3],
                                       int n, int v) {
  float rad;
  int i, j;
  rad = 2 * 3.1416 / 5;
  for (i = 1; i < v; i++) // hasta numero de vueltas
  {
    for (j = 0; j < n; j++) // hasta numero de puntos
    {
      // Rotar sobre eje y:
      ptos[i][j][0] = (float)(p[j][0] * cos((float)rad * i) +
                              p[j][2] * sin((float)-rad * i));
      ptos[i][j][1] = (float)(p[j][1]);
      ptos[i][j][2] = (float)(p[j][2] * cos((float)rad * i) -
                              p[j][0] * sin((float)-rad * i));
    }
  }

  for (i = 0; i < n; i++)
    for (j = 0; j < 3; j++)
      ptos[0][i][j] = ptos[v - 1][i][j] = p[i][j];
}

/**
* Dibuja la superficie dada por la matriz malla de puntos ptos.
*
* @param miNurb
* @param ptos
* @param n
* @param m
*/
void Graficos::dibujarSuperficie(GLUnurbsObj *miNurb, float ptos[6][6][3]) {

  float nudosUniforme[10] = {0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 3.0, 3.0, 3.0},
        nudosPeriodico[10] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
  gluBeginSurface(miNurb); /* Comienza la definicion de la superficie    */
  gluNurbsSurface(miNurb,  /* Genera la superficie                 */
                  10, nudosPeriodico, /* Vector de nodos direccion u       */
                  10, nudosUniforme,  /* Vector de nodos direccion v       */
                  6 * 3,              /* Numero de datos entre puntos de   */
                  3,                  /*  control en u y v                 */
                  &ptos[0][0][0],     /* Array de puntos de control        */
                  4, 4,               /* Orden en u y v                    */
                  GL_MAP2_VERTEX_3);  /* GL_MAP2_VERYEX_3 o GL_MAP2_VERTEX_4  */
  gluEndSurface(miNurb);              /* fin de definicion de superficie */
}

void Graficos::dibujarPeon() {

  glPushMatrix();
  {
    if (piezasStaunton) {
      generarMallaPorRotacion(pPeon, ptos, 6, 6);
      dibujarSuperficie(miNurb, ptos);
      glRotatef(180, 0, 1, 0);
      dibujarSuperficie(miNurb, ptos);
      generarMallaPorRotacion(cPeon, ptos, 6, 6);
      glTranslatef(0, 2.9, 0);
      glScalef(3, 3, 3);
      dibujarSuperficie(miNurb, ptos);
      glRotatef(180, 0, 1, 0);
      dibujarSuperficie(miNurb, ptos);
    } else if (piezasEgipcias) {
      // left side
      glBegin(GL_TRIANGLES);
      glNormal3f(-1.0f, 1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, 1.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.0f, PAWN_HEIGHT, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glEnd();

      // front side
      glBegin(GL_TRIANGLES);
      glNormal3f(0.0f, 1.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.0f, PAWN_HEIGHT, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, 1.0f);
      glEnd();

      // right side
      glBegin(GL_TRIANGLES);
      glNormal3f(1.0f, 1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.0f, PAWN_HEIGHT, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);
      glEnd();

      // back side
      glBegin(GL_TRIANGLES);
      glNormal3f(0.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.0f, PAWN_HEIGHT, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glEnd();
    }
  }
  glPopMatrix();
}

void Graficos::dibujarTorre() {

  glPushMatrix();
  {
    if (piezasStaunton) {
      generarMallaPorRotacion(pTorre, ptos, 6, 6);
      dibujarSuperficie(miNurb, ptos);
      glRotatef(180, 0, 1, 0);
      dibujarSuperficie(miNurb, ptos);
      generarMallaPorRotacion(cTorre, ptos, 6, 6);
      glTranslatef(0, 5, 0);
      dibujarSuperficie(miNurb, ptos);
      glRotatef(180, 0, 1, 0);
      dibujarSuperficie(miNurb, ptos);
      generarMallaPorRotacion(a, ptos, 6, 6);
    } else if (piezasEgipcias) {
      // lower left side
      glBegin(GL_QUADS);
      glNormal3f(-0.5f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-ROOK_LOWER_WIDTH, 0.0f, -ROOK_LOWER_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-ROOK_LOWER_WIDTH, 0.0f, ROOK_LOWER_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-0.5f, ROOK_HEIGHT_MID, 0.5f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-0.5f, ROOK_HEIGHT_MID, -0.5f);
      glEnd();

      // upper left side
      glBegin(GL_QUADS);
      glNormal3f(-0.5, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-0.5f, ROOK_HEIGHT_MID, -0.5f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-0.5f, ROOK_HEIGHT_MID, 0.5f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, ROOK_UPPER_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, -ROOK_UPPER_WIDTH);
      glEnd();

      // lower front side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-ROOK_LOWER_WIDTH, 0.0f, ROOK_LOWER_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(ROOK_LOWER_WIDTH, 0.0f, ROOK_LOWER_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(0.5f, ROOK_HEIGHT_MID, 0.5f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-0.5f, ROOK_HEIGHT_MID, 0.5f);
      glEnd();

      // upper front side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, -1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-0.5f, ROOK_HEIGHT_MID, 0.5f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(0.5f, ROOK_HEIGHT_MID, 0.5f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, ROOK_UPPER_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, ROOK_UPPER_WIDTH);
      glEnd();

      // lower right side
      glBegin(GL_QUADS);
      glNormal3f(0.5f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(ROOK_LOWER_WIDTH, 0.0f, ROOK_LOWER_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(ROOK_LOWER_WIDTH, 0.0f, -ROOK_LOWER_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(0.5f, ROOK_HEIGHT_MID, -0.5f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.5f, ROOK_HEIGHT_MID, 0.5f);
      glEnd();

      // upper right side
      glBegin(GL_QUADS);
      glNormal3f(0.5f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(0.5f, ROOK_HEIGHT_MID, 0.5f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(0.5f, ROOK_HEIGHT_MID, -0.5f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, -ROOK_UPPER_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, ROOK_UPPER_WIDTH);
      glEnd();

      // lower back side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, -0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(ROOK_LOWER_WIDTH, 0.0f, -ROOK_LOWER_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-ROOK_LOWER_WIDTH, 0.0f, -ROOK_LOWER_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-0.5f, ROOK_HEIGHT_MID, -0.5f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.5f, ROOK_HEIGHT_MID, -0.5f);
      glEnd();

      // upper back side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, -1.0f, -0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(0.5f, ROOK_HEIGHT_MID, -0.5f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-0.5f, ROOK_HEIGHT_MID, -0.5f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, -ROOK_UPPER_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, -ROOK_UPPER_WIDTH);
      glEnd();

      // top plate
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, ROOK_UPPER_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, ROOK_UPPER_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, -ROOK_UPPER_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-ROOK_UPPER_WIDTH, ROOK_HEIGHT_TOP, -ROOK_UPPER_WIDTH);
      glEnd();
    }
  }
  glPopMatrix();
}

void Graficos::dibujarCabezaCaballo() {
  gluBeginSurface(miNurb);
  gluNurbsSurface(miNurb, 8, knots, 8, knots, 4 * 3, 3, &ctlpoints[0][0][0], 4,
                  4, GL_MAP2_VERTEX_3);
  gluEndSurface(miNurb);

  glBegin(GL_QUAD_STRIP);
  { // Cinta de cuadrilateros
    glNormal3f(-1.0, 0.0, 0.0);

    glVertex3f(0, 4, 0);
    glVertex3f(0, 4, -2);
    glVertex3f(0, 2.66, 0);
    glVertex3f(0, 2.66, -2);

    glNormal3f(-0.91, 0.4, 0.0);
    glVertex3f(-1.5, 2, -0.3);
    glVertex3f(-1.5, 2, -1.7);

    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(-1.5, 1.33, -0.3);
    glVertex3f(-1.5, 1.33, -1.7);

    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(0, 1.33, 0);
    glVertex3f(0, 1.33, -2);

    glNormal3f(-0.6, 0.799, 0.0);
    glVertex3f(-1, 0, 0);
    glVertex3f(-1, 0, -2);

    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, -2);
  }
  glEnd();

  glBegin(GL_QUADS);
  { // Cuadrilateros
    glNormal3f(0, 0, 1);
    glVertex3f(0, 2.66, 0);
    glVertex3f(-1.5, 2, -0.3);
    glVertex3f(-1.5, 1.33, -0.3);
    glVertex3f(0, 1.33, 0);
  }
  glEnd();
  glBegin(GL_QUADS);
  { // Cuadrilateros
    glNormal3f(0, 0, -1);
    glVertex3f(0, 2.66, -2);
    glVertex3f(-1.5, 2, -1.7);
    glVertex3f(-1.5, 1.33, -1.7);
    glVertex3f(0, 1.33, -2);
  }
  glEnd();

  glBegin(GL_TRIANGLES);
  { // Cuadrilateros
    glNormal3f(0, 0, 1);
    glVertex3f(0, 1.3, 0);
    glVertex3f(-1, 0, 0);
    glVertex3f(0, 0, 0);
  }
  glEnd();
  glBegin(GL_TRIANGLES);
  { // Cuadrilateros
    glNormal3f(0, 0, -1);
    glVertex3f(0, 1.33, -2);
    glVertex3f(-1, 0, -2);
    glVertex3f(0, 0, -2);
  }
  glEnd();
}

void Graficos::dibujarCaballo() {

  glPushMatrix();
  if (piezasStaunton) {
    // Base de la pieza:
    generarMallaPorRotacion(pCaballo, ptos, 6, 6);
    dibujarSuperficie(miNurb, ptos);
    glRotatef(180, 0, 1, 0);
    dibujarSuperficie(miNurb, ptos);
    // Adorno:
    generarMallaPorRotacion(a, ptos, 6, 6);
    glTranslatef(0, 1.5, 0);
    glScalef(2.7, 2, 2.7);
    dibujarSuperficie(miNurb, ptos);
    glRotatef(180, 0, 1, 0);
    dibujarSuperficie(miNurb, ptos);
    // Cuerpo de la pieza:
    // glTranslatef(-0.5, 2, 0);  corregido en la segunda linea.
    glTranslatef(0, 1, 0); // <<< aqui.
    // glScalef(1.2, 1.2, 1.2);  corregido el tamaño de la cabeza
    glScalef(0.35, 0.35, 0.35); // aqui
    glTranslatef(0, 0, 1);
    dibujarCabezaCaballo();
  } else if (piezasEgipcias) {
    // lower left side
    glBegin(GL_QUADS);
    glNormal3f(-0.5f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_LOWER_WIDTH, 0.0f, -KNIGHT_LOWER_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-KNIGHT_LOWER_WIDTH, 0.0f, KNIGHT_LOWER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, KNIGHT_HEIGHT_MID, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, KNIGHT_HEIGHT_MID, -0.5f);
    glEnd();

    // upper left side
    glBegin(GL_QUADS);
    glNormal3f(-0.5, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, KNIGHT_HEIGHT_MID, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, KNIGHT_HEIGHT_MID, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);
    glEnd();

    // lower front side
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_LOWER_WIDTH, 0.0f, KNIGHT_LOWER_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(KNIGHT_LOWER_WIDTH, 0.0f, KNIGHT_LOWER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, KNIGHT_HEIGHT_MID, 0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, KNIGHT_HEIGHT_MID, 0.5f);
    glEnd();

    // upper front side
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, KNIGHT_HEIGHT_MID, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, KNIGHT_HEIGHT_MID, 0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, KNIGHT_UPPER_WIDTH);
    glEnd();

    // lower right side
    glBegin(GL_QUADS);
    glNormal3f(0.5f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(KNIGHT_LOWER_WIDTH, 0.0f, KNIGHT_LOWER_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(KNIGHT_LOWER_WIDTH, 0.0f, -KNIGHT_LOWER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, KNIGHT_HEIGHT_MID, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, KNIGHT_HEIGHT_MID, 0.5f);
    glEnd();

    // upper right side
    glBegin(GL_QUADS);
    glNormal3f(0.5f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, KNIGHT_HEIGHT_MID, 0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, KNIGHT_HEIGHT_MID, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, KNIGHT_UPPER_WIDTH);
    glEnd();

    // lower back side
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(KNIGHT_LOWER_WIDTH, 0.0f, -KNIGHT_LOWER_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-KNIGHT_LOWER_WIDTH, 0.0f, -KNIGHT_LOWER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-0.5f, KNIGHT_HEIGHT_MID, -0.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.5f, KNIGHT_HEIGHT_MID, -0.5f);
    glEnd();

    // upper back side
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, KNIGHT_HEIGHT_MID, -0.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, KNIGHT_HEIGHT_MID, -0.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);
    glEnd();

    // head
    glBegin(GL_QUADS);
    // left plate
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               -KNIGHT_UPPER_WIDTH);

    // front plate
    glNormal3f(0.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);

    // right plate
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);

    // top plate
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               -KNIGHT_UPPER_WIDTH);
    glEnd();

    // snout
    glBegin(GL_QUADS);
    // left plate
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP - KNIGHT_SNOUT_DROP,
               -KNIGHT_SNOUT_LENGTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_SNOUT_LENGTH);

    // bottom plate
    glNormal3f(0.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP - KNIGHT_SNOUT_DROP,
               -KNIGHT_SNOUT_LENGTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP - KNIGHT_SNOUT_DROP,
               -KNIGHT_SNOUT_LENGTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);

    // right plate
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP - KNIGHT_SNOUT_DROP,
               -KNIGHT_SNOUT_LENGTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_SNOUT_LENGTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               -KNIGHT_UPPER_WIDTH);

    // top plate
    glNormal3f(0.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_SNOUT_LENGTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               -KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_SNOUT_LENGTH);

    // snout plate
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP - KNIGHT_SNOUT_DROP,
               -KNIGHT_SNOUT_LENGTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP - KNIGHT_SNOUT_DROP,
               -KNIGHT_SNOUT_LENGTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_SNOUT_LENGTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP, -KNIGHT_SNOUT_LENGTH);
    glEnd();

    // ears
    glBegin(GL_TRIANGLES);
    // left plates
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH,
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               (KNIGHT_UPPER_WIDTH));
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               (KNIGHT_UPPER_WIDTH / 2.0f));

    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               (KNIGHT_UPPER_WIDTH / 2.0f));
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH,
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               (KNIGHT_UPPER_WIDTH));
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);

    // right plates
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               (KNIGHT_UPPER_WIDTH / 2.0f));
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-(KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-(KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f((KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f((KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f((KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               (KNIGHT_UPPER_WIDTH / 2.0f));
    glEnd();

    // ear plates
    glBegin(GL_QUADS);
    // left back
    glNormal3f(0.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-(KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-(KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH,
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);

    // right back
    glNormal3f(0.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f((KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               KNIGHT_UPPER_WIDTH + KNIGHT_HEAD_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH,
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f((KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);

    // left front
    glNormal3f(0.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-(KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               (KNIGHT_UPPER_WIDTH / 2.0f));
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               (KNIGHT_UPPER_WIDTH / 2.0f));
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-KNIGHT_UPPER_WIDTH,
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-(KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);

    // right front
    glNormal3f(0.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH, KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               (KNIGHT_UPPER_WIDTH / 2.0f));
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f((KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT,
               (KNIGHT_UPPER_WIDTH / 2.0f));
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f((KNIGHT_UPPER_WIDTH - KNIGHT_EAR_WIDTH),
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(KNIGHT_UPPER_WIDTH,
               KNIGHT_HEIGHT_TOP + KNIGHT_HEAD_HEIGHT + KNIGHT_EAR_HEIGHT,
               KNIGHT_UPPER_WIDTH);
    glEnd();
  }
  glPopMatrix();
}

void Graficos::dibujarAlfil() {

  glPushMatrix();
  {
    if (piezasStaunton) {
      // cuerpo
      generarMallaPorRotacion(pAlfil, ptos, 6, 6);
      dibujarSuperficie(miNurb, ptos);
      glRotatef(180, 0, 1, 0);
      dibujarSuperficie(miNurb, ptos);

      // cabeza
      generarMallaPorRotacion(cPeon, ptos, 6, 6);
      glTranslatef(0, 5.5, 0);
      dibujarSuperficie(miNurb, ptos);
      glRotatef(180, 0, 1, 0);
      dibujarSuperficie(miNurb, ptos);

      // disco
      generarMallaPorRotacion(a, ptos, 6, 6);
      // glTranslatef(0, 3.5, 0); corrección a -2
      glTranslatef(0, -2, 0); //<<< aqui
      glScalef(2.2, 2, 2.2);
      dibujarSuperficie(miNurb, ptos);
      glRotatef(180, 0, 1, 0);
      dibujarSuperficie(miNurb, ptos);

    } else if (piezasEgipcias) {
      // left side
      glBegin(GL_QUADS);
      glNormal3f(-0.5f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, BISHOP_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, -BISHOP_NECK_WIDTH);
      glEnd();

      // front side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, BISHOP_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, BISHOP_NECK_WIDTH);
      glEnd();

      // right side
      glBegin(GL_QUADS);
      glNormal3f(0.5f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, -BISHOP_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, BISHOP_NECK_WIDTH);
      glEnd();

      // back side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, -0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, -BISHOP_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, -BISHOP_NECK_WIDTH);
      glEnd();

      // the diamond head
      // lower left side
      glBegin(GL_QUADS);
      glNormal3f(-1.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, -BISHOP_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, BISHOP_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, BISHOP_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID,
                 -BISHOP_HEAD_WIDTH);
      glEnd();

      // upper left side
      glBegin(GL_TRIANGLES);
      glNormal3f(-1.0f, 1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, BISHOP_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.0f, BISHOP_HEIGHT_TOP, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID,
                 -BISHOP_HEAD_WIDTH);
      glEnd();

      // lower front side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, -1.0f, 1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, BISHOP_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, BISHOP_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, BISHOP_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, BISHOP_HEAD_WIDTH);
      glEnd();

      // upper front side
      glBegin(GL_TRIANGLES);
      glNormal3f(0.0f, 1.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, BISHOP_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.0f, BISHOP_HEIGHT_TOP, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, BISHOP_HEAD_WIDTH);
      glEnd();

      // lower right side
      glBegin(GL_QUADS);
      glNormal3f(1.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, BISHOP_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, -BISHOP_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, -BISHOP_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, BISHOP_HEAD_WIDTH);
      glEnd();

      // upper right side
      glBegin(GL_TRIANGLES);
      glNormal3f(1.0f, 1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, -BISHOP_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.0f, BISHOP_HEIGHT_TOP, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, BISHOP_HEAD_WIDTH);
      glEnd();

      // lower back side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, -BISHOP_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-BISHOP_NECK_WIDTH, BISHOP_HEIGHT_MID, -BISHOP_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID,
                 -BISHOP_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, -BISHOP_HEAD_WIDTH);
      glEnd();

      // upper back side
      glBegin(GL_TRIANGLES);
      glNormal3f(0.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID,
                 -BISHOP_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.0f, BISHOP_HEIGHT_TOP, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(BISHOP_HEAD_WIDTH, BISHOP_HEAD_HEIGHT_MID, -BISHOP_HEAD_WIDTH);
      glEnd();
    }
  }
  glPopMatrix();
}

void Graficos::dibujarReina() {

  glPushMatrix();
  {
    if (piezasStaunton) {
      generarMallaPorRotacion(pReina, ptos, 6, 6);
      dibujarSuperficie(miNurb, ptos);
      glRotatef(180, 0, 1, 0);
      dibujarSuperficie(miNurb, ptos);
      generarMallaPorRotacion(cReina, ptos, 6, 6);
      glTranslatef(0, 5, 0);
      dibujarSuperficie(miNurb, ptos);
      glRotatef(180, 0, 1, 0);
      dibujarSuperficie(miNurb, ptos);
    } else if (piezasEgipcias) {
      // left side
      glBegin(GL_QUADS);
      glNormal3f(-0.5f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, QUEEN_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, -QUEEN_NECK_WIDTH);
      glEnd();

      // front side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, QUEEN_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, QUEEN_NECK_WIDTH);
      glEnd();

      // right side
      glBegin(GL_QUADS);
      glNormal3f(0.5f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, -QUEEN_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, QUEEN_NECK_WIDTH);
      glEnd();

      // back side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, -0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, -QUEEN_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, -QUEEN_NECK_WIDTH);
      glEnd();

      // the head
      // lower left side
      glBegin(GL_QUADS);
      glNormal3f(-1.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, -QUEEN_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, QUEEN_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glEnd();

      // lower front side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, -1.0f, 1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, QUEEN_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, QUEEN_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glEnd();

      // lower right side
      glBegin(GL_QUADS);
      glNormal3f(1.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, QUEEN_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, -QUEEN_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glEnd();

      // lower back side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, -QUEEN_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-QUEEN_NECK_WIDTH, QUEEN_HEIGHT_MID, -QUEEN_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glEnd();

      // top plate
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glEnd();

      // left triangles
      glBegin(GL_TRIANGLES);
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEIGHT_TOP, QUEEN_HEAD_WIDTH / 2.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, 0.0f);

      glNormal3f(1.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEIGHT_TOP, QUEEN_HEAD_WIDTH / 2.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);

      glNormal3f(-1.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEIGHT_TOP,
                 -(QUEEN_HEAD_WIDTH / 2.0f));
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);

      glNormal3f(1.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEIGHT_TOP,
                 -(QUEEN_HEAD_WIDTH / 2.0f));
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, 0.0f);
      glEnd();

      // front triangle
      glBegin(GL_TRIANGLES);
      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH / 2.0f, QUEEN_HEIGHT_TOP, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(0.0f, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);

      glNormal3f(0.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(0.0f, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH / 2.0f, QUEEN_HEIGHT_TOP, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);

      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(0.0f, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-(QUEEN_HEAD_WIDTH / 2.0f), QUEEN_HEIGHT_TOP,
                 QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);

      glNormal3f(0.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-(QUEEN_HEAD_WIDTH / 2.0f), QUEEN_HEIGHT_TOP,
                 QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(0.0f, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glEnd();

      // right triangle
      glBegin(GL_TRIANGLES);
      glNormal3f(1.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEIGHT_TOP, QUEEN_HEAD_WIDTH / 2.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, 0.0f);

      glNormal3f(-1.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEIGHT_TOP, QUEEN_HEAD_WIDTH / 2.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, QUEEN_HEAD_WIDTH);

      glNormal3f(1.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEIGHT_TOP,
                 -(QUEEN_HEAD_WIDTH / 2.0f));
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, 0.0f);

      glNormal3f(-1.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, 0.0f);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEIGHT_TOP,
                 -(QUEEN_HEAD_WIDTH / 2.0f));
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glEnd();

      // back triangle
      glBegin(GL_TRIANGLES);
      glNormal3f(0.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-(QUEEN_HEAD_WIDTH / 2.0f), QUEEN_HEIGHT_TOP,
                 -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(0.0f, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);

      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(0.0f, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-(QUEEN_HEAD_WIDTH / 2.0f), QUEEN_HEIGHT_TOP,
                 -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);

      glNormal3f(0.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(0.0f, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH / 2.0f, QUEEN_HEIGHT_TOP, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);

      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(QUEEN_HEAD_WIDTH, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(QUEEN_HEAD_WIDTH / 2.0f, QUEEN_HEIGHT_TOP, -QUEEN_HEAD_WIDTH);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(0.0f, QUEEN_HEAD_HEIGHT_MID, -QUEEN_HEAD_WIDTH);
      glEnd();
    }
  }
  glPopMatrix();
}

void Graficos::dibujarRey() {

  glPushMatrix();
  {
    if (piezasStaunton) {
      dibujarReina();
      // arriba-abajo
      glTranslatef(-0.25, 6.1, -0.25);
      cubo(0.5, 1.7, 0.5);
      // izquierda-derecha
      // glTranslatef(-0.75, 6.8, -0.25); //correccion
      glTranslatef(-0.50, 0.70, 0); // <<< aqui
      cubo(1.5, 0.5, 0.5);
    } else if (piezasEgipcias) {
      // left side
      glBegin(GL_QUADS);
      glNormal3f(-0.5f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-KING_NECK_WIDTH, KING_HEIGHT_MID, KING_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-KING_NECK_WIDTH, KING_HEIGHT_MID, -KING_NECK_WIDTH);
      glEnd();

      // front side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, 0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(KING_NECK_WIDTH, KING_HEIGHT_MID, KING_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-KING_NECK_WIDTH, KING_HEIGHT_MID, KING_NECK_WIDTH);
      glEnd();

      // right side
      glBegin(GL_QUADS);
      glNormal3f(0.5f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, 1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(KING_NECK_WIDTH, KING_HEIGHT_MID, -KING_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(KING_NECK_WIDTH, KING_HEIGHT_MID, KING_NECK_WIDTH);
      glEnd();

      // back side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, -0.5f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-1.0f, 0.0f, -1.0f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-KING_NECK_WIDTH, KING_HEIGHT_MID, -KING_NECK_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(KING_NECK_WIDTH, KING_HEIGHT_MID, -KING_NECK_WIDTH);
      glEnd();

      // the head
      // lower left side
      glBegin(GL_QUADS);
      glNormal3f(-1.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-KING_NECK_WIDTH, KING_HEIGHT_MID, -KING_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-KING_NECK_WIDTH, KING_HEIGHT_MID, KING_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, KING_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, -KING_HEAD_WIDTH);
      glEnd();

      // lower front side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, -1.0f, 1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-KING_NECK_WIDTH, KING_HEIGHT_MID, KING_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(KING_NECK_WIDTH, KING_HEIGHT_MID, KING_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, KING_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, KING_HEAD_WIDTH);
      glEnd();

      // lower right side
      glBegin(GL_QUADS);
      glNormal3f(1.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(KING_NECK_WIDTH, KING_HEIGHT_MID, KING_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(KING_NECK_WIDTH, KING_HEIGHT_MID, -KING_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, -KING_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, KING_HEAD_WIDTH);
      glEnd();

      // lower back side
      glBegin(GL_QUADS);
      glNormal3f(0.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(KING_NECK_WIDTH, KING_HEIGHT_MID, -KING_NECK_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-KING_NECK_WIDTH, KING_HEIGHT_MID, -KING_NECK_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, -KING_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, -KING_HEAD_WIDTH);
      glEnd();

      // top plate
      glBegin(GL_QUADS);
      glNormal3f(0.0f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, KING_HEAD_WIDTH);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, KING_HEAD_WIDTH);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, -KING_HEAD_WIDTH);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-KING_HEAD_WIDTH, KING_HEAD_HEIGHT_MID, -KING_HEAD_WIDTH);
      glEnd();

      // cross
      // horizontal beam
      glBegin(GL_QUADS);
      // left plate
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 -(KING_CROSS_SIZE / 2.0f));

      // right plate
      glNormal3f(1.0f, 0.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 (KING_CROSS_SIZE / 2.0f));

      // front plate
      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 (KING_CROSS_SIZE / 2.0f));

      // back plate
      glNormal3f(0.0f, 0.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 -(KING_CROSS_SIZE / 2.0f));

      // bottom plate
      glNormal3f(0.0f, -1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-KING_CROSS_SIZE,
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE / 2.0f),
                 (KING_CROSS_SIZE / 2.0f));

      // top plate
      glNormal3f(0.0f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(KING_CROSS_SIZE, KING_HEAD_HEIGHT_MID + KING_CROSS_SIZE,
                 (KING_CROSS_SIZE / 2.0f));
      glEnd();

      // vertical beam
      glBegin(GL_QUADS);
      // left plate
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f), KING_HEAD_HEIGHT_MID,
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f), KING_HEAD_HEIGHT_MID,
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 -(KING_CROSS_SIZE / 2.0f));

      // right plate
      glNormal3f(1.0f, 0.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f), KING_HEAD_HEIGHT_MID,
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f), KING_HEAD_HEIGHT_MID,
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 (KING_CROSS_SIZE / 2.0f));

      // front plate
      glNormal3f(0.0f, 0.0f, 1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f), KING_HEAD_HEIGHT_MID,
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f), KING_HEAD_HEIGHT_MID,
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 (KING_CROSS_SIZE / 2.0f));

      // back plate
      glNormal3f(0.0f, 0.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f), KING_HEAD_HEIGHT_MID,
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f), KING_HEAD_HEIGHT_MID,
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 -(KING_CROSS_SIZE / 2.0f));

      // top plate
      glNormal3f(0.0f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 (KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f((KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 -(KING_CROSS_SIZE / 2.0f));
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-(KING_CROSS_SIZE / 2.5f),
                 KING_HEAD_HEIGHT_MID + (KING_CROSS_SIZE * 1.5f),
                 -(KING_CROSS_SIZE / 2.0f));
      glEnd();
    }
  }
  glPopMatrix();
}

//===================================================================

/**
    * Dibuja la pieza seleccionada (objMasterdrez.piezaSeleccionada) en la
  *posicion
  *casillaSeleccionada.
    *
    * @param xact coordenada X de la casilla origen
    * @param yact coordenada Z de la casilla origen
    * @param xfin coordenada X de la casilla destino
    * @param yfin coordenada Z de la casilla destino
    */
void Graficos::dibujarPieza(int xact, int yact, int xfin, int yfin) {
  t_masterdrez::pieza *p1;
  int i;

  // encontrar pieza a mover:
  p1 = NULL;
  for (i = 0; i < 16; i++) {
    if ((objMasterdrez.blancas[i].x == xact) &&
        (objMasterdrez.blancas[i].y) == yact)
      p1 = &(objMasterdrez.blancas[i]);
    if ((objMasterdrez.negras[i].x == xact) &&
        (objMasterdrez.negras[i].y) == yact)
      p1 = &(objMasterdrez.negras[i]);
    if ((objMasterdrez.amarillas[i].x == xact) &&
        (objMasterdrez.amarillas[i].y) == yact)
      p1 = &(objMasterdrez.amarillas[i]);
    if ((objMasterdrez.rojas[i].x == xact) &&
        (objMasterdrez.rojas[i].y) == yact)
      p1 = &(objMasterdrez.rojas[i]);
  }
  if (objMasterdrez.coronando) {
    for (i = 0; i < 4; i++) {
      if ((objMasterdrez.coronables[i].tipo > 0) &&
          (objMasterdrez.coronables[i].x == xact) &&
          (objMasterdrez.coronables[i].y) == yact)
        p1 = &(objMasterdrez.coronables[i]);
    }
  }

  if (!p1) {
    // std::cout << "p1 no existe...\n";
    return;
  } else {
    // std::cout << "::::::::EXTRAÑO::::::::\n";
  }

  /*switch(p1->color)
  {
  case colWhite:
  //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colWhite);
  glMaterialfv(GL_FRONT, GL_SPECULAR, colWhite);
  break;
  case colBlack:
  //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colBlack);
  glMaterialfv(GL_FRONT, GL_SPECULAR, colBlack);
  break;
  case AMARILLO:
  //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colYellow);
  glMaterialfv(GL_FRONT, GL_SPECULAR, colYellow);
  break;
  case colRed:
  //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colRed);
  glMaterialfv(GL_FRONT, GL_SPECULAR, colRed);
  break;
  }*/

  glPushMatrix();
  glTranslatef(5 * xfin + 2.5, 0, 5 * yfin + 2.5);

  switch (p1->tipo) {
  case PEON:
    dibujarPeon();
    break;

  case CABALLO:
    switch (p1->color) {
    case NEGRO:
      glRotatef(-90, 0, 1, 0);
      break;
    case BLANCO:
      glRotatef(90, 0, 1, 0);
      break;
    case AMARILLO:
      glRotatef(180, 0, 1, 0);
      break;
    case ROJO:
      break;
    }
    dibujarCaballo();
    break;

  case ALFIL:
    dibujarAlfil();
    break;

  case TORRE:
    dibujarTorre();
    break;

  case DAMA:
    dibujarReina();
    break;

  case REY:
    dibujarRey();
    break;
  }
  glPopMatrix();
}

/**
* Dibuja las piezas del tablero en la posicion donde se encuentren en el
*mismo.
*
*/
void Graficos::dibujarPiezas() {

  int i;
  glMaterialfv(GL_FRONT, GL_AMBIENT, colBlack);
  glMaterialfv(GL_FRONT, GL_SPECULAR, colWhite);
  glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

  if (objMasterdrez.seleccionandoCasilla) {
    int xact = objMasterdrez.piezaSeleccionada / 16;
    int yact = objMasterdrez.piezaSeleccionada % 16;
    int xfin = objMasterdrez.casillaSeleccionadaActualmente / 16;
    int yfin = objMasterdrez.casillaSeleccionadaActualmente % 16;

    dibujarPieza(xact, yact, xfin, yfin);
  }

  // Piezas Negras:
  // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colBlack);
  // glMaterialfv(GL_FRONT, GL_SPECULAR, colBlack);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, colBlack);

  for (i = 0; i < 16; i++) {
    if ((!objMasterdrez.seleccionandoCasilla) ||
        (objMasterdrez.seleccionandoCasilla &&
         (objMasterdrez.negras[i].x * 16 + objMasterdrez.negras[i].y) !=
             objMasterdrez.piezaSeleccionada))
      switch (objMasterdrez.negras[i].tipo) {
      case PEON:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.negras[i].x + 2.5, 0,
                     5 * objMasterdrez.negras[i].y + 2.5);
        dibujarPeon();
        glPopMatrix();
        break;
      case CABALLO:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.negras[i].x + 2.5, 0,
                     5 * objMasterdrez.negras[i].y + 2.5);
        glRotatef(-90, 0, 1, 0);
        dibujarCaballo();
        glPopMatrix();
        break;
      case ALFIL:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.negras[i].x + 2.5, 0,
                     5 * objMasterdrez.negras[i].y + 2.5);
        dibujarAlfil();
        glPopMatrix();
        break;
      case TORRE:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.negras[i].x + 2.5, 0,
                     5 * objMasterdrez.negras[i].y + 2.5);
        dibujarTorre();
        glPopMatrix();
        break;
      case DAMA:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.negras[i].x + 2.5, 0,
                     5 * objMasterdrez.negras[i].y + 2.5);
        dibujarReina();
        glPopMatrix();
        break;
      case REY:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.negras[i].x + 2.5, 0,
                     5 * objMasterdrez.negras[i].y + 2.5);
        dibujarRey();
        glPopMatrix();
        break;
      }
  }

  // Piezas blancas:
  // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colWhite);
  // glMaterialfv(GL_FRONT, GL_SPECULAR, colWhite);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, colWhite);

  for (i = 0; i < 16; i++) {
    if ((!objMasterdrez.seleccionandoCasilla) ||
        (objMasterdrez.seleccionandoCasilla &&
         (objMasterdrez.blancas[i].x * 16 + objMasterdrez.blancas[i].y) !=
             objMasterdrez.piezaSeleccionada))
      switch (objMasterdrez.blancas[i].tipo) {
      case PEON:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.blancas[i].x + 2.5, 0,
                     5 * objMasterdrez.blancas[i].y + 2.5);
        dibujarPeon();
        glPopMatrix();
        break;
      case CABALLO:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.blancas[i].x + 2.5, 0,
                     5 * objMasterdrez.blancas[i].y + 2.5);
        glRotatef(90, 0, 1, 0);
        dibujarCaballo();
        glPopMatrix();
        break;
      case ALFIL:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.blancas[i].x + 2.5, 0,
                     5 * objMasterdrez.blancas[i].y + 2.5);
        dibujarAlfil();
        glPopMatrix();
        break;
      case TORRE:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.blancas[i].x + 2.5, 0,
                     5 * objMasterdrez.blancas[i].y + 2.5);
        dibujarTorre();
        glPopMatrix();
        break;
      case DAMA:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.blancas[i].x + 2.5, 0,
                     5 * objMasterdrez.blancas[i].y + 2.5);
        dibujarReina();
        glPopMatrix();
        break;
      case REY:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.blancas[i].x + 2.5, 0,
                     5 * objMasterdrez.blancas[i].y + 2.5);
        dibujarRey();
        glPopMatrix();
        break;
      }
  }

  // Piezas rojas:
  // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colRed);
  // glMaterialfv(GL_FRONT, GL_SPECULAR, colRed);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, colRed);

  for (i = 0; i < 16; i++) {
    if ((!objMasterdrez.seleccionandoCasilla) ||
        (objMasterdrez.seleccionandoCasilla &&
         (objMasterdrez.rojas[i].x * 16 + objMasterdrez.rojas[i].y) !=
             objMasterdrez.piezaSeleccionada))
      switch (objMasterdrez.rojas[i].tipo) {
      case PEON:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.rojas[i].x + 2.5, 0,
                     5 * objMasterdrez.rojas[i].y + 2.5);
        dibujarPeon();
        glPopMatrix();
        break;
      case CABALLO:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.rojas[i].x + 2.5, 0,
                     5 * objMasterdrez.rojas[i].y + 2.5);
        dibujarCaballo();
        glPopMatrix();
        break;
      case ALFIL:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.rojas[i].x + 2.5, 0,
                     5 * objMasterdrez.rojas[i].y + 2.5);
        dibujarAlfil();
        glPopMatrix();
        break;
      case TORRE:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.rojas[i].x + 2.5, 0,
                     5 * objMasterdrez.rojas[i].y + 2.5);
        dibujarTorre();
        glPopMatrix();
        break;

      case DAMA:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.rojas[i].x + 2.5, 0,
                     5 * objMasterdrez.rojas[i].y + 2.5);
        dibujarReina();
        glPopMatrix();
        break;
      case REY:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.rojas[i].x + 2.5, 0,
                     5 * objMasterdrez.rojas[i].y + 2.5);
        glRotatef(90, 0, 1, 0);
        dibujarRey();
        glPopMatrix();
        break;
      }
  }

  // Piezas amarillas:
  // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colYellow);
  // glMaterialfv(GL_FRONT, GL_SPECULAR, colYellow);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, colYellow);

  for (i = 0; i < 16; i++) {
    if ((!objMasterdrez.seleccionandoCasilla) ||
        (objMasterdrez.seleccionandoCasilla &&
         (objMasterdrez.amarillas[i].x * 16 + objMasterdrez.amarillas[i].y) !=
             objMasterdrez.piezaSeleccionada))
      switch (objMasterdrez.amarillas[i].tipo) {
      case PEON:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.amarillas[i].x + 2.5, 0,
                     5 * objMasterdrez.amarillas[i].y + 2.5);
        dibujarPeon();
        glPopMatrix();
        break;
      case CABALLO:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.amarillas[i].x + 2.5, 0,
                     5 * objMasterdrez.amarillas[i].y + 2.5);
        glRotatef(180, 0, 1, 0);
        dibujarCaballo();
        glPopMatrix();
        break;
      case ALFIL:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.amarillas[i].x + 2.5, 0,
                     5 * objMasterdrez.amarillas[i].y + 2.5);
        dibujarAlfil();
        glPopMatrix();
        break;
      case TORRE:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.amarillas[i].x + 2.5, 0,
                     5 * objMasterdrez.amarillas[i].y + 2.5);
        dibujarTorre();
        glPopMatrix();
        break;
      case DAMA:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.amarillas[i].x + 2.5, 0,
                     5 * objMasterdrez.amarillas[i].y + 2.5);
        dibujarReina();
        glPopMatrix();
        break;
      case REY:
        glPushMatrix();
        glTranslatef(5 * objMasterdrez.amarillas[i].x + 2.5, 0,
                     5 * objMasterdrez.amarillas[i].y + 2.5);
        glRotatef(90, 0, 1, 0);
        dibujarRey();
        glPopMatrix();
        break;
      }
  }

  // Piezas coronables:
  if (objMasterdrez.coronando == 1) {
    switch (objMasterdrez.colorCoronando) {
    case NEGRO:
      // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colBlack);
      // glMaterialfv(GL_FRONT, GL_SPECULAR, colBlack);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, colBlack);
      break;
    case BLANCO:
      // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colWhite);
      // glMaterialfv(GL_FRONT, GL_SPECULAR, colWhite);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, colWhite);
      break;
    case AMARILLO:
      // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colYellow);
      // glMaterialfv(GL_FRONT, GL_SPECULAR, colYellow);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, colYellow);
      break;
    case ROJO:
      // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colRed);
      // glMaterialfv(GL_FRONT, GL_SPECULAR, colRed);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, colRed);
      break;
    }

    // DAMA
    glPushMatrix();
    glTranslatef(5 * objMasterdrez.coronables[0].x + 2.5, 0,
                 5 * objMasterdrez.coronables[0].y + 2.5);
    dibujarReina();
    glPopMatrix();
    // ALFIL:
    glPushMatrix();
    glTranslatef(5 * objMasterdrez.coronables[1].x + 2.5, 0,
                 5 * objMasterdrez.coronables[1].y + 2.5);
    dibujarAlfil();
    glPopMatrix();
    // CABALLO:
    glPushMatrix();
    glTranslatef(5 * objMasterdrez.coronables[2].x + 2.5, 0,
                 5 * objMasterdrez.coronables[2].y + 2.5);
    switch (objMasterdrez.colorCoronando) {
    case NEGRO:
      glRotatef(-90, 0, 1, 0);
      break;
    case BLANCO:
      glRotatef(90, 0, 1, 0);
      break;
    case AMARILLO:
      glRotatef(180, 0, 1, 0);
      break;
      /*case colRed:
      break;*/
    }

    dibujarCaballo();
    glPopMatrix();
    // TORRE:
    glPushMatrix();
    glTranslatef(5 * objMasterdrez.coronables[3].x + 2.5, 0,
                 5 * objMasterdrez.coronables[3].y + 2.5);
    dibujarTorre();
    glPopMatrix();
  }
}
