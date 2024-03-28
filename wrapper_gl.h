#ifndef _WRAPPER_H_
#define _WRAPPER_H_

typedef enum
{
	FRONT_FACE,
	BACK_FACE,
}Cull;

typedef unsigned int Mask;
#define COLOR_BIT 0x1
#define DEPTH_BIT 0x2
#define STENCIL_BIT 0x4

//BUFFER_ACCESS
//#define BUF_READ			0x01
//#define BUF_WRITE			0x02
//#define BUF_INVALIDATE	0x04
//#define BUF_FLUSH			0x08
//#define BUF_UNSYNC		0x10


typedef enum  //Graphics Buffer?
{
	BUFFER_ELEMENTS=0, //check indices against glBuffers in the cpp file :)
	BUFFER_ARRAY,
	//BUFFER_TEXTURE,
	//BUFFER_FEEDBACK,
	//BUFFER_UNIFORM
}Buffer;

//shoudl probably also check the 
//STEAM and COPY variables
//see how much they change performance
typedef enum  //Graphics Buffer?
{
	BUFFER_STREAM_DRAW=0, //check indices against glBuffers in the cpp file :) 
	BUFFER_STATIC,
	BUFFER_DYNAMIC
}BufferUse;

typedef enum 
{
	STREAM_SHORT = 0,
	STREAM_INT,
	STREAM_FLOAT
}StreamType;

typedef struct 
{
	unsigned int	vbo;		// if 0 use the currently bound VBO
	unsigned int	idx;		// could probably be a char?
	unsigned int	size;		// usually 3 or 4
	StreamType		type;		//also a char?
	unsigned int	normalized; //0 false, 1 true //could be a char?
	unsigned int	stride;		
	unsigned int	offset;
	unsigned int	divisor;	//for instancing, not used yet in ES
}SVertexAttribute;

typedef enum 
{
	DRAW_POINTS = 0,
	DRAW_TRIANGLES,
	DRAW_TRIANGLE_STRIP,
	DRAW_LINES,
	DRAW_LINESTRIP
}DrawMode;

typedef enum 
{
	ELEMENT_BYTE = 0,
	ELEMENT_SHORT,
	ELEMENT_INT
}ElementType;

typedef enum 
{
	//Order is Important
	//TEX_1D = 0, 
	//TEX_1D_ARRAY, 
	TEX_2D, 
	//TEX_2D_ARRAY, 
	//TEX_2D_MULTISAMPLE,
	//TEX_2D_MULTISAMPLE_ARRAY,
	//TEX_RECTANGLE, 
	TEX_CUBE_MAP,
	TEX_CUBE_MAP_POSITIVE_X,
	TEX_CUBE_MAP_POSITIVE_Y,
	TEX_CUBE_MAP_POSITIVE_Z,
	TEX_CUBE_MAP_NEGATIVE_X,
	TEX_CUBE_MAP_NEGATIVE_Y,
	TEX_CUBE_MAP_NEGATIVE_Z,
	//TEX_3D,
	//TEX_BUFFER,
}TextureType;

#define IS_1D_TEX(TYP) (TYP<=Texture::TEX_1D_ARRAY)? true: false
#define IS_2D_TEX(TYP) (TYP>=Texture::TEX_2D && TYP<Texture::TEX_3D)? true: false
#define IS_3D_TEX(TYP) (TYP==Texture::TEX_3D)? true: false
#define IS_TEX_BUFFER(TYP) (TYP==Texture::TEX_BUFFER)? true: false

//http://www.opengl.org/wiki/Image_Format#Texture_and_Renderbuffer
typedef enum
{
	TEX_FMT_ALPHA, 
	TEX_FMT_LUMINANCE, 
	TEX_FMT_LUMINANCE_ALPHA, 
	TEX_FMT_RGB, 
	TEX_FMT_RGBA,
	TEX_FMT_DEPTH
	/*
	//Order is important
	DEPTH_COMPONENT=0,
	DEPTH_COMPONENT16, 
	DEPTH_COMPONENT24, 
	DEPTH_COMPONENT32, 
	DEPTH_COMPONENT32F,
	DEPTH24_STENCIL8, 
	DEPTH32F_STENCIL8,
	//
	RGBA8, RG8, R8,
	RGBA16, RG16, R16,
	RGBA16F, RG16F, R16F,
	RGBA32F, RG32F, R32F,
	RGBA8I, RG8I, R8I,
	RGBA16I, RG16I, R16I,
	RGBA32I, RG32I, R32I,
	RGBA8UI, RG8UI, R8UI,
	RGBA16UI, RG16UI, R16UI,
	RGBA32UI, RG32UI, R32UI,
	RGB10_A2,R11F_G11F_B10F,SRGB8_ALPHA8,
	//TEX ONLY FORMATS
	RGB8, RGB16,  
	RGBA8_SNORM, RG8_SNORM, R8_SNORM,
	RGBA16_SNORM, RGB16_SNORM, RG16_SNORM, R16_SNORM,
	RGB16F, RGB32F,
	RGB8I, RGB16I, RGB32I,
	RGB8UI, RGB16UI, RGB32UI,
	*/
}TextureFormat;

#define IS_DEPTH_TEXTURE(FMT) (FMT==TEX_DEPTH)? 1: 0
#define IS_COLOR_TEXTURE(FMT) (FMT<TEX_DEPTH)? 1: 0
#define IS_STENCIL_TEXTURE(FMT) 0//(FMT<=Texture::DEPTH32F_STENCIL8)? true: false
//#define IS_DEPTH_STENCIL(FMT) (FMT==Texture::DEPTH24_STENCIL8 || FMT==Texture::DEPTH32F_STENCIL8)? true: false

typedef enum 
{
	UNIT_0 = 0,
	UNIT_1,
	UNIT_2,
	UNIT_3,
	UNIT_4,
	UNIT_5,
	UNIT_6,
	UNIT_7,
	UNIT_8,
	UNIT_9,
	UNIT_10,
	UNIT_11,
	UNIT_12,
	UNIT_13,
	UNIT_14,
	UNIT_15,
	UNIT_MAX = UNIT_15,
	UNIT_INVALID = -1
}TextureUnit;

typedef enum 
{
	MIN_FILTER=0, 
	MAG_FILTER, 	
	WRAP_S, 
	WRAP_T, 
}TextureParamName;

//http://www.opengl.org/sdk/docs/man4/xhtml/glTexParameter.xml
//http://www.opengl.org/wiki/Texture#Filtering
typedef enum 
{
	//for COMPARE_FUNC
	PARAM_LEQUAL = 0,
	PARAM_GEQUAL,
	PARAM_LESS,
	PARAM_GREATER,
	PARAM_EQUAL,
	PARAM_NOTEQUAL,
	PARAM_ALWAYS,	
	PARAM_NEVER,
	// for MIN_FILTER
	PARAM_NEAREST,	//also for MAG_FILTER
	PARAM_LINEAR,		//also for MAG_FILTER
	PARAM_NEAREST_MIPMAP_NEAREST, // for mipmap stuff see link to filtering
	PARAM_LINEAR_MIPMAP_NEAREST, //bilinear
	PARAM_NEAREST_MIPMAP_LINEAR, //bilinear between mipmap levels
	PARAM_LINEAR_MIPMAP_LINEAR, //trilinear
	//FOR WRAP_*
	PARAM_CLAMP_TO_EDGE, 
	PARAM_MIRRORED_REPEAT, 
	PARAM_REPEAT
}TextureParam;

typedef enum 
{
	GEN_MIPMAP,
	DONT_GEN_MIPMAP
}Mipmap;

typedef struct 
{
	TextureType		type;
	TextureFormat	fmt;
	unsigned int	w, h, d;
	unsigned int	nSamples;
	Mipmap			genMipMap;
}STextureDescriptor;

typedef struct 
{
	unsigned int		handle; //0 = invalid 
	STextureDescriptor	descriptor;
}STextureObject;

typedef struct 
{
	TextureParamName	name;
	TextureParam		param;
}STextureParam;

//http://www.opengl.org/wiki/GLAPI/glTexImage2D#Description
//see "format"
typedef enum 
{
	 PX_ALPHA, 
	 PX_RGB, 
	 PX_RGBA, 
	 PX_LUMINANCE, 
	 PC_LUMINANCE_ALPHA
}PixelFormat;

typedef enum 
{
	PX_UNSIGNED_BYTE, 
	PX_UNSIGNED_SHORT_5_6_5, 
	PX_UNSIGNED_SHORT_4_4_4_4, 
	PX_UNSIGNED_SHORT_5_5_5_1
}PixelType;

typedef struct 
{
	PixelFormat	fmt;
	PixelType	type;
	int			level;
	void*		pData;
}SPixelData;


//http://www.opengl.org/wiki/Image_Format#Texture_and_Renderbuffer
typedef enum
{	//Order Is Important
	RB_RGBA4, RB_RGB565, RB_RGB5_A1, RB_DEPTH_COMPONENT16, RB_STENCIL_INDEX8
}RenderbufferFormat;

#define IS_DEPTH_RENDERBUFFER(FMT) (FMT==RB_DEPTH_COMPONENT16)? 1 : 0
#define IS_COLOR_RENDERBUFFER(FMT) (FMT>=RB_RGBA4 && FMT<RB_DEPTH_COMPONENT16)? 1 : 0
#define IS_STENCIL_RENDERBUFFER(FMT) (FMT==RB_STENCIL_INDEX8)? 1: 0
//#define IS_DEPTH_STENCIL_RENDERBUFFER(FMT) (FMT==RB_DEPTH24_STENCIL8 || FMT==RB_DEPTH32F_STENCIL8)? true: false

typedef struct 
{
	RenderbufferFormat	fmt;
	unsigned int		w, h;
	unsigned int		nSamples;		
}SRenderbufferDescriptor;

typedef struct 
{
	unsigned int			handle;
	SRenderbufferDescriptor	descriptor;
}SRenderbufferObject;

typedef enum 
{
	ATT_CLR0 = 0,	
	ATT_DEPTH,
	ATT_STENCIL,
	//ATT_DEPTH_AND_STENCIL,
	ATT_MAX,	
	ATT_CLR_MAX = ATT_CLR0
}AttachmentTarget;

typedef enum
{
	ATT_RenderBuffer,
	ATT_Texture
}FramebufferAttachmentType;

typedef struct 
{	
	int							active;
	FramebufferAttachmentType	type;
	union
	{
		SRenderbufferObject r;
		STextureObject t;
	};
}SFramebufferAttachment;

typedef enum 
{
	//DRAW = 0,
	//READ,
	BOTH = 0
}FramebufferType;

typedef enum 
{
	BLIT_NEAREST=0,
	BLIT_LINEAR
}BlitFilter;

typedef struct
{
	//static const int MAX_ATTACH = 8; //BAD, get this from GL_MAX_DRAW_BUFFERS instead
	int							initialised;
	unsigned int				handle;		//frame buffer object handle		
	SFramebufferAttachment		attachment[ATT_CLR_MAX+1];
	SFramebufferAttachment		depth_attachment;
	SFramebufferAttachment		stencil_attachment;
	SFramebufferAttachment		depth_stencil_attachment;
}SFramebufferObject;

typedef enum 
{
	DEPTH_NEVER,
	DEPTH_LESS,
	DEPTH_EQUAL,
	DEPTH_LEQUAL,
	DEPTH_GREATER,
	DEPTH_NOTEQUAL,
	DEPTH_GEQUAL,
	DEPTH_ALWAYS
}DepthFunc;

typedef enum 
{
	DB_NONE = 0,
	DB_FRONT,
	DB_BACK,
	DB_FRONT_AND_BACK,
}DrawBuffer;

typedef enum 
{
	DBS_NONE = 0,	
	DBS_CLR_ATT_0,	
}DrawBuffers;
//-------------------------------------------------
// Textures
//-------------------------------------------------
void ActiveTexture(const TextureUnit);
void BindTexture(const STextureObject* const tx);
void BindTextureToUnit(const STextureObject* const tx, const TextureUnit);
void CopyFrameRGBToTexture(unsigned int, unsigned int);
void CopyFrameDepthToTexture(unsigned int, unsigned int);

int CreateTextureData(STextureObject* const, const STextureDescriptor* const, const SPixelData* const);
int CreateBaseTexture(STextureObject* const, const STextureDescriptor* const );
int CreateBaseTextureData(STextureObject* const, const STextureDescriptor* const, SPixelData* const);

void UpdateTextureData(const STextureObject* const, const SPixelData* const);
void SetTextureParams(const STextureObject* const , const STextureParam* const param, unsigned int count);

//void AttachTextureToBuffer(const Texture::SObject&, const unsigned int& bufID, const Texture::Format&);
void DeleteTexture(STextureObject* const);

//-------------------------------------------------
// Vertex Array Objects, attributes
//-------------------------------------------------
//VAO not supported yet
//unsigned int	CreateVertexArrayObject(void);
//void			DeleteVertexArrayObject(unsigned int);
//void			BindVertexArrayObject(unsigned int);
//void			UnbindVertexArrayObject(void);		
void			SetAttributeFormat( const SVertexAttribute* const pAttr, unsigned int numAttr, unsigned int v_offset);
//-------------------------------------------------
// BUFFERS
//-------------------------------------------------
//create/destroy various buffers
unsigned int 	CreateBuffer(Buffer type);	
unsigned int 	CreateBufferUsingData(Buffer type, BufferUse, unsigned int size, const void* pData);
void 			DeleteBuffer(unsigned int bufferID);

void			BindBuffer(Buffer type, unsigned int);
unsigned int	GetCurrentBuffer(Buffer type);

//void*			MapBuffer(Buffer type, int id, int offset, int size, int access);
//void*			MapBoundBuffer(Buffer type, int offset, int size, int access);	
//void			UnmapBuffer(Buffer type);	
void			UnbindBuffer(Buffer type);	
//void			BindBufferToIndex(Buffer type, unsigned int id, unsigned int idx);

void			InitBuffer(Buffer type, BufferUse, unsigned int id, unsigned int size, const void* pData);
void			UpdateBuffer(Buffer type, BufferUse, unsigned int id, unsigned int size, const void * pData, unsigned int offset);
void		 	UpdateBoundBuffer(Buffer type, BufferUse, unsigned int size, const void * pData, unsigned int offset);

//void			FillArrayBufferFromFrameBuffer(unsigned int id, int x, int y, int w, int h,  Pixel::Format);
//void			FillTextureFromArrayBuffer(unsigned int id, unsigned int texID, int x, int y, int w, int h,  Pixel::Format);

//-------------------------------------------------
// Drawing
//-------------------------------------------------
void Draw(DrawMode mode, ElementType elm, unsigned int nIndices, unsigned int i_offset);
void DrawArray(DrawMode mode, unsigned int count, unsigned int offset);
//void DrawInstanced(DrawMode mode, ElementType elm, unsigned int nIndices, unsigned int i_offset, unsigned int n);
//void DrawArrayInstanced(DrawMode mode, unsigned int count, unsigned int offset, unsigned int n);

//-------------------------------------------------
// Frame Buffers
//-------------------------------------------------
void CreateRenderBuffer(SRenderbufferObject* const rbo, const SRenderbufferDescriptor* const desc);
void DeleteRenderBuffer(SRenderbufferObject* const rbo);
void CreateFrameBuffer(SFramebufferObject* const fbo);
void DeleteFrameBuffer(SFramebufferObject* const fbo);
void AddRenderBuffer(SFramebufferObject* const fbo, const SRenderbufferObject* const rbo, const AttachmentTarget idx);
void AddTextureRenderBuffer(SFramebufferObject* const fbo, const STextureObject* const to, const AttachmentTarget idx);

void InactivateColorAttachment(SFramebufferObject * const fbo, const AttachmentTarget idx);
void RemoveColorAttachment(SFramebufferObject * const fbo, const AttachmentTarget idx);
void RemoveDepthAttachment(SFramebufferObject * const fbo);
void RemoveStencilAttachment(SFramebufferObject * const fbo);
void RemoveDepthStencilAttachment(SFramebufferObject * const fbo);
void CheckFrameBuffer(SFramebufferObject* const fbo);

void BindFrameBuffer(const FramebufferType type, const SFramebufferObject* const fbo);
void UnbindFrameBuffer(const FramebufferType type);
//void BlitFrameBuffer(int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH, Mask mask, FrameBuffer::BlitFilter filter);
//unsigned int GetMaxFrameBuffers(void);
//void SetDrawBuffer(const DrawBuffer);
//void SetDrawBuffers(const unsigned int n, const DrawBuffers * const);

//-------------------------------------------------	
void ClearScreenBuffer(Mask mask);
void SetupViewport(unsigned int startX, unsigned int startY, unsigned int width, unsigned int height);
void UseScissor(int );
void SetupScissor(unsigned int startX, unsigned int startY, unsigned int width, unsigned int height);

//-------------------------------------------------
//various render modes
void Flush(void);
void Finish(void);	
//OpenGL's default front face is CCW
void EnableCulling(int);
void CullMode(const Cull);	
void SetClearColour(const float r, const float g, const float b, const float a);

void EnableDepthTest(const int);
void EnableDepthWriting(const int);
void SetDepthFunction(const DepthFunc);


#endif //_WRENDER_HPP_
