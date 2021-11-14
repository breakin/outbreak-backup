
#include <windows.h>
#include <helper/helper.h>
#include "kaktus.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevinst, LPSTR cmdline, int showstate) {
	
	try {

	// *[PART 1]****************************************************************************

	  {
		Helper::uint32 tunofs;

		bool        SPOTS   =       TRUE;
		Helper::int16        intense =       0;

		frameCount=0;

		do
		  {
			tunofs=abs(sin(frameCount*0.53*rad2deg)*127+128)*256+cos(frameCount*0.65*rad2deg)*127+128;
			Helper::uint32 didi=abs(sin((frameCount*0.5+180*cos(frameCount*0.3*rad2deg))*rad2deg)*99+100)*320;

			disptable(matta+2*didi, texture, vaddr1, tunofs);

			if (SPOTS==TRUE) disptable(matta+2*didi, textures, deep,  tunofs);

			if (Weff & Weffect<128) intense=Weffect;

			if (Weff) switch (Weffect)
			  {
				 case 0xf0 : SPOTS=FALSE; flip(vaddr2, deep);
							 break;
			  }

			Weff=0;

			waitframe();
			fliplight(vaddr1, vga, deep, pal1_l+(intense<<8), 0);
		  }
		while (Weffect!=0xfe);

	//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

		setpalgrad(0,255,63,63,63,63,63,63);
		cls(vga, 0);
		sprite160x100(vga,    logo, 80, 50, 0, 0);

		do
		  {
			waitframe();

			if (Weffect>=0 & Weffect<64)
			  fadetowhite(pal2, Weffect);

			if (Weffect==0x4)
			  {
				flip(back, vga);
				sprite160x100(vga,    logo, 80, 50, 0, 0);
			  }
		  }
		while (Weffect!=0);

	//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

		putpal(pal2);

		char        lights  =       1;
		Helper::int16        lx[11];
		Helper::int16        ly[11];

					frameCount=     0;
					Weff    =       0;
					Weffect =       0;

		cls         (vga, 0);
		cls         (vaddr1, 0);

		bool        logshad=TRUE, mattex2=FALSE;

		Helper::uint32 X, Y;

		frameCount=0;

		Helper::int16 lval;

		float radius=1;
		Helper::uint8 subrad=0;

		do
		  {
			X=sin(frameCount*6.2421*rad2deg)*35;
			Y=sin(frameCount*5.3123*rad2deg)*35;

			Helper::uint8 cl=0;
			if (logshad==TRUE) cl=lights*32/10;
			  else cl=lval*32/10;

			if (subrad>0) cl=radius*63;

			if (subrad==3)
			  if (radius>0.1) radius-=0.005;
				else subrad=4;

			if (subrad==4)
			  if (radius<1) radius+=0.005;
				else subrad=3;

			cls     (deep,  cl);

			flip    (back,  vaddr1);

			for (C=0; C<lights; C++)
			  {
				lx[C] = cos((C*56+180*sin((C*86+0.7*frameCount)*rad2deg)+frameCount)*rad2deg)*60*radius;
				ly[C] = sin((C*78+180*cos((C*96+0.5*frameCount)*rad2deg)+frameCount)*rad2deg)*60*radius;
			  }

			if (logshad==TRUE)
			  {
				sprite160x100(vaddr1, logo, X+80, Y+50, 0, 0);

				for (C=0; C<lights; C++)
				  {
					Helper::int16 mx = 160-80-lx[C]+X;
					Helper::int16 my = 100-50-ly[C]+Y;

					if (1<2)
					  {
						if (my<=-100) break;
						if (my>= 199) break;

						Helper::uint8 yrader  = 100;
						Helper::uint8 yrader2 = 0;

						if (my>100) { yrader-=(my-100); }
						if (my<0)   { yrader+=my; my=0; yrader2+=100-yrader; }

						if (yrader==0) break;

						sprite160x100sub(deep, logo, mx, my, 160, yrader2, yrader);
					  }
				  }

				// Shadows shall not affect the logo
				sprite160x100(deep, logo, X+80, Y+50, 0, 100);
			  }
			else
			  {
				Helper::uint16 X=sin(1.25614*frameCount*rad2deg)*127+128;
				X*=2; if (X>256) X-=256;

				disptable(lintun, lintex, vaddr1,
				  X+abs(cos(1.31467*frameCount*rad2deg)*127+128)*256);
			  }

			if (subrad<3)
			{
			// Put out the flares (type 1)
			for (C=0; C<lights; C++)
			  ch16glenz(0, lx[C]+160-8, ly[C]+100-8, font, deep);

			// Put out the flares (type 2)
			for (C=0; C<lights; C++)
			  {
				Helper::int16 mx=160-80+lx[C];
				Helper::int16 my=100-50+ly[C];

				  {
					if (my<=-100)   break;
					if (my>= 199)   break;

					Helper::uint8 yrader     =       100;
					Helper::uint8 yrader2    =       0;

					if (my>100) { yrader-=(my-100); }
					if (my<0)   { yrader+=my; my=0; yrader2+=100-yrader; }

					if (yrader==0) break;

					sprite160x100add(deep, flare1, mx, my, 0, yrader2, yrader);
				  }
			  }
			}

			if (Weff) switch (Weffect)
			  {
				case    0xf0    : lights++; break;
				case    0xf1    : if (logshad==TRUE) lights--;
									else lval--;
								  break;

				case    0xf2    : lights=10;
								  logshad=FALSE;
								  lval=lights-1;
								  putpal(pal3);
								  break;

				case    0xf3    : lights++;
								  break;

				case    0xf4    : mattex2=TRUE;
								  pcx(&li_texture2, lintex,         pal3, s256256*2);
								  break;

				case    0xf7    : subrad=1;
								  break;

				case    0xf8    : subrad=2;
								  break;

				case    0xf9    : subrad=3;
								  break;
			  }

			if (subrad==1)
			  if (radius>0) radius-=0.01;
				else radius=0;

			if (subrad==2)
			  if (radius<1) radius+=0.02;
				else radius=1;

			Weff=0;

			if (lval<0) lval=0;
			if (lights>10) lights=10;
			if (lights<1) lights=1;

			waitframe();

			if (logshad==TRUE)
			  fliplight(vaddr1, vga, deep, pal2_l, 0);
			else
			  fliplight(vaddr1, vga, deep, pal3_l, 0);
		  }
		while (Weffect!=0xfe);
	  }

	// *[PART 2]****************************************************************************

	
	//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

	  {
		setpalgrad(0,255,63,63,63,63,63,63);
		cls(vga, 0);

		pcx(&be_mountex,  tex0,   pal5, s256256);
		pcx(&be_kostymc,  vaddr2, pal5, s320200);

		frameCount=0;
		Weff=0;
		Weffect=0;

	//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

		Weffect=0;
		pcx(&be_kostymbw, vga, pal4, s320200);
		memset(palnu, 63, 768);

		do
		  {
			waitframe();
			fadanerfel(pal4, palnu);
		  }
		while (Weffect!=0xfe);

	//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

		setpalgrad(0,255,63,63,63,63,63,63);
		memset(palnu, 63, 768);
		Weffect=0;
		pcx(&be_kostymc, vga, pal5, s320200);
		do
		  {
			fadanerfel(pal5, palnu);
			waitframe();
		  }
		while (Weffect!=0xfe);

	//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

		frameCount=0;

		Weffect=0;
		do
		  {
			waitframe();
			if (frameCount*0.5<16)
			  fade(pal5, 64-frameCount*0.5);
		  }
		while (Weffect!=0xfe);

	//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

		Weff=0; Weffect=0; frameCount=0;

		Helper::uint8 nu=0;

		cls(vaddr1,0);

		berg.objsize=3350;
		berg.v3=190;

		Helper::uint32 starter2=frameCount;

		bool PUTTE=TRUE;

		do
		  {
			berg.v2=sin(0.27*frameCount*rad2deg)*179+180;

			berg.x=sin(0.241*frameCount*rad2deg)*300;
			berg.z=cos(0.314*frameCount*rad2deg)*300;

			berg.y=800-(frameCount*4-starter2*4);
			if (berg.y<0) berg.y=0;

			putscene(&bergscen, NULL);

			waitframe();
			if (PUTTE=TRUE) { putpal(pal5); PUTTE=FALSE; }
			flipmix(vga, vaddr2, vaddr1, pal5_m);
		  }
		while (Weffect!=0xfe);

	//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

		Weff=0; Weffect=0;

		Helper::uint32 starter, starterny, starterny2, zoomer, nuhe;

		bool WATTE=FALSE, UFFA=FALSE, UFFA2=FALSE, ZMA=FALSE, MOTION=TRUE, HEJ=FALSE;

		float UF, UF2=1, UF3=1;

		do
		  {
			berg.v3=190*UF3+270*(float)(1.0-UF3);
			berg.v2=sin(0.27*frameCount*rad2deg)*179+180;

			berg.x=sin(0.241*frameCount*rad2deg)*300*UF3;
			berg.z=cos(0.314*frameCount*rad2deg)*300*UF3;

			if (ZMA==TRUE) berg.objsize=3350+(frameCount-zoomer)*64;

			if (UFFA2==TRUE)
			  {
				UF3=1.0-0.0005*(frameCount-starterny2);
				if (UF3<0) UF3=0;
			  }

			if (UF3!=1)
			  {
				// Make tunnel
				berg.vertexorg[32*15+16].y=(1-UF3)*512*4;
				berg.vertexorg[32*15+17].y=(1-UF3)*512*4;
				berg.vertexorg[32*16+16].y=(1-UF3)*512*4;
				berg.vertexorg[32*16+17].y=(1-UF3)*512*4;
				berg.vertexorg[32*15+15].y=(1-UF3)*256*4;
				berg.vertexorg[32*15+18].y=(1-UF3)*256*4;
				berg.vertexorg[32*16+15].y=(1-UF3)*256*4;
				berg.vertexorg[32*16+18].y=(1-UF3)*256*4;
				berg.vertexorg[32*14+16].y=(1-UF3)*256*4;
				berg.vertexorg[32*14+17].y=(1-UF3)*256*4;
				berg.vertexorg[32*17+16].y=(1-UF3)*256*4;
				berg.vertexorg[32*17+17].y=(1-UF3)*256*4;

				// Rise some
				berg.vertexorg[32*13+14].y=-(1-UF3)*256;
				berg.vertexorg[32*13+15].y=-(1-UF3)*256;
				berg.vertexorg[32*13+16].y=-(1-UF3)*256;
				berg.vertexorg[32*13+17].y=-(1-UF3)*256;
				berg.vertexorg[32*13+18].y=-(1-UF3)*256;
				berg.vertexorg[32*13+19].y=-(1-UF3)*256;
				berg.vertexorg[32*18+14].y=-(1-UF3)*256;
				berg.vertexorg[32*18+15].y=-(1-UF3)*256;
				berg.vertexorg[32*18+16].y=-(1-UF3)*256;
				berg.vertexorg[32*18+17].y=-(1-UF3)*256;
				berg.vertexorg[32*18+18].y=-(1-UF3)*256;
				berg.vertexorg[32*18+19].y=-(1-UF3)*256;
			  }

			if (Weff) switch (Weffect)
			  {
				case  0xf0 : starter=frameCount; WATTE=TRUE; break;
				case  0xf1 : UFFA=TRUE; starterny=frameCount; break;
				case  0xf2 : UFFA2=TRUE; starterny2=frameCount; break;
				case  0xf3 : zoomer=frameCount; ZMA=TRUE; break;

				case 0xf5 : pcx(&credits, vaddr2, pal5, s320200); MOTION=TRUE; break;
				case 0xf6 : MOTION=FALSE; nuhe=frameCount; HEJ=TRUE; break;

				case 0xf8 : pcx(&credits, vaddr2, pal5, s320200); break;
			  }

			Weff=0;

			cls(vaddr1,0);
			putscene(&bergscen, NULL);

			waitframe();

			if (MOTION==TRUE)
			  flipmixblur(vga, vaddr2, vaddr1, pal5_m);
			else
			  flipcls(vaddr1, vga);

			// slimy surface
			if (WATTE==TRUE)
			  {
				if (UFFA==FALSE)
				  {
					UF=(frameCount*0.5-starter*0.5)/(float)8;
					if (UF>48) UF=48;
				  }
				else
				  {
					UF2=1.0-0.005*(frameCount-starterny);
					if (UF2<0) UF2=0;
					UF=(frameCount*0.5-starter*0.5)/(float)8*UF2;
					if (UF>48) UF=48;
				  }

				for (Helper::uint16 X=0, O=0; X<32; X++)
				  for (Helper::uint16 Y=0; Y<32; Y++, O++)
					berg.vertexorg[O].y=
					  berg2.vertexorg[O].y*UF2+
					  berg2.vertexorg[O].y*(1-UF3)+
					  UF*sin((cos((frameCount*0.765323+X*21.123)*rad2deg)*180+frameCount*0.522234+Y*21.42112)*rad2deg);
			  }
		  }
		while (Weffect!=0xfe);

	//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

		Weff=0;
		Weffect=0;
		frameCount=0;

		cls(back,0);
		cls(deep,0);

		bool        fadaut=FALSE;

		Helper::uint32 stut;

		do
		  {
			animplaynext(deep, &minanim);

			if (fadaut==TRUE)
			  {
				float Ysd=(frameCount-stut);
				if (Ysd<16)
				  cls(back, Ysd);
				else
				  cls(back, 15);
			  }

			fliptrans(back, deep, 320*200);

			if (Weff) switch (Weffect)
			  {
				case 0xf1 : pcx(&cred1, back, pal6, s320200); break;
				case 0xf2 : pcx(&cred2, back, pal6, s320200); break;
				case 0xf3 : pcx(&cred3, back, pal6, s320200); break;
				case 0xf4 : pcx(&cred4, back, pal6, s320200); break;
				case 0xf5 : stut=frameCount; fadaut=TRUE; break;
			  }

			Weff=0;

			for (Helper::uint8 O=0; O<16; O++)
			  {
				krds[O].tx=sin((abs(100-krds[O].y)*1.123+abs(160-krds[O].x)*3.324+frameCount*1.124+cos(frameCount*1.123123*rad2deg)*45)*rad2deg)*127+128;
				krds[O].ty=cos((abs(160-krds[O].x)*2.123+abs(100-krds[O].y)*1.215+frameCount*1.431+sin(frameCount*1.312312*rad2deg)*45)*rad2deg)*127+128;
			  }

			tmap(vaddr2, tex0, &krds[ 0], &krds[ 1], &krds[ 3]);
			tmap(vaddr2, tex0, &krds[ 0], &krds[ 3], &krds[ 5]);
			tmap(vaddr2, tex0, &krds[ 0], &krds[ 4], &krds[ 5]);
			tmap(vaddr2, tex0, &krds[ 1], &krds[ 2], &krds[ 3]);
			tmap(vaddr2, tex0, &krds[ 2], &krds[ 3], &krds[ 7]);
			tmap(vaddr2, tex0, &krds[ 2], &krds[ 7], &krds[ 8]);
			tmap(vaddr2, tex0, &krds[ 4], &krds[ 5], &krds[10]);
			tmap(vaddr2, tex0, &krds[10], &krds[ 5], &krds[ 9]);
			tmap(vaddr2, tex0, &krds[10], &krds[11], &krds[ 9]);
			tmap(vaddr2, tex0, &krds[ 9], &krds[11], &krds[12]);
			tmap(vaddr2, tex0, &krds[ 7], &krds[ 9], &krds[12]);
			tmap(vaddr2, tex0, &krds[ 7], &krds[ 8], &krds[12]);
 
			waitframe();
			fliplight(vaddr2, vga, deep, pal5_l16, 0);
			darken(back);
		  }
		while (Weffect!=0xfe);
	  }

	// *[THE END]***************************************************************************

	}
	
	// Catch known exceptions.
	catch (const Helper::Exception &e) {
		MessageBox(NULL, e.what(), "Helper::Exception", MB_OK);
	}

	// Catch unknown exceptions.
	catch (...) {
		MessageBox(NULL, "Unknown exception", "(...)::Exception", MB_OK);
	}

	// Return the answer :-D
	return 42;
}