#include <SDL.h>
#include <SDL_opengl.h>
#include <FreeImage.h>
#include <iostream>
#include <GL/glu.h>

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "There was an error\n";
		exit(1);
	}

	SDL_Window* win = SDL_CreateWindow("firstPolygon",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(win);

	glClearColor(0.0, 0.0, 0.0, 1);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, 640 / 480.f, 0.1, 100);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);

	//TEXTURA
	const char* archivo = new char[20];
	archivo = "opengl.png";

	//CARGAR IMAGEN
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
	FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
	bitmap = FreeImage_ConvertTo24Bits(bitmap);
	float w = (float)FreeImage_GetWidth(bitmap);
	float h = (float)FreeImage_GetHeight(bitmap);
	void* datos = FreeImage_GetBits(bitmap);
	//FIN CARGAR IMAGEN

	std::cout << 49 / w << " / " << 51 / h << " / " << (452 + 49) / w << " / " << (h - 212 - 51) / h;


	GLuint textura;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Con MinMap
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_BGR, GL_UNSIGNED_BYTE, datos);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//FIN TEXTURA

	SDL_Event event;

	bool full = false;
	float scale = 1;

	do {
		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0);

		glBegin(GL_TRIANGLES); // ------------------
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1.5, 1, -6);
		glColor3f(0, 1.0, 0.0);
		glVertex3f(-2.5, -1, -6);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-0.5, -1, -6);
		glEnd(); // --------------------------------


		glPushMatrix();

		glTranslatef(1.5, 0, -6);
		glScalef(scale, scale, 1);
		glTranslatef(-1.5, 0, 6);

		

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textura);

		glBegin(GL_QUADS); // ----------------------
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(49/w, (h-49-212)/h );
		glVertex3f(0.5, -1, -6);
		glTexCoord2f((49+452)/w, (h-49-212)/h);
		glVertex3f(2.5, -1, -6);
		glTexCoord2f((49 + 452) / w, (h - 51) / h);
		glVertex3f(2.5, 1, -6);
		glTexCoord2f(49/w, (h-51)/h );
		glVertex3f(0.5, 1, -6);
		glEnd(); // --------------------------------

		glDisable(GL_TEXTURE_2D);
		
		glPopMatrix();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					return 0;
					break;
				case SDLK_F11:
					glMatrixMode(GL_PROJECTION);
					if (full) {
						SDL_SetWindowFullscreen(win, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
						full = false;
					}
					else {
						SDL_SetWindowFullscreen(win, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
						full = true;
						std::cout << "FULL";
					}
					break;
				case SDLK_UP:
					scale += 0.1;
					std::cout << scale;
					break;
				case SDLK_DOWN:
					scale -= 0.1;
					break;
				default:
					break;
				}
			}
		}

		SDL_GL_SwapWindow(win);
	} while (true);

	return 0;

}

