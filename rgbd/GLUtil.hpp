//Copyright(c) 2015 Shuda Li[lishuda1980@gmail.com]
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//FOR A PARTICULAR PURPOSE AND NON - INFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.



#ifndef BTL_GL_UTIL
#define BTL_GL_UTIL
#define _USE_MATH_DEFINES
#include "DllExportDef.h"


namespace btl{	namespace gl_util
{
class  CGLUtil
{
public:
	//type
	typedef std::shared_ptr<CGLUtil> tp_shared_ptr;
	typedef CGLUtil* tp_ptr;
public:
	//************************************
	// Method:    CGLUtil
	// FullName:  btl::gl_util::CGLUtil::CGLUtil
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: ushort uResolution_
	// Parameter: ushort uPyrLevel_
	// Parameter: btl::utility::tp_coordinate_convention eConvention_: gl convention or cv convention
	// Parameter: const Eigen::Vector3f & eivCentroid_: the viewing position 
	//************************************
	CGLUtil(ushort uResolution_, ushort uPyrLevel_, const Eigen::Vector3f& eivCentroid_ = Eigen::Vector3f(1.5f,1.5f,0.3f));
	void clearColorDepth();
	void init();
	//to initialize for the interoperation with opengl
	static void setCudaDeviceForGLInteroperation();
	static void initCuda();

	void renderVoxelGL( const float fSize_) const;
	void renderAxisGL() const;
	void renderVoxelGL2( const float fSize_) const;
	void renderPatternGL(const float fSize_, const unsigned short usRows_, const unsigned short usCols_ ) const;
	// Esc: exit; g: zoom in; h:zoom out; 0: set to default position
	//   <: rotate around Y; >:rotate around Y
	void normalKeys ( unsigned char key, int x, int y );
	void specialKeys( int key, int x, int y );
	void mouseClick ( int nButton_, int nState_, int nX_, int nY_ );
	void mouseMotion ( int nX_, int nY_ );

	template< typename T >
	void renderDisk(const T& x, const T& y, const T& z, const T& dNx, const T& dNy, const T& dNz, 
		const unsigned char* pColor_, const T& dSize_, bool bRenderNormal_ );
	template< typename T > 
	void renderDiskFastGL(const T& x, const T& y, const T& z, const T& tAngle_, const T& dNx, const T& dNy,  
		const unsigned char* pColor_, const T& dSize_, bool bRenderNormal_ );

	template< typename T >
	void renderOctTree(const T& x, const T& y, const T& z, const T& dSize_, const unsigned short sLevel_ ) const;
	template< typename T >
	void renderVoxel( const T& x, const T& y, const T& z, const T& dSize_ ) const;
	template< typename T >
	void renderRectangle( const T* pPt1_, const T* pPt2_, const T* pPt3_, const T* pPt4_) const;
	void renderTestPlane();

	//create vertex buffer
	void createVBO(const unsigned int uRows, const unsigned int uCols_, const unsigned short usChannel_, const unsigned short usBytes_, GLuint* puVBO_, cudaGraphicsResource** ppResourceVBO_ );
	void releaseVBO( GLuint uVBO_, cudaGraphicsResource* pResourceVBO_ );
	//create pixel buffer
	void createPBO(const unsigned int uRows_, const unsigned int uCols_, const unsigned short usChannel_, const unsigned short usBytes_, GLuint* puPBO_, cudaGraphicsResource** ppResourcePixelBO_, GLuint* pTexture_);
	void releasePBO( GLuint uPBO_,cudaGraphicsResource *pResourcePixelBO_ );
	void constructVBOsPBOs();
	//void constructVBOsPBOs(ushort usLevel_);
	void destroyVBOsPBOs();
	//void destroyVBOsPBOs(ushort usLevel_);
	void gpuMapPtResources(const cv::cuda::GpuMat& cvgmPts_);
	void gpuMapNlResources(const cv::cuda::GpuMat& cvgmNls_);
	void gpuMapRGBResources(const cv::cuda::GpuMat& cvgmRGBs_);
	GLuint gpuMapRgb2PixelBufferObj(const cv::cuda::GpuMat& cvgmRGB_ );
	void errorDetectorGL() const;
	void viewerGL();
	void setInitialPos();
	static void getRTFromWorld2CamCV(Eigen::Matrix3f* pRw_, Eigen::Vector3f* pTw_);
	//
	void drawString(const char *str, int x, int y, float color[4], void *font) const;
	void initLights();
	void setOrthogonal();
	static void printShortCudaDeviceInfo(int nDeviceNO_) ;
	static int getCudaEnabledDeviceCount() ;
	int getLevel(int nCols_ );
public:
	Eigen::Affine3f _eimModelViewGL; //model view transformation matrix in GL convention.
	//double _adModelViewGL[16];
	float _fSize; //disk size
	bool _bRenderNormal;
	bool _bEnableLighting;
	bool _bDisplayCamera;
	bool _bRenderReference;
	bool _bCtlDown;
	unsigned short _usPyrHeight;
	bool _bVBOsPBOsCreated;
	ushort _usLevel;
	ushort _uResolution;
	//Cuda OpenGl interoperability
	GLuint _auPtVBO[4];
	cudaGraphicsResource* _apResourcePtVBO[4]; //3D points
	GLuint _auNlVBO[4];
	cudaGraphicsResource* _apResourceNlVBO[4]; //normal of 3D points
	GLuint _auRGBVBO[4];
	cudaGraphicsResource* _apResourceRGBVBO[4]; //for color of 3D points
	GLuint _auRGBPixelBO[4];
	cudaGraphicsResource* _apResourceRGBPxielBO[4];//2D image color
	GLuint _auTexture[4];
	GLuint _auGrayPixelBO[4];
	cudaGraphicsResource* _apResourceGrayPxielBO[4];//2D image gray
	GLuint _auGrayTexture[4];

private:
	GLuint _uDisk;
	GLuint _uNormal;
	GLuint _uVoxel;
	GLuint _uOctTree;
	GLUquadricObj *_pQObj;

	float _aCentroid[3];
	Eigen::Vector3f _eivCentroid;
public:
	double _dZoom;
	double _dZoomLast;
	double _dScale;

	double _dXAngle;
	double _dYAngle;
	double _dXLastAngle;
	double _dYLastAngle;
	double _dX;
	double _dY;
	double _dXLast;
	double _dYLast;

	int  _nXMotion;
	int  _nYMotion;
	int  _nXLeftDown, _nYLeftDown;
	int  _nXRightDown, _nYRightDown;
	bool _bLButtonDown;
	bool _bRButtonDown;
private:
	GLfloat _aLight[4];

	//btl::utility::tp_coordinate_convention _eConvention;

};//CGLUtil

template< typename T >
void btl::gl_util::CGLUtil::renderVoxel( const T& x, const T& y, const T& z, const T& dSize_ ) const
{
	glColor3f( 0.f,0.f,1.f );
	glPushMatrix();
	glTranslatef( x, y, z );
	glScalef( dSize_, dSize_, dSize_ );
	glCallList(_uVoxel);
	glPopMatrix();
};

template< typename T >
void CGLUtil::renderDisk(const T& x, const T& y, const T& z, const T& dNx, const T& dNy, const T& dNz, const unsigned char* pColor_, const T& dSize_, bool bRenderNormal_ )
{
	glColor3ubv( pColor_ );

	glPushMatrix();
	glTranslatef( x, y, z );

	float fAx,fAy,fA;
	fAx =-dNy; //because of cv-convention
	fAy = dNx;
	//normalization
	float norm = sqrtf(fAx*fAx + fAy*fAy );
	if( norm < 1.0e-10 ) return;
	fAx /= norm;
	fAy /= norm;
	fA = asin(norm)*180.f/M_PI;
	glRotatef( fA,fAx,fAy,0 );

	//T dA = atan2(dNx,dNz);
	//T dxz= sqrt( dNx*dNx + dNz*dNz );
	//T dB = atan2(dNy,dxz);

	//glRotatef(-dB*180 / M_PI,1,0,0 );
	//glRotatef( dA*180 / M_PI,0,1,0 );

	T dR = -z*2; //the further the disk the larger the size
	glScalef( dR*dSize_, dR*dSize_, dR*dSize_ );
	glCallList(_uDisk);
	if( bRenderNormal_ )
	{
		glCallList(_uNormal);
	}
	glPopMatrix();
};

template< typename T >
void CGLUtil::renderDiskFastGL(const T& x, const T& y, const T& z, const T& tAngle_, const T& dNx, const T& dNy, const unsigned char* pColor_, const T& dSize_, bool bRenderNormal_ )
{
	glColor3ubv( pColor_ );
	glPushMatrix();
	glTranslatef( x, y, z );
	//cross product
	glRotatef( tAngle_,dNx,dNy,0.f );
	T dR = -z*2; //the further the disk the larger the size
	glScalef( dR*dSize_, dR*dSize_, dR*dSize_ );
	glCallList(_uDisk);
	if( bRenderNormal_ )
	{glCallList(_uNormal);}
	glPopMatrix();
};

template< typename T >
void CGLUtil::renderOctTree(const T& x, const T& y, const T& z, const T& dSize_, const unsigned short sLevel_ ) const{
	if( 0==sLevel_ ) {renderVoxel<T>( x, y, z, dSize_);return;}
	//render next level 
	T tCx,tCy,tCz, tS = dSize_/4, tL = dSize_/2;
	tCx = x + tS;  tCy = y + tS;  tCz = z + tS;
	renderOctTree<T>(tCx,tCy,tCz,tL, sLevel_-1 );
	tCx = x + tS;  tCy = y + tS;  tCz = z - tS;
	renderOctTree<T>(tCx,tCy,tCz,tL, sLevel_-1 );
	tCx = x + tS;  tCy = y - tS;  tCz = z + tS;
	renderOctTree<T>(tCx,tCy,tCz,tL, sLevel_-1 );
	tCx = x - tS;  tCy = y + tS;  tCz = z + tS;
	renderOctTree<T>(tCx,tCy,tCz,tL, sLevel_-1 );

	tCx = x - tS;  tCy = y - tS;  tCz = z - tS;
	renderOctTree<T>(tCx,tCy,tCz,tL, sLevel_-1 );
	tCx = x - tS;  tCy = y - tS;  tCz = z + tS;
	renderOctTree<T>(tCx,tCy,tCz,tL, sLevel_-1 );
	tCx = x - tS;  tCy = y + tS;  tCz = z - tS;
	renderOctTree<T>(tCx,tCy,tCz,tL, sLevel_-1 );
	tCx = x + tS;  tCy = y - tS;  tCz = z - tS;
	renderOctTree<T>(tCx,tCy,tCz,tL, sLevel_-1 );
};	

template< typename T >
void CGLUtil::renderRectangle(const T* pPt1_, const T* pPt2_, const T* pPt3_, const T* pPt4_) const{
	glBegin(GL_POLYGON);
	glVertex3fv(pPt1_);
	glVertex3fv(pPt2_);
	glVertex3fv(pPt3_);
	glVertex3fv(pPt4_);
	glEnd();
}

}//gl_util
}//btl

#endif