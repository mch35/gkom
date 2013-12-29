#include "glut.h"
#include <stdlib.h>
//#include "colors.h"

// sta³e do obs³ugi menu podrêcznego

enum
{
    STENCIL, // bufor szablonowy w³¹cz/wy³¹cz
    FULL_WINDOW, // aspekt obrazu - ca³e okno
    ASPECT_1_1, // aspekt obrazu 1:1
    EXIT // wyjœcie
};

// aspekt obrazu

int aspect = FULL_WINDOW;

// rozmiary bry³y obcinania

const GLdouble left = - 2.0;
const GLdouble right = 2.0;
const GLdouble bottom = - 2.0;
const GLdouble top = 2.0;
const GLdouble near = 3.0;
const GLdouble far = 7.0;

// k¹ty obrotu sceny

GLfloat rotatex = 30.0;
GLfloat rotatey = 0.0;

// k¹t obrotu obiektu

GLfloat angle = 0.0;

// wskaŸnik naciœniêcia lewego przycisku myszki

int button_state = GLUT_UP;

// po³o¿enie kursora myszki

int button_x, button_y;

// identyfikatory list wyœwietlania

GLint GROUND_LIST;
GLint WORLD_LIST;

// po³o¿enie Ÿród³a œwiat³a

GLfloat light_position[ 4 ] =
{
    0.0, 10.0, 10.0, 1.0
};

// u¿ycie bufora szablonowego

bool stencil_test = true;

// funkcja generuj¹ca scenê 3D

void DisplayScene()
{
    // kolor t³a - zawartoœæ bufora koloru
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
   
    // czyszczenie bufora koloru, bufora g³êbokoœci i opcjonalnie bufora szablonowego
    if( stencil_test == true )
         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    else
         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
    // wybór macierzy modelowania
    glMatrixMode( GL_MODELVIEW );
   
    // macierz modelowania = macierz jednostkowa
    glLoadIdentity();
   
    // w³¹czenie testu bufora g³êbokoœci
    glEnable( GL_DEPTH_TEST );
   
    // w³¹czenie oœwietlenia
    glEnable( GL_LIGHTING );
   
    // w³¹czenie œwiat³a GL_LIGHT0
    glEnable( GL_LIGHT0 );
   
    // w³¹czenie automatycznej normalizacji wektorów normalnych
    glEnable( GL_NORMALIZE );
   
    // w³¹czenie obs³ugi w³aœciwoœci materia³ów
    glEnable( GL_COLOR_MATERIAL );
   
    // w³aœciwoœci materia³u okreœlone przez kolor wierzcho³ków
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
   
    // w³¹czenie nierysowania tylnej strony wielok¹tów
    glEnable( GL_CULL_FACE );
   
    // przesuniêcie uk³adu wspó³rzêdnych obiektów do œrodka bry³y odcinania
    glTranslatef( 0, 0, -( near + far ) / 2 );
   
    // obroty ca³ej sceny
    glRotatef( rotatex, 1.0, 0.0, 0.0 );
    glRotatef( rotatey, 0.0, 1.0, 0.0 );
   
    // przy w³¹czonym buforze szblonowym rysujemy "pod³ogê" w celu utworzenia
    // szablonu, który zostanie wykorzystany do narysowania obiektów odbitych
    if( stencil_test == true )
    {
        // wy³¹czenie bufora g³êbokoœci
        glDisable( GL_DEPTH_TEST );
       
        // w³¹czenie bufora szablonowego
        glEnable( GL_STENCIL_TEST );
       
        // test bufora szablonowego
        glStencilFunc( GL_ALWAYS, 0x00000001, 0xFFFFFFFF );
       
        // okreœlenie operacji na buforze szablonowym
        glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
       
        // wy³¹czenie zapisu sk³adowych RGBA do bufora kolorów
        glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
       
        // wyœwietlenie p³aszczyzny odbicia
        glCallList( GROUND_LIST );
       
        // w³¹czenie zapisu sk³adowych RGBA do bufora kolorów
        glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
       
        // w³¹czenie testu bufora g³êbokoœci
        glEnable( GL_DEPTH_TEST );
       
        // test bufora szablonowego
        glStencilFunc( GL_EQUAL, 0x00000001, 0xFFFFFFFF );
       
        // okreœlenie operacji na buforze szablonowym
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
    }
   
    // od³o¿enie macierzy modelowania na stos
    glPushMatrix();
   
    // skalowanie macierzy modelowania w celu narysownaia obiektów œwiata "lustrzanego"
    glScalef( 1.0, - 1.0, 1.0 );
   
    // po³o¿enie "lustrzanego" Ÿród³a œwiat³a
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
   
    // przesuniêcie obiektów œwiata "lustrzanego"
    glTranslatef( 0.0, 1.2, 0.0 );
   
    // obroty obiektów œwiata "lustrzanego"
    glRotatef( angle, 1.0, 0.0, 0.0 );
    glRotatef( angle, 0.0, 2.0, 0.0 );
    glRotatef( angle, 0.0, 0.0, - 1.0 );
   
    // w œwiecie lustrzanym trzeba zmieniæ orientacjê wierzcho³ków prymitywów
    glFrontFace( GL_CW );
   
    // wyœwietlenie obiektów œwiata "lustrzanego"
    glCallList( WORLD_LIST );
   
    // powrót do normalnej orientacji wierzcho³ków prymitywów
    glFrontFace( GL_CCW );
   
    // zdjêcie macierzy modelowania ze stosu
    glPopMatrix();
   
    // koniec rysowania obiektów œwiata "lustrznaego" to tak¿e
    // koniec korzystania z bufora szablonowego
    if( stencil_test == true )
    {
        // wy³¹czenie bufora szablonowego
        glDisable( GL_STENCIL_TEST );
    }
   
    // wy³¹czenie oœwietlenia
    glDisable( GL_LIGHTING );
   
    // w³¹czenie mieszania kolorów
    glEnable( GL_BLEND );
   
    // wspó³czynniki równania mieszania kolorów
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   
    // wyœwietlenie p³aszczyzny odbicia
    glCallList( GROUND_LIST );
   
    // wy³¹czenie mieszania kolorów
    glDisable( GL_BLEND );
   
    // w³¹czenie oœwietlenia
    glEnable( GL_LIGHTING );
   
    // po³o¿enie Ÿród³a œwiat³a
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
   
    // przesuniêcie obiektów œwiata
    glTranslatef( 0.0, 1.2, 0.0 );
   
    // obroty obiektów œwiata
    glRotatef( angle, 1.0, 0.0, 0.0 );
    glRotatef( angle, 0.0, 2.0, 0.0 );
    glRotatef( angle, 0.0, 0.0, - 1.0 );
   
    // wyœwietlenie obiektów œwiata
    glCallList( WORLD_LIST );
   
    // skierowanie poleceñ do wykonania
    glFlush();
   
    // zamiana buforów koloru
    glutSwapBuffers();
}

// funkcja timera

void Timer( int value )
{
    // zwiêkszenie k¹ta obrotu obiektu sceny
    angle++;
   
    // wyœwietlenie sceny
    DisplayScene();
   
    // nastêpne wywo³anie funkcji timera
    glutTimerFunc( 20, Timer, 0 );
}

// zmiana wielkoœci okna

void Reshape( int width, int height )
{
    // obszar renderingu - ca³e okno
    glViewport( 0, 0, width, height );
   
    // wybór macierzy rzutowania
    glMatrixMode( GL_PROJECTION );
   
    // macierz rzutowania = macierz jednostkowa
    glLoadIdentity();
   
    // parametry bry³y obcinania
    if( aspect == ASPECT_1_1 )
    {
        // wysokoœæ okna wiêksza od wysokoœci okna
        if( width < height && width > 0 )
             glFrustum( left, right, bottom * height / width, top * height / width, near, far );
        else
       
        // szerokoœæ okna wiêksza lub równa wysokoœci okna
        if( width >= height && height > 0 )
             glFrustum( left * width / height, right * width / height, bottom, top, near, far );
       
    }
    else
         glFrustum( left, right, bottom, top, near, far );
   
    // generowanie sceny 3D
    DisplayScene();
}

// obs³uga przycisków myszki

void MouseButton( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {
        // zapamiêtanie stanu lewego przycisku myszki
        button_state = state;
       
        // zapamiêtanie po³o¿enia kursora myszki
        if( state == GLUT_DOWN )
        {
            button_x = x;
            button_y = y;
        }
    }
}

// obs³uga ruchu kursora myszki

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

// obs³uga menu podrêcznego

void Menu( int value )
{
    switch( value )
    {
        // bufor szablonowy w³¹cz/wy³¹cz
    case STENCIL:
        stencil_test = !stencil_test;
        DisplayScene();
        break;
       
        // obszar renderingu - ca³e okno
    case FULL_WINDOW:
        aspect = FULL_WINDOW;
        Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
        break;
       
        // obszar renderingu - aspekt 1:1
    case ASPECT_1_1:
        aspect = ASPECT_1_1;
        Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
        break;
       
        // wyjœcie
    case EXIT:
        exit( 0 );
    }
}

// utworzenie list wyœwietlania

void GenerateDisplayLists()
{
    // generowanie identyfikatora pierwszej listy wyœwietlania
    GROUND_LIST = glGenLists( 1 );
   
    // pierwsza lista wyœwietlania - pod³oga
    glNewList( GROUND_LIST, GL_COMPILE );
   
    // "pod³oga" rysowana jest jako szachownica o rozmiarach
    // 4 x 4 jednostki i zawiera siê w p³aszczyŸnie y = 0
    glBegin( GL_QUADS );
    glNormal3f( 1.0, 0.0, 0.0 );
   
    // przy ka¿dej iteracji pêtli rysowane s¹ cztery prostok¹ty szachownicy
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
   
    // koniec pierwszej listy wyœwietlania
    glEndList();
   
    // generowanie identyfikatora drugiej listy wyœwietlania
    WORLD_LIST = glGenLists( 1 );
   
    // druga lista wyœwietlania - obiekt "œwiata"
    glNewList( WORLD_LIST, GL_COMPILE );
   
    // zielony torus
    glColor4fv( Green );
    glutSolidTorus( 0.3, 0.7, 50, 40 );
   
    // koniec drugiej listy wyœwietlania
    glEndList();
}

int main( int argc, char * argv[] )
{
    // inicjalizacja biblioteki GLUT
    glutInit( & argc, argv );
   
    // inicjalizacja bufora ramki
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL );
   
    // rozmiary g³ównego okna programu
    glutInitWindowSize( 500, 500 );
   
    // utworzenie g³ównego okna programu
    glutCreateWindow( "Odbicie" );
   
    // do³¹czenie funkcji generuj¹cej scenê 3D
    glutDisplayFunc( DisplayScene );
   
    // do³¹czenie funkcji wywo³ywanej przy zmianie rozmiaru okna
    glutReshapeFunc( Reshape );
   
    // obs³uga przycisków myszki
    glutMouseFunc( MouseButton );
   
    // obs³uga ruchu kursora myszki
    glutMotionFunc( MouseMotion );
   
    // utworzenie podmenu - Aspekt obrazu
    int MenuAspect = glutCreateMenu( Menu );
    #ifdef WIN32
   
    glutAddMenuEntry( "Aspekt obrazu - ca³e okno", FULL_WINDOW );
    #else
   
    glutAddMenuEntry( "Aspekt obrazu - cale okno", FULL_WINDOW );
    #endif
   
    glutAddMenuEntry( "Aspekt obrazu 1:1", ASPECT_1_1 );
   
    // menu g³ówne
    glutCreateMenu( Menu );
   
    #ifdef WIN32
   
    glutAddMenuEntry( "Bufor szablonowy w³¹cz/wy³¹cz", STENCIL );
    glutAddSubMenu( "Aspekt obrazu", MenuAspect );
    glutAddMenuEntry( "Wyjœcie", EXIT );
    #else
   
    glutAddMenuEntry( "Bufor szablonowy wlacz/wylacz", STENCIL );
    glutAddSubMenu( "Aspekt obrazu", MenuAspect );
    glutAddMenuEntry( "Wyjscie", EXIT );
    #endif
   
    // okreœlenie przycisku myszki obs³uguj¹cego menu podrêczne
    glutAttachMenu( GLUT_RIGHT_BUTTON );
   
    // utworzenie list wyœwietlania
    GenerateDisplayLists();
   
    // wywo³anie funkcji timera
    glutTimerFunc( 20, Timer, 0 );
   
    // wprowadzenie programu do obs³ugi pêtli komunikatów
    glutMainLoop();
    return 0;
}