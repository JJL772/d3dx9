#pragma once

#include "appframework/iappsystem.h"

#ifdef USE_DXVK
#include "D3D9/d3d9.h"
#endif

#include "utlmemory.h"
#include "utlvector.h"

#ifdef USE_DXVK

#ifndef _MSC_VER
	#undef CONST
	#define CONST const

	#undef D3DXINLINE
	#define D3DXINLINE inline

	typedef const void* LPCVOID;
	typedef const char* LPCSTR;
	typedef size_t SIZE_T;
#endif

typedef void* VD3DHWND;
typedef void* VD3DHANDLE;
typedef DWORD   COLORREF;
#undef RGB
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

// ------------------------------------------------------------------------------------------------------------------------------ //
// Hacky macro fixes here!
#define D3DSAMP_SHADOWFILTER (D3DSAMPLERSTATETYPE)12
#define D3DRS_FORCE_DWORD 0x7fffffff /* Forces 32-bit sized enum in an enum */
#define D3DENUM_WHQL_LEVEL 0x00000002L
#define D3DSGR_NO_CALIBRATION 0x00000000L
#define D3DSAMP_FORCE_DWORD 0x7fffffff
#define D3DXSHADER_AVOID_FLOW_CONTROL (1 << 9)
#define D3DXSHADER_DEBUG (1 << 0)
// ------------------------------------------------------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------------------------------------------------------ //
// D3DX
// ------------------------------------------------------------------------------------------------------------------------------ //
typedef class CUtlMemory<D3DMATRIX> CD3DMATRIXAllocator;
typedef class CUtlVector<D3DMATRIX, CD3DMATRIXAllocator> CD3DMATRIXStack;

// ------------------------------------------------------------------------------------------------------------------------------ //
typedef enum _D3DXINCLUDE_TYPE
{
	D3DXINC_LOCAL,

	// force 32-bit size enum
	D3DXINC_FORCE_DWORD = 0x7fffffff

} D3DXINCLUDE_TYPE;
// ------------------------------------------------------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------------------------------------------------------ //
typedef struct _D3DXMACRO
{
	LPCSTR Name;
	LPCSTR Definition;

} D3DXMACRO, *LPD3DXMACRO;
// ------------------------------------------------------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------------------------------------------------------ //
// D3DX Matrix class
class D3DXMATRIX : public D3DMATRIX
{
public:
	D3DXMATRIX operator*( const D3DXMATRIX &o ) const;
	operator FLOAT* ();
	float& operator()( int row, int column );
	const float& operator()( int row, int column ) const;
	bool operator != ( CONST D3DXMATRIX& src ) const;
};
// ------------------------------------------------------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------------------------------------------------------ //
// D3DX 2D vector class
class D3DXVECTOR2
{
public:
	operator FLOAT* ();
	operator CONST FLOAT* () const;
	float x,y;
};
// ------------------------------------------------------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------------------------------------------------------ //
// D3DX 3D vector class
class D3DXVECTOR3 : public D3DVECTOR
{
public:
	D3DXVECTOR3() {}
	D3DXVECTOR3( float a, float b, float c );
	operator FLOAT* ();
	operator CONST FLOAT* () const;
};
// ------------------------------------------------------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------------------------------------------------------ //
// D3DX 4D vector class
class D3DXVECTOR4
{
public:
	D3DXVECTOR4() {}
	D3DXVECTOR4( float a, float b, float c, float d );
	float x,y,z,w;
};
// ------------------------------------------------------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------------------------------------------------------ //
// D3DX Plane
typedef struct D3DXPLANE
{
	float& operator[]( int i );
	bool operator==( const D3DXPLANE &o );
	bool operator!=( const D3DXPLANE &o );
	operator float*();
	operator const float*() const;
	float a, b, c, d;
} D3DXPLANE;
// ------------------------------------------------------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------------------------------------------------------ //
// Matrix stack class
struct ID3DXMatrixStack
{
		int	m_refcount[2];
	bool m_mark;
	CD3DMATRIXStack	m_stack;
	int						m_stackTop;	// top of stack is at the highest index, this is that index.  push increases, pop decreases.

	ID3DXMatrixStack();
	void  AddRef( int which=0, char *comment = NULL );
	ULONG Release( int which=0, char *comment = NULL );

	HRESULT	Create( void );

	D3DXMATRIX* GetTop();
	void Push();
	void Pop();
	void LoadIdentity();
	void LoadMatrix( const D3DXMATRIX *pMat );
	void MultMatrix( const D3DXMATRIX *pMat );
	void MultMatrixLocal( const D3DXMATRIX *pMat );
	HRESULT ScaleLocal(FLOAT x, FLOAT y, FLOAT z);

	// Left multiply the current matrix with the computed rotation
	// matrix, counterclockwise about the given axis with the given angle.
	// (rotation is about the local origin of the object)
	HRESULT RotateAxisLocal(CONST D3DXVECTOR3* pV, FLOAT Angle);

	// Left multiply the current matrix with the computed translation
	// matrix. (transformation is about the local origin of the object)
	HRESULT TranslateLocal(FLOAT x, FLOAT y, FLOAT z);
};
typedef ID3DXMatrixStack* LPD3DXMATRIXSTACK;
// ------------------------------------------------------------------------------------------------------------------------------ //

struct ID3DXInclude
{
	virtual HRESULT Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	virtual HRESULT Close(LPCVOID pData);
};
typedef ID3DXInclude* LPD3DXINCLUDE;

// ------------------------------------------------------------------------------------------------------------------------------ //
// D3DX Buffer
struct ID3DXBuffer : public IUnknown
{
	void* GetBufferPointer();
	DWORD GetBufferSize();
};
typedef ID3DXBuffer* LPD3DXBUFFER;
// ------------------------------------------------------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------------------------------------------------------ //

class ID3DXConstantTable : public IUnknown
{
};
typedef ID3DXConstantTable* LPD3DXCONSTANTTABLE;
// ------------------------------------------------------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------------------------------------------------------ //
// Memory status
typedef struct _MEMORYSTATUS
{
	DWORD dwLength;
	SIZE_T dwTotalPhys;
} MEMORYSTATUS, *LPMEMORYSTATUS;
// ------------------------------------------------------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------------------------------------------------------ //
// FUNCTIONS
const char* D3DXGetPixelShaderProfile( IDirect3DDevice9 *pDevice );

D3DXMATRIX* D3DXMatrixMultiply( D3DXMATRIX *pOut, CONST D3DXMATRIX *pM1, CONST D3DXMATRIX *pM2 );
D3DXVECTOR3* D3DXVec3TransformCoord( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV, CONST D3DXMATRIX *pM );

bool operator==(const GUID& a, const GUID& b);

HRESULT D3DXCreateMatrixStack( DWORD Flags, LPD3DXMATRIXSTACK* ppStack);
void D3DXMatrixIdentity( D3DXMATRIX * );

D3DXINLINE D3DXVECTOR3* D3DXVec3Subtract( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2 )
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
	return pOut;
}

D3DXINLINE D3DXVECTOR3* D3DXVec3Cross( D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2 )
{
	D3DXVECTOR3 v;

	v.x = pV1->y * pV2->z - pV1->z * pV2->y;
	v.y = pV1->z * pV2->x - pV1->x * pV2->z;
	v.z = pV1->x * pV2->y - pV1->y * pV2->x;

	*pOut = v;
	return pOut;
}

D3DXINLINE FLOAT D3DXVec3Dot( CONST D3DXVECTOR3 *pV1, CONST D3DXVECTOR3 *pV2 )
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

D3DXMATRIX* D3DXMatrixInverse( D3DXMATRIX *pOut, FLOAT *pDeterminant, CONST D3DXMATRIX *pM );

D3DXMATRIX* D3DXMatrixTranspose( D3DXMATRIX *pOut, CONST D3DXMATRIX *pM );

D3DXPLANE* D3DXPlaneNormalize( D3DXPLANE *pOut, CONST D3DXPLANE *pP);

D3DXVECTOR4* D3DXVec4Transform( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV, CONST D3DXMATRIX *pM );


D3DXVECTOR4* D3DXVec4Normalize( D3DXVECTOR4 *pOut, CONST D3DXVECTOR4 *pV );

D3DXMATRIX* D3DXMatrixTranslation( D3DXMATRIX *pOut, FLOAT x, FLOAT y, FLOAT z );

// Build an ortho projection matrix. (right-handed)
D3DXMATRIX* D3DXMatrixOrthoOffCenterRH( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,FLOAT zf );

D3DXMATRIX* D3DXMatrixPerspectiveRH( D3DXMATRIX *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

D3DXMATRIX* D3DXMatrixPerspectiveOffCenterRH( D3DXMATRIX *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf );

// Transform a plane by a matrix.  The vector (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
D3DXPLANE* D3DXPlaneTransform( D3DXPLANE *pOut, CONST D3DXPLANE *pP, CONST D3DXMATRIX *pM );

/* Implemented in the DXVK library */
extern "C" WINAPI IDirect3D9 *Direct3DCreate9(UINT SDKVersion);

void D3DPERF_SetOptions( DWORD dwOptions );

HRESULT D3DXCompileShader(
				LPCSTR                          pSrcData,
				UINT                            SrcDataLen,
				CONST D3DXMACRO*                pDefines,
				LPD3DXINCLUDE                   pInclude,
				LPCSTR                          pFunctionName,
				LPCSTR                          pProfile,
				DWORD                           Flags,
				LPD3DXBUFFER*                   ppShader,
				LPD3DXBUFFER*                   ppErrorMsgs,
				LPD3DXCONSTANTTABLE*            ppConstantTable);

//void dxvkGetClientRect( void *hWnd, RECT *destRect );
//#define toglGetClientRect dxvkGetClientRect

// fake D3D usage constant for SRGB tex creation
#define D3DUSAGE_TEXTURE_SRGB (0x80000000L)

#endif
