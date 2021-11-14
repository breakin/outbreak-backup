#include "kaktus.h"
#include <memory.h>
#include <math.h>

#define rad2deg (3.1415/180.0)

#define MAXLIGHTS       5
#define MAXRADIXFACES   15000
#define MAXPOLYS        15000
#define MAXVERTEX       15000
#define MAXOBJS         10

//****************************************************************************

static Helper::uint16 radixnumbers     [MAXRADIXFACES];
static Helper::uint16 radixsorted      [MAXRADIXFACES];
static Helper::uint32 radix_num        [256];
static Helper::uint32 radix_real       [256];
static Helper::uint32 radix_temp       [MAXRADIXFACES];
static Helper::uint32 radix_pointer    [MAXRADIXFACES];
static Helper::uint8  radix_pointer2   [MAXRADIXFACES];

//****************************************************************************

void radixsort(Helper::uint16 *radix_ptr, Helper::uint16 *sort_pointer, Helper::uint16 max)
  {
    memset(radix_real, 0, 256*4); // Clear radix_real

    for (Helper::uint16 i=0; i<max; i++)
      radix_pointer[i]=(i << 16)+radix_ptr[i];

    for (Helper::uint32 m=0; m<2; m++)
      {
        for (i=0; i<256; i++) radix_num[i]=0;
        for (i=0; i<max; i++)
          {
            Helper::uint8 j=(radix_pointer[i] >> (m*8)) & 255;
            radix_pointer2[i]=j;
            radix_num[j]++;
          }
        radix_real[0]=0;
        for (i=1; i<256; i++) radix_real[i] = radix_num[i-1]+radix_real[i-1];       
        for (i=0; i<max; i++)
          {
            Helper::uint8 j=radix_pointer2[i];
            Helper::uint32 l=radix_real[j];
            radix_temp[l]=radix_pointer[i];
            radix_real[j]++;
          }
        for (i=0; i<max; i++) radix_pointer[i]=radix_temp[i];
      }
    for (i=0; i<max; i++) sort_pointer[i]=radix_pointer[i] >> 16;
  }

//****************************************************************************

void rotatesceneobject(s3d *scen, o3d *object, c3d *camera)
  {
    float pe, x2, y2, z2;
    long  lval;

    // These will be modified by the camera I guess...
    // How should I know... ;)

    float dist;

    if (object->correctsize==true)
      dist=object->objsizemiddle*4096.0f/object->objsize;
    else
      dist = object->objsize;

    float    x = object->x;
    float    y = object->y;
    float    z = object->z;
    float   v1 = object->v1;
    float   v2 = object->v2;
    float   v3 = object->v3;

    float xx =float( cos(rad2deg*v1)*cos(rad2deg*v2));
    float xy =float( sin(rad2deg*v1)*cos(rad2deg*v2));
    float xz =float( sin(rad2deg*v2));
    float yx =float( sin(rad2deg*v1)*cos(rad2deg*v3)+cos(rad2deg*v1)*sin(rad2deg*v2)*sin(rad2deg*v3));
    float yy =float(-cos(rad2deg*v1)*cos(rad2deg*v3)+sin(rad2deg*v1)*sin(rad2deg*v2)*sin(rad2deg*v3));
    float yz =float(-cos(rad2deg*v2)*sin(rad2deg*v3));
    float zx =float( sin(rad2deg*v1)*sin(rad2deg*v3)-cos(rad2deg*v1)*sin(rad2deg*v2)*cos(rad2deg*v3));
    float zy =float(-cos(rad2deg*v1)*sin(rad2deg*v3)-sin(rad2deg*v1)*sin(rad2deg*v2)*cos(rad2deg*v3));
    float zz =float( cos(rad2deg*v2)*cos(rad2deg*v3));
    float xx_xy =float( xx*xy);
    float yx_yy =float( yx*yy);
    float zx_zy =float( zx*zy);
    float x_y;

    float xp, yp, zp, xpn, ypn, zpn;

    for (int C=0; C<object->vertex; C++)
      {
        xp =object->vertexorg[C].x+x;
        yp =object->vertexorg[C].y+y;
        zp =object->vertexorg[C].z+z;

        x_y   = xp*yp;

        z2 = (zx+yp)*(zy+xp)+zp*zz-(zx_zy+x_y);

        pe=256.0f*500.0f/float(1.0f+fabs(dist+z2));
        
        object->vertexrot[C].x = ((xx+yp)*(xy+xp)+zp*xz-(xx_xy+x_y))*pe/256+160;
        object->vertexrot[C].y = ((yx+yp)*(yy+xp)+zp*yz-(yx_yy+x_y))*pe/256+100;
        object->vertexrot[C].z = z2*pe/256+127;

        ypn=object->normalorg[C].y;
        zpn=object->normalorg[C].z;
        xpn=object->normalorg[C].x;

        x_y   = xpn*ypn;

        object->vertexrot[C].nx = (xx+ypn)*(xy+xpn)+zpn*xz-(xx_xy+x_y);
        object->vertexrot[C].ny = (yx+ypn)*(yy+xpn)+zpn*yz-(yx_yy+x_y);
        object->vertexrot[C].nz = (zx+ypn)*(zy+xpn)+zpn*zz-(zx_zy+x_y);
      }
  }

//****************************************************************************

void putscene(s3d *scen, c3d *camera)
  {
     int      radixsortnmr=0;
     Helper::uint32      radixlista[MAXOBJS]={0};

     radixlista[0]=0;

     o3d **obl=scen->objectptr;

     long facecnt;
     Helper::uint8 objcnt;

     // Rotate and put into radixlist
     for (objcnt=0; objcnt<scen->objects; objcnt++)
       {
         if (obl[objcnt]->active==true)
           {
             // Modify angles...
             for (;obl[objcnt]->v1<0;   obl[objcnt]->v1+=360);
             for (;obl[objcnt]->v1>360; obl[objcnt]->v1-=360);
             for (;obl[objcnt]->v2<0;   obl[objcnt]->v2+=360);
             for (;obl[objcnt]->v2>360; obl[objcnt]->v2-=360);
             for (;obl[objcnt]->v3<0;   obl[objcnt]->v3+=360);
             for (;obl[objcnt]->v3>360; obl[objcnt]->v3-=360);

             //Rotate
             rotatesceneobject(scen, obl[objcnt], camera);

			 //TODO: add if (nz>, backfaceculling!)

             for (facecnt=0; facecnt<obl[objcnt]->face; facecnt++)
               radixnumbers[radixsortnmr+facecnt]=65536/2-
                  int((obl[objcnt]->vertexrot[ obl[objcnt]->faceorg[facecnt].v1 ].z+
                       obl[objcnt]->vertexrot[ obl[objcnt]->faceorg[facecnt].v2 ].z+
                       obl[objcnt]->vertexrot[ obl[objcnt]->faceorg[facecnt].v3 ].z));

             radixsortnmr+=obl[objcnt]->face;
           }

         radixlista[objcnt+1]=radixsortnmr;
       }

     // SORT
     if (radixsortnmr<MAXRADIXFACES)
       radixsort(radixnumbers, radixsorted, radixsortnmr);

     // DRAW
     for (facecnt=0; facecnt<radixsortnmr; facecnt++)
       {
         Helper::uint32 poly=radixsorted[facecnt];

         // FIXA TILL DENNA LITE, TACK
         for (Helper::uint8 obj1=0;obj1<scen->objects; obj1++) {
           if ((poly>=radixlista[obj1]) & (poly<radixlista[obj1+1])) poly-=radixlista[obj1]; objcnt=obj1;
		 }

        tmap(scen->dest,
          scen->textureptr[ obl[objcnt]->faceorg[poly].C ],
              &obl[objcnt]->vertexrot[ obl[objcnt]->faceorg[poly].v1 ],
                &obl[objcnt]->vertexrot[ obl[objcnt]->faceorg[poly].v2 ],
                  &obl[objcnt]->vertexrot[ obl[objcnt]->faceorg[poly].v3 ]);
        }
  }
             
//****************************************************************************
