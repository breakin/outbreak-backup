#ifndef KAKTUS_H
#define KAKTUS_H

#include <helper/helper/core/typedefs.h>

/* TODO: Reverse engineer s3d,c3d,o3d,sh3d

  We've got

	extern f3d berg_face;
	#pragma aux berg_face "*"

	extern n3d berg_norm;
	#pragma aux berg_norm "*"

	extern v3d berg_vert;
	#pragma aux berg_vert "*"

	extern v3dr berg_vrot;
	#pragma aux berg_vrot "*"

    o3d berg={TRUE, 1024, &berg_vert, &berg_norm, &berg_vrot, 1922, &berg_face, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1024, 1024, TRUE, TRUE, FALSE, TRUE, FALSE};

  and

    Helper::uint8 * vaddr1   = new Helper::uint8 [s320200*3]+s320200;
	Helper::uint8 * pal5_l   = new Helper::uint8 [256*64];

	Helper::uint8       *textures3d[1]      = { tex0 };
    sh3d       *shadings  [1]      = { &shadings3d[texgrd3d] };
    o3d        *bobjlista[1]       = { &berg };

    s3d bergscen   = { 1, bobjlista, 0, NULL, 1, shadings, &textures3d[0], 0, vaddr1, pal5_l };

*/

struct v3d {
	float x,y,z;
};

struct v3dr
  {
    long        x,      y;
    long        tx,     ty;
	Helper::uint8      c;
  };


struct n3d{
	float x,y,z;
};

struct f3d {
	int C; // texture;
	float nx,ny,nz;
	int v1,v2,v3;
};

struct o3d {
	bool active;
	bool correctsize;	
	float objsizemiddle;
	float objsize;

	float x,y,z,v1,v2,v3;
	
	int vertex;						// vertexCount
	int face;						// faceCount

	v3d *vertexorg;
	v3dr *vertexrot;
	n3d *normalorg;
	f3d *faceorg;
};

struct s3d {
	Helper::uint8 *dest;
	o3d **objectptr;
	int objects; // objectCount

	Helper::uint8 **textureptr;
};


struct c3d{};

struct sh3d{};

extern void rotatesceneobject(s3d *scen, o3d *object, c3d *camera);
extern void putscene(s3d *scen, c3d *camera);

#endif
