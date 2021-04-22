#include "Drawer.h"
#include "Game.h"
#include "TexturesCodes.h"

Drawer::Drawer(Game *game) {
    this->game = game;
}

void Drawer::clear() {
    textureIDs_.clear();
}


void Drawer::drawAmbientColor(Color * color) {
    GLfloat ambientColor[] = {color->getRed(),color->getGreen(),color->getBlue(), 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
}


void Drawer::drawLight(Light * light) {
    Color ambientColor = light->getAmbientColor();
    Color diffusionColor = light->getDiffusionColor();
    Point position;
    Vector direction;
    bool isPositional = light->isPositional;
    int index = light->getIndex();

    GLfloat color1[] = {ambientColor.getRed(),ambientColor.getGreen(),ambientColor.getBlue(), 1.0f};
    glLightfv(GL_LIGHT0+index, GL_AMBIENT, color1);

    GLfloat color2[] = {diffusionColor.getRed(),diffusionColor.getGreen(),diffusionColor.getBlue(), 1.0f};
    glLightfv(GL_LIGHT0+index, GL_DIFFUSE, color2);

    if (isPositional) {
        position = light->getPosition();
        GLfloat lightPosition[] = {position.coord[0],position.coord[1],position.coord[2],1.0f};
        glLightfv(GL_LIGHT0+index, GL_POSITION, lightPosition);
    } else {
        direction = light->getDirection();
        GLfloat lightDirection[] = {direction.coord[0],direction.coord[1],direction.coord[2],0.0f};
        glLightfv(GL_LIGHT0+index, GL_POSITION, lightDirection);
    }
}


void Drawer::drawObject(Object * object) {
    int i,j;
    int polygon_number;
    int vertices_number;
    vector <Polygon*> polygons;
    vector <Point> vertices;
    Polygon* polygon;
    Color color;
    Transformer transformer;
    vector <Vector> normals;
    unsigned int textureID;
    vector <Point> textureCoords;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    transformer.translate(object->getTranslation());
    transformer.rotate(object->getRotation());
    transformer.scale(object->getScalation());

    polygons = object->getPolygons();
    polygon_number = polygons.size();


    for (i=0; i<polygon_number; i++) {
        polygon = polygons[i];
        vertices = polygon->getVertices();
        vertices_number = vertices.size();
        normals = polygon->getNormals();
        color = polygon->getColor();
        glColor3f(color.getRed(),color.getGreen(),color.getBlue());


        if (polygon->hasTextureCode()) {
            textureID = textureCode2ID_[polygon->getTextureCode()];
            textureCoords = polygon->getTextureCoords();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        glBegin(GL_POLYGON);
        for (j=0; j<vertices_number; j++) {
            glNormal3f(normals[j].coord[0],normals[j].coord[1],normals[j].coord[2]);
            if (polygon->hasTextureCode()) {
                glTexCoord2f(textureCoords[j].coord[0],textureCoords[j].coord[1]);
            }
            glVertex3f(vertices[j].coord[0],vertices[j].coord[1],vertices[j].coord[2]);
        }
        glEnd();

        if (polygon->hasTextureCode()) {
            glDisable(GL_TEXTURE_2D);
        }
    }

    glPopMatrix();

}


void Drawer::loadTextures() {
    auto objects = game->scene->getObjects();
    for (auto it=objects.begin(); it!=objects.end(); it++) {
        auto polygons = (*it)->getPolygons();
        for (auto it2=polygons.begin(); it2!=polygons.end(); it2++) {
            auto p = *it2;
            auto textureCode = p->getTextureCode();
            unsigned int textureID;
            if (!textureCode.empty()) {
                auto it = textureCode2ID_.find(textureCode);
                if (it == textureCode2ID_.end()) {
                    glGenTextures(1, &textureID);
                    textureCode2ID_[textureCode] = textureID;
                    loadResource(textureID, textureCode);
                }
            }
        }
    }
}


void Drawer::loadResource(unsigned int textureID, string textureCode) {
    glBindTexture(GL_TEXTURE_2D, textureID);

    char dest[PATH_MAX];
    memset(dest, 0, sizeof(dest));
    if (readlink("/proc/self/exe", dest, PATH_MAX) == -1) {
        abort();
    }

    // load file in memory
    ifstream file(dest, std::ios::binary | std::ios::ate);
    streamsize exeSize = file.tellg();
    file.seekg(0, std::ios::beg);

    auto exe = new unsigned char[exeSize];
    file.read((char*)exe, exeSize);

    string pattern = "X" + textureCode + "X";

    unsigned char *ptr = (unsigned char *) memmem(exe, exeSize, pattern.c_str(), pattern.length());
    if (ptr == NULL) {
        abort();
    }

    ptr += CODE_LEN + 2;

    unsigned char *xorPattern = ptr;
    ptr += PATTERN_LEN;

    for (unsigned int i=0; i<4; i++) {
        (*(ptr+i)) ^= xorPattern[i % PATTERN_LEN];
    }
    unsigned int len = *((unsigned int *)ptr);

    auto res = ptr+4;
    for (unsigned int i=4; i<len; i++) {
        (*(ptr+i)) ^= xorPattern[i % PATTERN_LEN];
    }

    res += 18;
    auto width = *((unsigned int *) res);
    res += 4;
    auto height = *((unsigned int *) res);
    res += 4;
    auto planes = *((unsigned short *) res);
    if (planes != 1) abort();
    res += 2;
    auto bpp = *((unsigned short *) res);
    if (bpp != 24) abort();
    res += 2;

    res += 24;
    auto pixels = res;
    auto bmpSize = width * height * 3;

    // shuffle pixels (taken from old code)
    unsigned char tmp;
    for( unsigned int i=0; i < bmpSize; i += 3 )
    {
        tmp = pixels[i];
        pixels[i] = pixels[i+2];
        pixels[i+2] = tmp;
    }

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 width,
                 height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 pixels);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    delete []exe;
}


void Drawer::drawScene() {
    auto scene = game->scene;
    Color background_color;

    background_color = scene->getBackgroundColor();
    glClearColor(background_color.getRed(),background_color.getGreen(),background_color.getBlue(),1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto lights = scene->getLights();
    for (auto it=lights.begin(); it!=lights.end(); it++) {
        auto l = *it;
        drawLight(l);
    }

    auto objects = scene->getObjects();
    for (auto it=objects.begin(); it!=objects.end(); it++) {
        auto o = *it;
        drawObject(o);
    }

    auto camera = game->camera;
    int cx = ((int) (camera->getTX() / 1000)) * 1000;
    int cz = ((int) (camera->getTZ() / 1000)) * 1000;

    int radius = 5000;
    for (int x=cx-radius; x<=cx+radius; x+=1000) {
        for (int z=cz-radius; z<=cz+radius; z+=1000) {
            auto c = make_pair(x, z);
            auto objects = scene->objectsNodesByCoords[c];
            for (auto it=objects.begin(); it!=objects.end(); it++) {
                auto o = *it;
                drawObject(o);
            }
        }
    }

    drawLevelText();

    drawFlag();

    if (!game->connected && game->flag.empty()) {
        drawDisconnected();
    }
}


void Drawer::setCameraProjection(Camera * camera, Window * window) {
    float width;
    float height;
    float zoom;

    width = window->getWidth();
    height = window->getHeight();
    zoom = camera->getZoom();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(zoom, width/height, 0.1, 5000.0);
    glMatrixMode(GL_MODELVIEW);
}


void Drawer::setCameraPosition(Camera * camera) {
    float tx,ty,tz;
    float angleX,angleY,angleZ;
    float centerMatrix[16];

    tx = camera->getTX();
    ty = camera->getTY();
    tz = camera->getTZ();

    angleX = camera->getAngleX();
    angleY = camera->getAngleY();
    angleZ = camera->getAngleZ();

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(tx,ty,tz);
    glRotatef(angleY,0,1,0);
    glRotatef(angleX,1,0,0);
    glRotatef(angleZ,0,0,1);
    glTranslatef(0,0,-1);
    glGetFloatv(GL_MODELVIEW_MATRIX,centerMatrix);
    glPopMatrix();

    gluLookAt(tx,ty,tz,
        centerMatrix[12],centerMatrix[13],centerMatrix[14],
        centerMatrix[4],centerMatrix[5],centerMatrix[6]);
}


void Drawer::drawLevelText() {
    ostringstream ss;
    ss << "Level " << game->level;
    string s = ss.str();
    drawText(s, 10, 10);
}


void Drawer::drawDisconnected() {
    auto w = game->window->getWidth();
    auto h = game->window->getHeight();
    string s = "Disconnected :-(";
    drawText(s, w/2-30, h/2);
}


void Drawer::drawFlag() {
    auto h = game->window->getHeight();
    if (!game->flag.empty()) {
        drawText(game->flag, 2, h/2-30);
    }
}


void Drawer::drawText(string s, int x, int y) {
    auto w = game->window->getWidth();
    auto h = game->window->getHeight();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, w, 0.0, h);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(x, y);

    for (auto it = s.begin(); it != s.end(); it++) {
        char c = *it;
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
