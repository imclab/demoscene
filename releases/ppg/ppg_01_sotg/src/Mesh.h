#ifndef __MESH__
#define __MESH__

class Mesh {
public:
	// constructor
	Mesh(void);
	// destructor
	~Mesh();
	// dibuja los poligonos (triangulos) en pantalla
	void draw(void);
	// carga una malla
	int loadMesh(char *filename);
	// dibuja las aristas en pantalla
	void drawWire(float linewidth,int numcar);

private:
	int numVertex; // n�mero de v�rtices de la malla
	int numFaces; // n�mero de caras
	float *x,*y,*z; // puntero a vector de v�rtices
	int *faceList[3]; // puntero a vector de caras (o sea una lista de caras)
	float *normalList[3]; // puntero a vector de normales
	//Light *lightList;
	void normalize(void);

};

#endif

