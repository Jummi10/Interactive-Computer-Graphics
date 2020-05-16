#ifndef VBOCUBE_H
#define VBOCUBE_H

class VBOCube
{
private:
    unsigned int vaoHandle;

public:
    VBOCube();

    void render();
};

#endif // VBOCUBE_H

scene{
	VBOCube *cube;
}

scene sc;
startup()
{
	sc.cube = new VBOCube();

}

render()
{
	 transformation 
		 »ö»ó

	sc.cube->render();
}




