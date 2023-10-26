float skyboxVertices[] = 
{
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
};

unsigned int indicesSky[]{
	0,1,2,2,3,0,
	4,1,0,0,5,4,
	2,6,7,7,3,2,
	4,5,7,7,6,4,
	0,3,7,7,5,0,
	1,4,2,2,4,6
};

//SKY
vector<string> faces
{
	/*
	right
	left
	top
	bottom
	front
	back

	o

	ft
	bk
	ip
	dn
	rt
	lf

	*/
	"Imagenes/right.jpg",
	"Imagenes/left.jpg",
	"Imagenes/top.jpg",
	"Imagenes/bottom.jpg",
	"Imagenes/front.jpg",
	"Imagenes/back.jpg",
};