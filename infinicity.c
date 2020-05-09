/** @file Demonstrates 3D shapes of the big city.
 *
 * @author Soheil Sepahyar
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "libkuhl.h"

float aspectRatio;
GLuint texId = 0;

static GLuint program = 0; /**< id value for the GLSL program */
static GLuint program1 = 0; /**< id value for the GLSL program */

static kuhl_geometry triangle;


//static kuhl_geometry quad;
//static kuhl_geometry left_windows;
//static kuhl_geometry right_windows;

float BHeight = 1;
float BWidth = 1;
float BDepth = 1;


//static kuhl_geometry triangle2;

float CBHeight = 1;
float CBWidth = 1;
float CBDepth = 1;

//static kuhl_geometry Cquad;
//static kuhl_geometry left_Cwindows;
//static kuhl_geometry right_Cwindows;

////////////////////////////////////////////////////////////

float x_amount = 0;
float y_amount = 0;
float z_amount = 0;



static kuhl_geometry buildingBottom[10][10];
static kuhl_geometry windowBottomfront[10][10];
static kuhl_geometry windowBottomleft[10][10];
static kuhl_geometry windowBottomright[10][10];

//////////////////////////////////////////////////////////////////////

static kuhl_geometry buildingTop[10][10];
static kuhl_geometry buildingTop[10][10];
static kuhl_geometry windowTopfront[10][10];
static kuhl_geometry windowTopleft[10][10];
static kuhl_geometry windowTopright[10][10];



//////////////////////////////////////////////////////////////////////

int isComplex[10][10];  // tells us if the building is complex or not.

////////////////////////////////////////////////////////////////////// // ALl thing relates to camera settings

int which_one_FBRLUD = 0;

float amount_of_moving_FB = 520;
float amount_of_moving_LR = 60;
float amount_of_up_down = 60;




float varx = 50;
float vary = 50;
float varz = 0;


/* Called by GLFW whenever a key is pressed. */
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/* If the library handles this keypress, return */
	if (kuhl_keyboard_handler(window, key, scancode, action, mods))
		return;

	if(action == GLFW_PRESS)
		return;
	
	switch(key)
	{
#if 0
		
#endif
		case GLFW_KEY_SPACE:
			which_one_FBRLUD = 0;
			z_amount += 2;
			amount_of_moving_FB -= 1;
			varz--;

			break;

		case GLFW_KEY_B:
			which_one_FBRLUD = 1;
			amount_of_moving_FB += 1;
			varz++;

			break;

		case GLFW_KEY_R:
			which_one_FBRLUD = 2;
			amount_of_moving_LR += 1;
			varx++;


			break;

		case GLFW_KEY_L:
			which_one_FBRLUD = 3;
			amount_of_moving_LR -= 1;
			varx--;

			break;


		case GLFW_KEY_U:
			which_one_FBRLUD = 3;
			amount_of_up_down += 1;
			vary++;

			break;	

		case GLFW_KEY_D:
			which_one_FBRLUD = 3;
			amount_of_up_down -= 1;
			vary--;

			break;			
	}
}


/** Draws the 3D scene. */
void display()
{
	/* Render the scene once for each viewport. Frequently one
	 * viewport will fill the entire screen. However, this loop will
	 * run twice for HMDs (once for the left eye and once for the
	 * right). */
	viewmat_begin_frame();
	for(int viewportID=0; viewportID<viewmat_num_viewports(); viewportID++)
	{
		viewmat_begin_eye(viewportID);

		/* Where is the viewport that we are drawing onto and what is its size? */
		int viewport[4]; // x,y of lower left corner, BWidth, BHeight
		viewmat_get_viewport(viewport, viewportID);
		/* Tell OpenGL the area of the window that we will be drawing in. */
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

		/* Clear the current viewport. Without glScissor(), glClear()
		 * clears the entire screen. We could call glClear() before
		 * this viewport loop---but in order for all variations of
		 * this code to work (Oculus support, etc), we can only draw
		 * after viewmat_begin_eye(). */
		glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);
		glEnable(GL_SCISSOR_TEST);
		glClearColor(.1,.1,.1,0); // set clear color to grey
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST); // turn on BDepth testing
		kuhl_errorcheck();

		/* Get the view matrix and the projection matrix */
		float viewMat[16], perspective[16];
		viewmat_get(viewMat, perspective, viewportID);
	

		if (which_one_FBRLUD == 0) 
		{
			mat4f_lookat_new( viewMat, amount_of_moving_LR ,amount_of_up_down ,  amount_of_moving_FB,   varx ,vary,varz,   0,1,0);
		}
		
		if (which_one_FBRLUD == 1) 
		{
			mat4f_lookat_new( viewMat, amount_of_moving_LR ,amount_of_up_down , amount_of_moving_FB  ,   varx ,vary, varz,   0,1,0);
		}		

		if (which_one_FBRLUD == 2) 
		{
			mat4f_lookat_new( viewMat, amount_of_moving_LR ,amount_of_up_down ,amount_of_moving_FB ,   varx ,vary,varz,   0,1,0);
		}

		if (which_one_FBRLUD == 3) 
		{
			mat4f_lookat_new( viewMat, amount_of_moving_LR ,amount_of_up_down ,amount_of_moving_FB ,   varx ,vary,varz,   0,1,0);
		}

		if (which_one_FBRLUD == 4) 
		{
			mat4f_lookat_new( viewMat, amount_of_moving_LR ,amount_of_up_down ,amount_of_moving_FB ,   varx ,vary,varz,   0,1,0);
		}
		
		if (which_one_FBRLUD == 5) 
		{
			mat4f_lookat_new( viewMat, amount_of_moving_LR ,amount_of_up_down ,amount_of_moving_FB ,   varx ,vary,varz,   0,1,0);
		}		

		/* Calculate an angle to rotate the object. glfwGetTime() gets
		 * the time in seconds since GLFW was initialized. Rotates 45 degrees every second. */
		float angle = fmod(glfwGetTime()*45, 360);

		/* Make sure all computers/processes use the same angle */
		dgr_setget("angle", &angle, sizeof(GLfloat));

		/* Create a 4x4 rotation matrix based on the angle we computed. */
		float rotateMat[16];
		mat4f_rotateAxis_new(rotateMat, 0 /* angle */, 0,1,0);

		/* Create a scale matrix. */
		float scaleMat[16];
		mat4f_scale_new(scaleMat, 1, 1, 1);

		float translation[16];
		//mat4f_translate_new(translation, -10, 0, -10);

		/* Combine the scale and rotation matrices into a single model matrix.
		   modelMat = scaleMat * rotateMat
		*/
		float modelMat[16];
		mat4f_mult_mat4f_new(modelMat, scaleMat, rotateMat);



		float modelview[16];


		/* Draw the geometry using the matrices that we sent to the
		 * vertex programs immediately above */
		//kuhl_geometry_draw(&triangle);
		//kuhl_geometry_draw(&quad);
		//kuhl_geometry_draw(&left_windows);
		//kuhl_geometry_draw(&right_windows();


		float translation2[16];



/////////////////////////////////////////////////////////////////////////////////////

		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < 10; j++)
			{

				
				
				mat4f_translate_new(translation, 0  , 0, (20 + (j*5)));
			
				mat4f_mult_mat4f_new(modelMat, modelMat, translation);
				/* Construct a modelview matrix: modelview = viewMat * modelMat */
				mat4f_mult_mat4f_new(modelview, viewMat, modelMat);
		
				/* Tell OpenGL which GLSL program the subsequent
				 * glUniformMatrix4fv() calls are for. */
				kuhl_errorcheck();
				glUseProgram(program);
				kuhl_errorcheck();

								/* Send the perspective projection matrix to the vertex program. */
				glUniformMatrix4fv(kuhl_get_uniform("Projection"),
				                   1, // number of 4x4 float matrices
				                   0, // transpose
				                   perspective); // value
				/* Send the modelview matrix to the vertex program. */
				glUniformMatrix4fv(kuhl_get_uniform("ModelView"),
				                   1, // number of 4x4 float matrices
				                   0, // transpose
				                   modelview); // value
				kuhl_errorcheck();


				kuhl_geometry_draw(&buildingBottom[i][j]);
				kuhl_geometry_draw(&windowBottomfront[i][j]);
				kuhl_geometry_draw(&windowBottomleft[i][j]);
				kuhl_geometry_draw(&windowBottomright[i][j]);

				if (isComplex[i][j] == true) 
				{
					kuhl_geometry_draw(&buildingTop[i][j]);
					kuhl_geometry_draw(&windowTopfront[i][j]);
					kuhl_geometry_draw(&windowTopleft[i][j]);
					kuhl_geometry_draw(&windowTopright[i][j]);
				}

				
				


				
			}

							 mat4f_translate_new(translation2,(20 + (i * 20)), 0, 0);
		
							mat4f_mult_mat4f_new(modelMat, scaleMat, rotateMat);

							mat4f_mult_mat4f_new(modelMat, modelMat, translation2);

							mat4f_mult_mat4f_new(modelview, viewMat, modelMat);
	//										/* Send the perspective projection matrix to the vertex program. */
	//						glUniformMatrix4fv(kuhl_get_uniform("Projection"),
	//						                   1, // number of 4x4 float matrices
	//						                   0, // transpose
	//						                   perspective); // value
	//						/* Send the modelview matrix to the vertex program. */
	//						glUniformMatrix4fv(kuhl_get_uniform("ModelView"),
	//						                   1, // number of 4x4 float matrices
	//						                   0, // transpose
	//						                   modelview); // value
	//						kuhl_errorcheck();
	//													/* Send the perspective projection matrix to the vertex program. */
							kuhl_errorcheck();
		}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/* Get the view matrix and the projection matrix */
		float viewMatC[16], perspectiveC[16];
		viewmat_get(viewMatC, perspectiveC, viewportID);

		/* Calculate an angle to rotate the object. glfwGetTime() gets
		 * the time in seconds since GLFW was initialized. Rotates 45 degrees every second. */
		float angleC = fmod(glfwGetTime()*45, 360);

		/* Make sure all computers/processes use the same angle */
		dgr_setget("angle", &angleC, sizeof(GLfloat));

		/* Create a 4x4 rotation matrix based on the angle we computed. */
		float rotateMatC[16];
		mat4f_rotateAxis_new(rotateMatC, 0 /* angle */, 0,1,0);

		/* Create a scale matrix. */
		float scaleMatC[16];
		mat4f_scale_new(scaleMatC, 0.5, 1, 1	);

		float scaleMatC2[16];
		mat4f_scale_new(scaleMatC2, 1, 1, 1	);

		float translationC[16];
		mat4f_translate_new(translationC, 200, 0, 250 + (amount_of_moving_FB - 519));
		//mat4f_translate_new(translationC, 0  , 0, (20 + (j*5)));
			
				//mat4f_mult_mat4f_new(modelMat, modelMat, translation);
				/* Construct a modelview matrix: modelview = viewMat * modelMat */
		/* Combine the scale and rotation matrices into a single model matrix.
		   modelMat = scaleMat * rotateMat
		*/
		float modelMatC[16];
		mat4f_mult_mat4f_new(modelMatC, scaleMatC, rotateMatC);

		mat4f_mult_mat4f_new(modelMatC, modelMatC, translationC);

		//float modelviewC[16];

		//mat4f_mult_mat4f_new(modelMatC, scaleMatC2, rotateMatC);

		mat4f_mult_mat4f_new(modelview, viewMat, modelMatC);

		kuhl_errorcheck();
		glUseProgram(program1);
		kuhl_errorcheck();

						/* Send the perspective projection matrix to the vertex program. */
		glUniformMatrix4fv(kuhl_get_uniform("Projection"),
		                   1, // number of 4x4 float matrices
		                   0, // transpose
		                   perspectiveC); // value
		/* Send the modelview matrix to the vertex program. */
		glUniformMatrix4fv(kuhl_get_uniform("ModelView"),
		                   1, // number of 4x4 float matrices
		                   0, // transpose
		                   modelview); // value
		kuhl_errorcheck();



		kuhl_geometry_draw(&triangle);

		glUseProgram(0); // stop using a GLSL program.
		viewmat_end_eye(viewportID);
	} // finish viewport loop
	viewmat_end_frame();

	/* Check for errors. If there are errors, consider adding more
	 * calls to kuhl_errorcheck() in your code. */
	kuhl_errorcheck();

}







/* This illustrates how to draw a quad by drawing two triangles and reusing vertices. */   // For front window
void init_geometrySimpleWindows(kuhl_geometry *geom, GLuint prog)
{
	GLfloat * vertices = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter = 0;

	for(float i = 0; i < (BHeight) - 0.5 ; i += 0.5)
	{
		for(float j = 0; j < (BWidth) - 0.5  ; j+= 0.5)
		{
			kuhl_geometry_new(geom, prog,  6 * (BHeight * BWidth * 4), // num vertices                   //******** here
	              GL_TRIANGLES); // primitive type

			vertices[my_counter++] = j;
			vertices[my_counter++] = i;
			vertices[my_counter++] = (BDepth) + 0.15;

			vertices[my_counter++] = j + 0.5 ;
			vertices[my_counter++] = i;
			vertices[my_counter++] = (BDepth) + 0.15;

			vertices[my_counter++] = j + 0.5;
			vertices[my_counter++] = i + 0.5;
			vertices[my_counter++] = (BDepth) + 0.15;


			vertices[my_counter++] = j;
			vertices[my_counter++] = i;
			vertices[my_counter++] = (BDepth) + 0.15;

			vertices[my_counter++] = j + 0.5;
			vertices[my_counter++] = i + 0.5;
			vertices[my_counter++] = (BDepth) + 0.15;

			vertices[my_counter++] = j;
			vertices[my_counter++] = i + 0.5;
			vertices[my_counter++] = (BDepth) + 0.15;

			j+= 0.1;


		}

		i += 0.1;
	}

		   kuhl_geometry_attrib(geom, vertices, // data   // Drawing front Windows
		   3, // number of components (x,y,z)
		   "in_Position", // GLSL variable
		   	KG_WARN); // warn if attribute is missing in GLSL program?




	GLfloat * colors = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_colors = 0;

	for(float i = 0; i < (BHeight) - 0.5 ; i += 0.5)
	{
		for(float j = 0; j < (BWidth) - 0.5  ; j+= 0.5)
		{
			float my_num = drand48();

			if(my_num <= 0.1)
			{
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;
			}

			else
			{
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;
			}




		}
	}

	kuhl_geometry_attrib(geom, colors, 3, "in_Color", KG_WARN);

}




///////////////////////////////////////////////




/* This illustrates how to draw a quad by drawing two triangles and reusing vertices. */
void init_geometrySimpleWindows3(kuhl_geometry *geom, GLuint prog)
{

	GLfloat * vertices_side = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_side = 0;

	for(float i = 0; i < (BHeight) - 0.5 ; i += 0.5)
	{
		for(float j = 0; j < (BDepth) - 0.5  ; j+= 0.5)
		{
			kuhl_geometry_new(geom, prog,  6 * (BHeight * BDepth * 4), // num vertices                   //******** here
	              GL_TRIANGLES); // primitive type

			vertices_side[my_counter_side++] = ((BWidth) + 0.15);
			vertices_side[my_counter_side++] = i;
			vertices_side[my_counter_side++] = j;

			vertices_side[my_counter_side++] = ((BWidth) + 0.15);
			vertices_side[my_counter_side++] = i;
			vertices_side[my_counter_side++] = j + 0.5;

			vertices_side[my_counter_side++] = ((BWidth) + 0.15);
			vertices_side[my_counter_side++] = i + 0.5;
			vertices_side[my_counter_side++] = j + 0.5;


			vertices_side[my_counter_side++] = ((BWidth) + 0.15);
			vertices_side[my_counter_side++] = i;
			vertices_side[my_counter_side++] = j;

			vertices_side[my_counter_side++] = ((BWidth) + 0.15);
			vertices_side[my_counter_side++] = i + 0.5;
			vertices_side[my_counter_side++] = j + 0.5;

			vertices_side[my_counter_side++] = ((BWidth) + 0.15);
			vertices_side[my_counter_side++] = i + 0.5;
			vertices_side[my_counter_side++] = j;

			j+= 0.1;


		}

		i += 0.1;
	}
			   kuhl_geometry_attrib(geom, vertices_side, // data   // Side windows
		   3, // number of components (x,y,z)
		   "in_Position", // GLSL variable
		   	1); // warn if attribute is missing in GLSL program?

				 //  kuhl_geometry_attrib(cube, vertexData, 3, "in_Position", 1);



	GLfloat * colors = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_colors = 0;

	for(float i = -(BHeight/2.0); i < (BHeight/2) - 0.3 ; i += 0.5)
	{
		for(float j = -(BDepth/2.0); j < (BDepth/2) - 0.5  ; j+= 0.5)
		{
			float my_num = drand48();

			if(my_num <= 0.1)
			{
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;
			}

			else
			{
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;
			}




		}
	}

	kuhl_geometry_attrib(geom, colors, 3, "in_Color", KG_WARN);

}


///////////////////////////////////////////////










/* This illustrates how to draw a quad by drawing two triangles and reusing vertices. */
void init_geometrySimpleWindows2(kuhl_geometry *geom, GLuint prog)
{

	GLfloat * vertices_side = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_side = 0;

	for(float i = 0; i < (BHeight) - 0.5 ; i += 0.5)
	{
		for(float j = 0; j < (BDepth) - 0.5  ; j+= 0.5)
		{
			kuhl_geometry_new(geom, prog,  6 * (BHeight * BDepth * 4), // num vertices                   //******** here
	              GL_TRIANGLES); // primitive type

			vertices_side[my_counter_side++] = -0.15;
			vertices_side[my_counter_side++] = i;
			vertices_side[my_counter_side++] = j;

			vertices_side[my_counter_side++] = -0.15;
			vertices_side[my_counter_side++] = i;
			vertices_side[my_counter_side++] = j + 0.5;

			vertices_side[my_counter_side++] = -0.15;
			vertices_side[my_counter_side++] = i + 0.5;
			vertices_side[my_counter_side++] = j + 0.5;


			vertices_side[my_counter_side++] = -0.15;
			vertices_side[my_counter_side++] = i;
			vertices_side[my_counter_side++] = j;

			vertices_side[my_counter_side++] = -0.15;
			vertices_side[my_counter_side++] = i + 0.5;
			vertices_side[my_counter_side++] = j + 0.5;

			vertices_side[my_counter_side++] = -0.15;
			vertices_side[my_counter_side++] = i + 0.5;
			vertices_side[my_counter_side++] = j;

			j+= 0.1;


		}

		i += 0.1;
	}
			   kuhl_geometry_attrib(geom, vertices_side, // data   // Side windows
		   3, // number of components (x,y,z)
		   "in_Position", // GLSL variable
		   	1); // warn if attribute is missing in GLSL program?

				 //  kuhl_geometry_attrib(cube, vertexData, 3, "in_Position", 1);



	GLfloat * colors = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_colors = 0;

	for(float i = -(BHeight/2.0); i < (BHeight/2) - 0.3 ; i += 0.5)
	{
		for(float j = -(BDepth/2.0); j < (BDepth/2) - 0.5  ; j+= 0.5)
		{
			float my_num = drand48();

			if(my_num <= 0.1)
			{
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;
			}

			else
			{
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;
			}




		}
	}

	kuhl_geometry_attrib(geom, colors, 3, "in_Color", KG_WARN);

}



/* This illustrates how to draw 3 sides of a cube. */
void init_geometrySimpleBuilding(kuhl_geometry *cube, GLuint program)
{
	// 4*3 is the number of vertices. If you add more, you should increase this number!
	kuhl_geometry_new(cube, program, 12*3, GL_TRIANGLES);

	/* The data that we want to draw. Each set of three numbers is a
	 * position. */
	GLfloat vertexData[] = {(0 * BWidth), (0 * BHeight), (0 * BDepth), // -Z face  0
	                        ( 1* BWidth), (0 * BHeight), (0 * BDepth), // 1
	                        ( 1* BWidth), ( 1* BHeight), (0 * BDepth), // 2
	                        (0 * BWidth), ( 1* BHeight), (0 * BDepth), // 3

	                        (0 * BWidth), (0 * BHeight), ( 1* BDepth), // +Z face 4
	                        ( 1* BWidth), (0 * BHeight), ( 1* BDepth), // 5
	                        ( 1* BWidth), ( 1* BHeight), ( 1* BDepth), // 6
	                        (0 * BWidth), ( 1* BHeight), ( 1* BDepth), // 7

	                        (0 * BWidth), (0 * BHeight), (0 * BDepth),  // -X face  //0
	                        (0 * BWidth), ( 1* BHeight), (0 * BDepth),  // 3
	                        (0 * BWidth), ( 1* BHeight), ( 1* BDepth),  // 7
	                        (0 * BWidth), (0 * BHeight), ( 1* BDepth),  // 4

						 	 (1* BWidth), (0 * BHeight), (0 * BDepth), // 5      // x face
							 (1* BWidth), ( 1* BHeight), (0 * BDepth), // 1
							 (1* BWidth), ( 1* BHeight), ( 1* BDepth), // 6
							 (1* BWidth), (0 * BHeight), ( 1* BDepth), // 5

						    (0 * BWidth), ( 1* BHeight ),( 1* BDepth), // 7    // y
							( 1* BWidth), ( 1* BHeight ),( 1* BDepth), // 6
							( 1* BWidth), ( 1* BHeight ),(0 * BDepth), // 2
						    (0 * BWidth), ( 1* BHeight ),(0 * BDepth), // 3

						    (0 * BWidth), ( 0 * BHeight),( 1* BDepth), // 0   // -y
							( 1* BWidth), ( 0 * BHeight),( 1* BDepth), // 1
							( 1* BWidth), ( 0 * BHeight),(0 * BDepth), //5
						    (0 * BWidth), ( 0 * BHeight),(0 * BDepth) // 4




	};

	kuhl_geometry_attrib(cube, vertexData, 3, "in_Position", 1);



	/* The colors of each of the vertices */
	GLfloat colorData[] = {0.2,0.2,0.2,
	                       0.2,0.2,0.2,
	                       0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2};

	kuhl_geometry_attrib(cube, colorData, 3, "in_Color", KG_WARN);

	// Since we can't draw quads in OpenGL 3.0+, we'll make quads out of our vertices: Two triangles form a quad.
	GLuint indexData[] = { 0, 1, 2,  // first triangle is index 0, 1, and 2 in the list of vertices
	                       0, 2, 3,
	                       4, 5, 6,  // second quad
	                       4, 6, 7,
	                       8, 9, 10, // third quad
	                       8, 10, 11,
						   12,13,14,
						   12,14,15,
						   16,17,18,
						   16,18,19,
						   20,21,22,
						   20,22,23

							};



	//3*6 is the length of the indices array. Since we are drawing
	//triangles, it should be a multiple of 3.
	kuhl_geometry_indices(cube, indexData, 12 * 3);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// // All the complex building

/* This illustrates how to draw a quad by drawing two triangles and reusing vertices. */
void init_geometryComplexWindows(kuhl_geometry *geom, GLuint prog)
{
	GLfloat * vertices = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter = 0;

	for(float i = 0; i < (CBHeight) - 0.5 ; i += 0.5)
	{
		for(float j = 0; j < (CBWidth) - 0.5  ; j+= 0.5)
		{
			kuhl_geometry_new(geom, prog,  6 * (CBHeight * CBWidth * 4), // num vertices                   //******** here
	              GL_TRIANGLES); // primitive type

			vertices[my_counter++] = j + (BWidth/2) - (CBWidth/2);
			vertices[my_counter++] = i + (BHeight);
			vertices[my_counter++] = (CBDepth) + 0.15 + (BDepth/2) - (CBDepth/2);

			vertices[my_counter++] = j + 0.5 + (BWidth/2) - (CBWidth/2) ;
			vertices[my_counter++] = i + (BHeight);
			vertices[my_counter++] = (CBDepth) + 0.15 + (BDepth/2) - (CBDepth/2);

			vertices[my_counter++] = j + 0.5 + (BWidth/2) - (CBWidth/2);
			vertices[my_counter++] = i + 0.5 + (BHeight);
			vertices[my_counter++] = (CBDepth) + 0.15 + (BDepth/2) - (CBDepth/2); 


			vertices[my_counter++] = j + (BWidth/2) - (CBWidth/2);
			vertices[my_counter++] = i + (BHeight);
			vertices[my_counter++] = (CBDepth) + 0.15 + (BDepth/2) - (CBDepth/2);

			vertices[my_counter++] = j + 0.5 + (BWidth/2) - (CBWidth/2);
			vertices[my_counter++] = i + 0.5 + (BHeight);
			vertices[my_counter++] = (CBDepth) + 0.15 + (BDepth/2) - (CBDepth/2);

			vertices[my_counter++] = j + (BWidth/2) - (CBWidth/2);
			vertices[my_counter++] = i + 0.5 + (BHeight);
			vertices[my_counter++] = (CBDepth) + 0.15 + (BDepth/2) - (CBDepth/2);  

			j+= 0.1;


		}

		i += 0.1;
	}

		   kuhl_geometry_attrib(geom, vertices, // data   // Drawing front Windows
		   3, // number of components (x,y,z)
		   "in_Position", // GLSL variable
		   	KG_WARN); // warn if attribute is missing in GLSL program?



	GLfloat * colors = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_colors = 0;

	for(float i = -(CBHeight/2.0); i < (CBHeight/2) - 0.3 ; i += 0.5)
	{
		for(float j = -(CBWidth/2.0); j < (CBWidth/2) - 0.5  ; j+= 0.5)
		{
			float my_num = drand48();

			if(my_num <= 0.1)
			{
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;
			}

			else
			{
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;
			}




		}
	}

	kuhl_geometry_attrib(geom, colors, 3, "in_Color", KG_WARN);

}




///////////////////////////////////////////////




/* This illustrates how to draw a quad by drawing two triangles and reusing vertices. */ //Right Side
void init_geometryComplexWindows3(kuhl_geometry *geom, GLuint prog)
{

	GLfloat * vertices_side = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_side = 0;

	for(float i = 0; i < (CBHeight) - 0.5 ; i += 0.5)
	{
		for(float j = 0; j < (CBDepth) - 0.5  ; j+= 0.5)
		{
			kuhl_geometry_new(geom, prog,  6 * (CBHeight * CBDepth * 4), // num vertices                   //******** here
	              GL_TRIANGLES); // primitive type

			vertices_side[my_counter_side++] = (CBWidth +  (BWidth/2) - (CBWidth/2) + 0.15);
			vertices_side[my_counter_side++] = i + BHeight;
			vertices_side[my_counter_side++] = j + (BDepth/2) - (CBDepth/2);

			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) + 0.15);
			vertices_side[my_counter_side++] = i + BHeight;
			vertices_side[my_counter_side++] = j + 0.5 + (BDepth/2) - (CBDepth/2);

			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) + 0.15);
			vertices_side[my_counter_side++] = i + 0.5 + BHeight;
			vertices_side[my_counter_side++] = j + 0.5 + (BDepth/2) - (CBDepth/2);


			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) + 0.15);
			vertices_side[my_counter_side++] = i + BHeight;
			vertices_side[my_counter_side++] = j + (BDepth/2) - (CBDepth/2);

			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) + 0.15);
			vertices_side[my_counter_side++] = i + 0.5 + BHeight;
			vertices_side[my_counter_side++] = j + 0.5 + (BDepth/2) - (CBDepth/2);

			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) + 0.15);
			vertices_side[my_counter_side++] = i + 0.5 + BHeight;
			vertices_side[my_counter_side++] = j + (BDepth/2) - (CBDepth/2);

			j+= 0.1;


		}

		i += 0.1;
	}
			   kuhl_geometry_attrib(geom, vertices_side, // data   // Side windows
		   3, // number of components (x,y,z)
		   "in_Position", // GLSL variable
		   	1); // warn if attribute is missing in GLSL program?

				 //  kuhl_geometry_attrib(cube, vertexData, 3, "in_Position", 1);



	GLfloat * colors = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_colors = 0;

	for(float i = -(CBHeight/2.0); i < (CBHeight/2) - 0.3 ; i += 0.5)
	{
		for(float j = -(CBDepth/2.0); j < (CBDepth/2) - 0.5  ; j+= 0.5)
		{
			float my_num = drand48();

			if(my_num <= 0.1)
			{
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;
			}

			else
			{
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;
			}




		}
	}

	kuhl_geometry_attrib(geom, colors, 3, "in_Color", KG_WARN);

}


///////////////////////////////////////////////










/* This illustrates how to draw a quad by drawing two triangles and reusing vertices. */
void init_geometryComplexWindows2(kuhl_geometry *geom, GLuint prog)  // LEFT SIDE
{

	GLfloat * vertices_side = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_side = 0;

	for(float i = 0; i < (CBHeight) - 0.5 ; i += 0.5)
	{
		for(float j = 0; j < (CBDepth) - 0.5  ; j+= 0.5)
		{
			kuhl_geometry_new(geom, prog,  6 * (CBHeight * CBDepth * 4), // num vertices                   //******** here
	              GL_TRIANGLES); // primitive type

			vertices_side[my_counter_side++] = (CBWidth +  (BWidth/2) - (CBWidth) - (CBWidth/2) - 0.15);
			vertices_side[my_counter_side++] = i + BHeight;
			vertices_side[my_counter_side++] = j + (BDepth/2) - (CBDepth/2);

			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) - (CBWidth) - 0.15);
			vertices_side[my_counter_side++] = i + BHeight;
			vertices_side[my_counter_side++] = j + 0.5 + (BDepth/2) - (CBDepth/2);

			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) - (CBWidth) - 0.15);
			vertices_side[my_counter_side++] = i + 0.5 + BHeight;
			vertices_side[my_counter_side++] = j + 0.5 + (BDepth/2) - (CBDepth/2);


			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) - (CBWidth) - 0.15);
			vertices_side[my_counter_side++] = i + BHeight;
			vertices_side[my_counter_side++] = j + (BDepth/2) - (CBDepth/2);

			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) - (CBWidth) - 0.15);
			vertices_side[my_counter_side++] = i + 0.5 + BHeight;
			vertices_side[my_counter_side++] = j + 0.5 + (BDepth/2) - (CBDepth/2);

			vertices_side[my_counter_side++] = (CBWidth + (BWidth/2) - (CBWidth/2) - (CBWidth) - 0.15);
			vertices_side[my_counter_side++] = i + 0.5 + BHeight;
			vertices_side[my_counter_side++] = j + (BDepth/2) - (CBDepth/2);

	

			j+= 0.1;

		}

		i += 0.1;
	}
			   kuhl_geometry_attrib(geom, vertices_side, // data   // Side windows
		   3, // number of components (x,y,z)
		   "in_Position", // GLSL variable
		   	1); // warn if attribute is missing in GLSL program?

				 //  kuhl_geometry_attrib(cube, vertexData, 3, "in_Position", 1);



	GLfloat * colors = (GLfloat*) malloc((3 * 6 * 6000) * sizeof(GLfloat));   // We should use dynamic array in order to use for nested loops for our purpose
	int my_counter_colors = 0;

	for(float i = -(CBHeight/2.0); i < (CBHeight/2) - 0.3 ; i += 0.5)
	{
		for(float j = -(CBDepth/2.0); j < (CBDepth/2) - 0.5  ; j+= 0.5)
		{
			float my_num = drand48();

			if(my_num <= 0.1)
			{
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0.6;
				colors[my_counter_colors++] = 0;
			}

			else
			{
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;


				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;

				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0.01;
				colors[my_counter_colors++] = 0;
			}




		}
	}

	kuhl_geometry_attrib(geom, colors, 3, "in_Color", KG_WARN);

}

///////////////////////////////////////////////////  //windows for complex building is end.

/* This illustrates how to draw 3 sides of a cube. */
void init_geometryComplexBuilding(kuhl_geometry *cube, GLuint program)
{
	// 4*3 is the number of vertices. If you add more, you should increase this number!
	kuhl_geometry_new(cube, program, 12*3, GL_TRIANGLES);

	/* The data that we want to draw. Each set of three numbers is a
	 * position. */
	GLfloat vertexData[] = {(0 * CBWidth + (BWidth/2) - (CBWidth/2)), (0 * CBHeight  + BHeight ) , (0 * CBDepth + (BDepth/2) - (CBDepth/2)), // -Z face  0
	                        ( 1 * CBWidth + (BWidth/2) - (CBWidth/2)), (0 * CBHeight  + BHeight), (0 * CBDepth + (BDepth/2) - (CBDepth/2)), // 1
	                        ( 1 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight + BHeight), (0 * CBDepth + (BDepth/2) - (CBDepth/2)), // 2
	                        (0 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight  + BHeight), (0 * CBDepth + (BDepth/2) - (CBDepth/2)), // 3

	                        (0 * CBWidth + (BWidth/2) - (CBWidth/2)), (0 * CBHeight   + BHeight ), ( 1 * CBDepth + (BDepth/2) - (CBDepth/2)), // +Z face 4
	                        ( 1 * CBWidth + (BWidth/2) - (CBWidth/2)), (0 * CBHeight   + BHeight), ( 1 * CBDepth + (BDepth/2) - (CBDepth/2)), // 5
	                        ( 1 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight  + BHeight), ( 1 * CBDepth + (BDepth/2) - (CBDepth/2)), // 6
	                        (0 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight  + BHeight ), ( 1 * CBDepth + (BDepth/2) - (CBDepth/2)), // 7

	                        (0 * CBWidth + (BWidth/2) - (CBWidth/2)), (0 * CBHeight   + BHeight ), (0 * CBDepth + (BDepth/2) - (CBDepth/2)),  // -X face  //0
	                        (0 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight  + BHeight ), (0 * CBDepth + (BDepth/2) - (CBDepth/2)),  // 3
	                        (0 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight  + BHeight ), ( 1 * CBDepth +(BDepth/2) - (CBDepth/2)),  // 7
	                        (0 * CBWidth + (BWidth/2) - (CBWidth/2)), (0 * CBHeight   + BHeight ), ( 1 * CBDepth +(BDepth/2) - (CBDepth/2)),  // 4

						 	 (1 * CBWidth + (BWidth/2) - (CBWidth/2)), (0 * CBHeight   + BHeight), (0 * CBDepth +  (BDepth/2) - (CBDepth/2)), // 5      // x face
							 (1 * CBWidth + (BWidth/2)- (CBWidth/2)), ( 1 * CBHeight  + BHeight), (0 * CBDepth +  (BDepth/2) - (CBDepth/2)), // 1
							 (1 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight  + BHeight), ( 1 * CBDepth + (BDepth/2) - (CBDepth/2)), // 6
							 (1 * CBWidth + (BWidth/2) - (CBWidth/2)), (0 * CBHeight   + BHeight), ( 1 * CBDepth + (BDepth/2) - (CBDepth/2)) , // 5

						    (0 * CBWidth +(BWidth/2) - (CBWidth/2)), ( 1 * CBHeight   + BHeight), ( 1 * CBDepth +   (BDepth/2) - (CBDepth/2)), // 7    // y
							( 1 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight  + BHeight), ( 1 * CBDepth +  (BDepth/2) - (CBDepth/2)), // SSS6
							( 1 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight  + BHeight), (0 * CBDepth +   (BDepth/2) - (CBDepth/2)), // 2
						    (0 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 1 * CBHeight   + BHeight), (0 * CBDepth +   (BDepth/2) - (CBDepth/2)), // 3

						    (0 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 0 * CBHeight + BHeight), ( 1 * CBDepth +  (BDepth/2) - (CBDepth/2)), // 0   // -y
							( 1 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 0 * CBHeight + BHeight), ( 1 * CBDepth + (BDepth/2) - (CBDepth/2)), // 1
							( 1 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 0 * CBHeight + BHeight), (0 * CBDepth +  (BDepth/2) - (CBDepth/2)), //5
						    (0 * CBWidth + (BWidth/2) - (CBWidth/2)), ( 0 * CBHeight + BHeight), (0 * CBDepth +   (BDepth/2) - (CBDepth/2) ) // 4




	};





	kuhl_geometry_attrib(cube, vertexData, 3, "in_Position", 1);



	/* The colors of each of the vertices */
	GLfloat colorData[] = {0.2,0.2,0.2,
	                       0.2,0.2,0.2,
	                       0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2,
						   0.2,0.2,0.2};

	kuhl_geometry_attrib(cube, colorData, 3, "in_Color", KG_WARN);

	// Since we can't draw quads in OpenGL 3.0+, we'll make quads out of our vertices: Two triangles form a quad.
	GLuint indexData[] = { 0, 1, 2,  // first triangle is index 0, 1, and 2 in the list of vertices
	                       0, 2, 3,
	                       4, 5, 6,  // second quad
	                       4, 6, 7,
	                       8, 9, 10, // third quad
	                       8, 10, 11,
						   12,13,14,
						   12,14,15,
						   16,17,18,
						   16,18,19,
						   20,21,22,
						   20,22,23

							};



	//3*6 is the length of the indices array. Since we are drawing
	//triangles, it should be a multiple of 3.
	kuhl_geometry_indices(cube, indexData, 12 * 3);

}
///////////////////////


void init_geometryQuad(kuhl_geometry *geom, GLuint prog)
{
	kuhl_geometry_new(geom, prog,
	                  4, // number of vertices
	                  GL_TRIANGLES); // type of thing to draw

	GLfloat texcoordData[] = {0, 0,
							  10, 0,
						      10, 10,
	                          0, 10};

	kuhl_geometry_attrib(geom, texcoordData, 2, "in_TexCoord", KG_WARN);
	// The 2 parameter above means each texture coordinate is a 2D coordinate.				  

	GLfloat vertexPositions[] = {-250, -1, -250,
	                             250, -1, -250,
	                             250, -1, 250,
	                             -250, -1, 250 };						 
	kuhl_geometry_attrib(geom, vertexPositions,
	                     3, // number of components x,y,z
	                     "in_Position", // GLSL variable
	                     KG_WARN); // warn if attribute is missing in GLSL program?

	/* Load the texture. It will be bound to texId */	
	GLuint texId = 0;
	//float aspectRatio = kuhl_read_texture_file("../road_texture/road.png", &texId);
	float aspectRatio = kuhl_read_texture_file_wrap("../road_texture/road.png", &texId, GL_REPEAT, GL_REPEAT);
	msg(MSG_DEBUG, "Aspect ratio of image is %f\n", aspectRatio); // write message to log.txt

	/* Tell this piece of geometry to use the texture we just loaded. */
	kuhl_geometry_texture(geom, texId, "tex", KG_WARN);

	
	/* A list of triangles that we want to draw. "0" refers to the
	 * first vertex in our list of vertices. Every three numbers forms
	 * a single triangle. */
	GLuint indexData[] = { 0, 1, 2,  
	                       0, 2, 3 };
	kuhl_geometry_indices(geom, indexData, 6);


	kuhl_errorcheck();
}


///////////////////////

void draw_building()
{
	BHeight = 50;
	BWidth = 10;
	BDepth = 10;

	//init_geometrySimpleBuilding(&triangle,program);
	//init_geometrySimpleWindows(&quad, program);
	//init_geometrySimpleWindows2(&left_windows, program);
	//init_geometrySimpleWindows3(&right_windows, program);
}

void draw_complex_building()
{
	CBHeight = 50;
	CBWidth = 8;
	CBDepth = 8;

	//init_geometryComplexBuilding(&triangle2,program);
	//init_geometryComplexWindows(&Cquad, program);
	//init_geometryComplexWindows2(&left_Cwindows, program);
	//init_geometryComplexWindows3(&right_Cwindows, program);
}

void draw_them_all()
{
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			float my_num = drand48();

			if ( my_num <= 0.5)
			{
				isComplex[j][i] = 0;
			}

			else
			{
				isComplex[j][i] = 1;
			}	
		}
	}

	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			//buildingBottom[j][i] = drand48() * 20;
			//buildingBottomfront[j][i]  = drand48() * 20;
			//buildingBottomleft[j][i]  = drand48() * 20;
			//buildingBottomright[j][i] = drand48() * 20;

			//buildingTop[j][i] = drand48() * 20;
			//buildingTopfront[j][i]  = drand48() * 20;
			//buildingTopleft[j][i]  = drand48() * 20;
			//buildingTopright[j][i] = drand48() * 20;

			BHeight = (drand48() + 0.5) * 30;
		    BWidth = (drand48() + 0.5) * 15;
			BDepth = (drand48() + 0.5) * 15;
			
			CBHeight = (drand48() + 0.5) * 20;
		    CBWidth = (drand48() + 0.5) * 10;
			CBDepth = (drand48() + 0.5) * 10;

			if(BHeight < CBHeight)
			{
				BHeight += (CBHeight/2);
			}

			if(BWidth < CBWidth)
			{
				BWidth += (CBWidth/2);
			}		

			if(BDepth < CBDepth)
			{
				BDepth += (CBDepth/2);
			}

			init_geometrySimpleBuilding(&buildingBottom[j][i],program);
			init_geometrySimpleWindows(&windowBottomfront[j][i], program);
			init_geometrySimpleWindows2(&windowBottomleft[j][i], program);
			init_geometrySimpleWindows3(&windowBottomright[j][i], program);

			if ( isComplex[j][i] == 1)
			{
			init_geometryComplexBuilding(&buildingTop[j][i],program);
			init_geometryComplexWindows(&windowTopfront[j][i], program);
			init_geometryComplexWindows2(&windowTopleft[j][i], program);
			init_geometryComplexWindows3(&windowTopright[j][i], program);
			}

			else
			{
				// do nothing
			}	
		}
	}

	
	
}



//////////////////////////////////////




int main(int argc, char** argv)
{
	/* Initialize GLFW and GLEW */
	kuhl_ogl_init(&argc, argv, 512, 512, 32, 4);

	/* Specify function to call when keys are pressed. */
	glfwSetKeyCallback(kuhl_get_window(), keyboard);
	// glfwSetFramebufferSizeCallback(window, reshape);

	/* Compile and link a GLSL program composed of a vertex shader and
	 * a fragment shader. */


	




	kuhl_errorcheck();

	/* Good practice: Unbind objects until we really need them. */
	//glUseProgram(0);
	
	program1 = kuhl_create_program("texture.vert", "texture.frag");
	glUseProgram(program1);
	kuhl_errorcheck();

	init_geometryQuad(&triangle, program1);

	//init_geometryQuad(&triangle, program1);
	
	kuhl_errorcheck();
	/* Create kuhl_geometry structs for the objects that we want to
	 * draw. */
	//init_geometryTriangle(&triangle, program);
	//init_geometryCube(&triangle,program);

    //	init_geometryQuad(&quad, program);
    //	init_geometryCube(&triangle,program);
	glUseProgram(0);

	program = kuhl_create_program("infinicity.vert", "infinicity.frag");
	/* Use the GLSL program so subsequent calls to glUniform*() send the variable to
	   the correct program. */

	glUseProgram(program);

	//draw_building();
	//draw_complex_building();

	draw_them_all();
	




	dgr_init();     /* Initialize DGR based on environment variables. */

	float initCamPos[3]  = {120,70,500}; // location of camera
	float initCamLook[3] = {50,50,0}; // a point the camera is facing at
	float initCamUp[3]   = {0,1,0}; // a vector indicating which direction is up
	viewmat_init(initCamPos, initCamLook, initCamUp);

	while(!glfwWindowShouldClose(kuhl_get_window()))
	{
		display();
		kuhl_errorcheck();

		/* process events (keyboard, mouse, etc) */
		glfwPollEvents();
	}
	exit(EXIT_SUCCESS);
}
