#include "glut.h"
#include <stdlib.h>
//#include "colors.h"

// sta�e do obs�ugi menu podr�cznego

enum
{
    STENCIL, // bufor szablonowy w��cz/wy��cz
    FULL_WINDOW, // aspekt obrazu - ca�e okno
    ASPECT_1_1, // aspekt obrazu 1:1
    EXIT // wyj�cie
};

// aspekt obrazu

int aspect = FULL_WINDOW;

// rozmiary bry�y obcinania

const GLdouble left = - 2.0;
const GLdouble right = 2.0;
const GLdouble bottom = - 2.0;
const GLdouble top = 2.0;
const GLdouble near = 3.0;
const GLdouble far = 7.0;

// k�ty obrotu sceny

GLfloat rotatex = 30.0;
GLfloat rotatey = 0.0;

// k�t obrotu obiektu

GLfloat angle = 0.0;

// wska�nik naci�ni�cia lewego przycisku myszki

int button_state = GLUT_UP;

// po�o�enie kursora myszki

int button_x, button_y;

// identyfikatory list wy�wietlania

GLint GROUND_LIST;
GLint WORLD_LIST;

// po�o�enie �r�d�a �wiat�a

GLfloat light_position[ 4 ] =
{
    0.0, 10.0, 10.0, 1.0
};

// u�ycie bufora szablonowego

bool stencil_test = true;

// funkcja generuj�ca scen� 3D

void DisplayScene()
{
    // kolor t�a - zawarto�� bufora koloru
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
   
    // czyszczenie bufora koloru, bufora g��boko�ci i opcjonalnie bufora szablonowego
    if( stencil_test == true )
         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    else
         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
    // wyb�r macierzy modelowania
    glMatrixMode( GL_MODELVIEW );
   
    // macierz modelowania = macierz jednostkowa
    glLoadIdentity();
   
    // w��czenie testu bufora g��boko�ci
    glEnable( GL_DEPTH_TEST );
   
    // w��czenie o�wietlenia
    glEnable( GL_LIGHTING );
   
    // w��czenie �wiat�a GL_LIGHT0
    glEnable( GL_LIGHT0 );
   
    // w��czenie automatycznej normalizacji wektor�w normalnych
    glEnable( GL_NORMALIZE );
   
    // w��czenie obs�ugi w�a�ciwo�ci materia��w
    glEnable( GL_COLOR_MATERIAL );
   
    // w�a�ciwo�ci materia�u okre�lone przez kolor wierzcho�k�w
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
   
    // w��czenie nierysowania tylnej strony wielok�t�w
    glEnable( GL_CULL_FACE );
   
    // przesuni�cie uk�adu wsp�rz�dnych obiekt�w do �rodka bry�y odcinania
    glTranslatef( 0, 0, -( near + far ) / 2 );
   
    // obroty ca�ej sceny
    glRotatef( rotatex, 1.0, 0.0, 0.0 );
    glRotatef( rotatey, 0.0, 1.0, 0.0 );
   
    // przy w��czonym buforze szblonowym rysujemy "pod�og�" w celu utworzenia
    // szablonu, kt�ry zostanie wykorzystany do narysowania obiekt�w odbitych
    if( stencil_test == true )
    {
        // wy��czenie bufora g��boko�ci
        glDisable( GL_DEPTH_TEST );
       
        // w��czenie bufora szablonowego
        glEnable( GL_STENCIL_TEST );
       
        // test bufora szablonowego
        glStencilFunc( GL_ALWAYS, 0x00000001, 0xFFFFFFFF );
       
        // okre�lenie operacji na buforze szablonowym
        glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
       
        // wy��czenie zapisu sk�adowych RGBA do bufora kolor�w
        glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
       
        // wy�wietlenie p�aszczyzny odbicia
        glCallList( GROUND_LIST );
       
        // w��czenie zapisu sk�adowych RGBA do bufora kolor�w
        glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
       
        // w��czenie testu bufora g��boko�ci
        glEnable( GL_DEPTH_TEST );
       
        // test bufora szablonowego
        glStencilFunc( GL_EQUAL, 0x00000001, 0xFFFFFFFF );
       
        // okre�lenie operacji na buforze szablonowym
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
    }
   
    // od�o�enie macierzy modelowania na stos
    glPushMatrix();
   
    // skalowanie macierzy modelowania w celu narysownaia obiekt�w �wiata "lustrzanego"
    glScalef( 1.0, - 1.0, 1.0 );
   
    // po�o�enie "lustrzanego" �r�d�a �wiat�a
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
   
    // przesuni�cie obiekt�w �wiata "lustrzanego"
    glTranslatef( 0.0, 1.2, 0.0 );
   
    // obroty obiekt�w �wiata "lustrzanego"
    glRotatef( angle, 1.0, 0.0, 0.0 );
    glRotatef( angle, 0.0, 2.0, 0.0 );
    glRotatef( angle, 0.0, 0.0, - 1.0 );
   
    // w �wiecie lustrzanym trzeba zmieni� orientacj� wierzcho�k�w prymityw�w
    glFrontFace( GL_CW );
   
    // wy�wietlenie obiekt�w �wiata "lustrzanego"
    glCallList( WORLD_LIST );
   
    // powr�t do normalnej orientacji wierzcho�k�w prymityw�w
    glFrontFace( GL_CCW );
   
    // zdj�cie macierzy modelowania ze stosu
    glPopMatrix();
   
    // koniec rysowania obiekt�w �wiata "lustrznaego" to tak�e
    // koniec korzystania z bufora szablonowego
    if( stencil_test == true )
    {
        // wy��czenie bufora szablonowego
        glDisable( GL_STENCIL_TEST );
    }
   
    // wy��czenie o�wietlenia
    glDisable( GL_LIGHTING );
   
    // w��czenie mieszania kolor�w
    glEnable( GL_BLEND );
   
    // wsp�czynniki r�wnania mieszania kolor�w
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   
    // wy�wietlenie p�aszczyzny odbicia
    glCallList( GROUND_LIST );
   
    // wy��czenie mieszania kolor�w
    glDisable( GL_BLEND );
   
    // w��czenie o�wietlenia
    glEnable( GL_LIGHTING );
   
    // po�o�enie �r�d�a �wiat�a
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
   
    // przesuni�cie obiekt�w �wiata
    glTranslatef( 0.0, 1.2, 0.0 );
   
    // obroty obiekt�w �wiata
    glRotatef( angle, 1.0, 0.0, 0.0 );
    glRotatef( angle, 0.0, 2.0, 0.0 );
    glRotatef( angle, 0.0, 0.0, - 1.0 );
   
    // wy�wietlenie obiekt�w �wiata
    glCallList( WORLD_LIST );
   
    // skierowanie polece� do wykonania
    glFlush();
   
    // zamiana bufor�w koloru
    glutSwapBuffers();
}

// funkcja timera

void Timer( int value )
{
    // zwi�kszenie k�ta obrotu obiektu sceny
    angle++;
   
    // wy�wietlenie sceny
    DisplayScene();
   
    // nast�pne wywo�anie funkcji timera
    glutTimerFunc( 20, Timer, 0 );
}

// zmiana wielko�ci okna

void Reshape( int width, int height )
{
    // obszar renderingu - ca�e okno
    glViewport( 0, 0, width, height );
   
    // wyb�r macierzy rzutowania
    glMatrixMode( GL_PROJECTION );
   
    // macierz rzutowania = macierz jednostkowa
    glLoadIdentity();
   
    // parametry bry�y obcinania
    if( aspect == ASPECT_1_1 )
    {
        // wysoko�� okna wi�ksza od wysoko�ci okna
        if( width < height && width > 0 )
             glFrustum( left, right, bottom * height / width, top * height / width, near, far );
        else
       
        // szeroko�� okna wi�ksza lub r�wna wysoko�ci okna
        if( width >= height && height > 0 )
             glFrustum( left * width / height, right * width / height, bottom, top, near, far );
       
    }
    else
         glFrustum( left, right, bottom, top, near, far );
   
    // generowanie sceny 3D
    DisplayScene();
}

// obs�uga przycisk�w myszki

void MouseButton( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {
        // zapami�tanie stanu lewego przycisku myszki
        button_state = state;
       
        // zapami�tanie po�o�enia kursora myszki
        if( state == GLUT_DOWN )
        {
            button_x = x;
            button_y = y;
        }
    }
}

// obs�uga ruchu kursora myszki

void MouseMotion( int x, int y )
{
    if( button_state == GLUT_DOWN )
    {
        rotatey += 30 *( right - left ) / glutGet( GLUT_WINDOW_WIDTH ) *( x - button_x );
        button_x = x;
        rotatex -= 30 *( top - bottom ) / glutGet( GLUT_WINDOW_HEIGHT ) *( button_y - y );
        button_y = y;
        glutPostRedisplay();
    }
}

// obs�uga menu podr�cznego

void Menu( int value )
{
    switch( value )
    {
        // bufor szablonowy w��cz/wy��cz
    case STENCIL:
        stencil_test = !stencil_test;
        DisplayScene();
        break;
       
        // obszar renderingu - ca�e okno
    case FULL_WINDOW:
        aspect = FULL_WINDOW;
        Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
        break;
       
        // obszar renderingu - aspekt 1:1
    case ASPECT_1_1:
        aspect = ASPECT_1_1;
        Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
        break;
       
        // wyj�cie
    case EXIT:
        exit( 0 );
    }
}

// utworzenie list wy�wietlania

void GenerateDisplayLists()
{
    // generowanie identyfikatora pierwszej listy wy�wietlania
    GROUND_LIST = glGenLists( 1 );
   
    // pierwsza lista wy�wietlania - pod�oga
    glNewList( GROUND_LIST, GL_COMPILE );
   
    // "pod�oga" rysowana jest jako szachownica o rozmiarach
    // 4 x 4 jednostki i zawiera si� w p�aszczy�nie y = 0
    glBegin( GL_QUADS );
    glNormal3f( 1.0, 0.0, 0.0 );
   
    // przy ka�dej iteracji p�tli rysowane s� cztery prostok�ty szachownicy
    for( GLfloat z = - 2.0; z < 2.0; z += 1 )
    for( GLfloat x = - 2.0; x < 2.0; x += 1 )
    {
        glColor4f( Blue[ 0 ], Blue[ 1 ], Blue[ 2 ], 0.5 );
        glVertex3f( x + 0.0, 0.0, z + 0.0 );
        glVertex3f( x + 0.0, 0.0, z + 0.5 );
        glVertex3f( x + 0.5, 0.0, z + 0.5 );
        glVertex3f( x + 0.5, 0.0, z + 0.0 );
        glColor4f( Silver[ 0 ], Silver[ 1 ], Silver[ 2 ], 0.7 );
        glVertex3f( x + 0.5, 0.0, z + 0.0 );
        glVertex3f( x + 0.5, 0.0, z + 0.5 );
        glVertex3f( x + 1.0, 0.0, z + 0.5 );
        glVertex3f( x + 1.0, 0.0, z + 0.0 );
        glColor4f( Blue[ 0 ], Blue[ 1 ], Blue[ 2 ], 0.5 );
        glVertex3f( x + 0.5, 0.0, z + 0.5 );
        glVertex3f( x + 0.5, 0.0, z + 1.0 );
        glVertex3f( x + 1.0, 0.0, z + 1.0 );
        glVertex3f( x + 1.0, 0.0, z + 0.5 );
        glColor4f( Silver[ 0 ], Silver[ 1 ], Silver[ 2 ], 0.7 );
        glVertex3f( x + 0.0, 0.0, z + 0.5 );
        glVertex3f( x + 0.0, 0.0, z + 1.0 );
        glVertex3f( x + 0.5, 0.0, z + 1.0 );
        glVertex3f( x + 0.5, 0.0, z + 0.5 );
    }
    glEnd();
   
    // koniec pierwszej listy wy�wietlania
    glEndList();
   
    // generowanie identyfikatora drugiej listy wy�wietlania
    WORLD_LIST = glGenLists( 1 );
   
    // druga lista wy�wietlania - obiekt "�wiata"
    glNewList( WORLD_LIST, GL_COMPILE );
   
    // zielony torus
    glColor4fv( Green );
    glutSolidTorus( 0.3, 0.7, 50, 40 );
   
    // koniec drugiej listy wy�wietlania
    glEndList();
}

int main( int argc, char * argv[] )
{
    // inicjalizacja biblioteki GLUT
    glutInit( & argc, argv );
   
    // inicjalizacja bufora ramki
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL );
   
    // rozmiary g��wnego okna programu
    glutInitWindowSize( 500, 500 );
   
    // utworzenie g��wnego okna programu
    glutCreateWindow( "Odbicie" );
   
    // do��czenie funkcji generuj�cej scen� 3D
    glutDisplayFunc( DisplayScene );
   
    // do��czenie funkcji wywo�ywanej przy zmianie rozmiaru okna
    glutReshapeFunc( Reshape );
   
    // obs�uga przycisk�w myszki
    glutMouseFunc( MouseButton );
   
    // obs�uga ruchu kursora myszki
    glutMotionFunc( MouseMotion );
   
    // utworzenie podmenu - Aspekt obrazu
    int MenuAspect = glutCreateMenu( Menu );
    #ifdef WIN32
   
    glutAddMenuEntry( "Aspekt obrazu - ca�e okno", FULL_WINDOW );
    #else
   
    glutAddMenuEntry( "Aspekt obrazu - cale okno", FULL_WINDOW );
    #endif
   
    glutAddMenuEntry( "Aspekt obrazu 1:1", ASPECT_1_1 );
   
    // menu g��wne
    glutCreateMenu( Menu );
   
    #ifdef WIN32
   
    glutAddMenuEntry( "Bufor szablonowy w��cz/wy��cz", STENCIL );
    glutAddSubMenu( "Aspekt obrazu", MenuAspect );
    glutAddMenuEntry( "Wyj�cie", EXIT );
    #else
   
    glutAddMenuEntry( "Bufor szablonowy wlacz/wylacz", STENCIL );
    glutAddSubMenu( "Aspekt obrazu", MenuAspect );
    glutAddMenuEntry( "Wyjscie", EXIT );
    #endif
   
    // okre�lenie przycisku myszki obs�uguj�cego menu podr�czne
    glutAttachMenu( GLUT_RIGHT_BUTTON );
   
    // utworzenie list wy�wietlania
    GenerateDisplayLists();
   
    // wywo�anie funkcji timera
    glutTimerFunc( 20, Timer, 0 );
   
    // wprowadzenie programu do obs�ugi p�tli komunikat�w
    glutMainLoop();
    return 0;
}