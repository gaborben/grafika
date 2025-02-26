#include "scene.h"

#include <time.h>

#include <obj/load.h>
#include <obj/draw.h>

void init_scene(Scene* scene)
{
    load_model(&(scene->cube), "assets/models/cube.obj");
    scene->texture_id = load_texture("assets/textures/cube.png");
    scene->charmap_id = load_texture("assets/textures/charmap.png");

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;
}

void set_lighting()
{
    float ambient_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[] = { 0.0f, 5.0f, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene)
{
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_origin();
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);
    draw_model(&(scene->cube));
    draw_timestamp(scene);
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}

void draw_timestamp(const Scene* scene)
{
    time_t ts;
    const char* time_string;

    time(&ts);
    time_string = ctime(&ts);

    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0, 1, 0);
    draw_string(scene, time_string);
    glTranslatef(0, 0, 1);
    glColor3f(1, 1, 1);
    draw_string(scene, "Text Rendering Demo Here!");
    glTranslatef(0, 0, 1);
    glColor3f(1, 0, 0);
    draw_string(scene, time_string);
    glDisable(GL_COLOR_MATERIAL);
}

void draw_string(const Scene* scene, const char* s)
{
    float FONT_WIDTH = 1.0 / 16;
    float FONT_HEIGHT = 1.0 / 8;
    float FONT_X_WIDTH = 0.5;

    glBindTexture(GL_TEXTURE_2D, scene->charmap_id);
    glBegin(GL_QUADS);
    float x = 2.0;
    for (int i = 0; s[i] != 0; ++i) {
        char c = s[i];
        if (c >= 32 && c < 127) {
            int row = c / 16 + 1;
            int column = c % 16;
            float u = FONT_WIDTH * column;
            float v = FONT_HEIGHT * row;

            glNormal3f(0, 1, 0);
            glTexCoord2f(u, v);
            glVertex3f(x, 0, 0);

            glNormal3f(0, 1, 0);
            glTexCoord2f(u + FONT_WIDTH, v);
            glVertex3f(x + FONT_X_WIDTH, 0, 0);

            glNormal3f(0, 1, 0);
            glTexCoord2f(u + FONT_WIDTH, v - FONT_HEIGHT);
            glVertex3f(x + FONT_X_WIDTH, 0, 1);

            glNormal3f(0, 1, 0);
            // 0 0
            glTexCoord2f(u, v - FONT_HEIGHT);
            glVertex3f(x, 0, 1);

            x += FONT_X_WIDTH;
        }
    }
    glEnd();
}
