#include "wrapper_gl.h"
#include <string.h>
#include <stdio.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

//TODO
//glClearDepth(1.0f);
//glClearStencil(0);				
//glDepthFunc(GL_LEQUAL);								
//glDepthMask(GL_TRUE/FALSE);								
//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//?GL_PERSPECTIVE_CORRECTION_HINT not in 3.x?
//glDisable(GL_BLEND);
#define BUFFER_OFFSET(i) ((char *)0 + (i))

//add more later
static const GLenum enumToGLTextureType[] = {
	//GL_TEXTURE_1D, 
	//GL_TEXTURE_1D_ARRAY, 
	GL_TEXTURE_2D, 
	//GL_TEXTURE_2D_ARRAY, 
	//GL_TEXTURE_2D_MULTISAMPLE,
	//GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
	//GL_TEXTURE_RECTANGLE, 
	GL_TEXTURE_CUBE_MAP,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	//GL_TEXTURE_3D,
	//GL_TEXTURE_BUFFER
};

static const GLboolean enumToGLBoolean[] = {GL_TRUE, GL_FALSE};
static const GLenum enumToGLDepthFuncParams[] = {GL_NEVER,GL_LESS,GL_EQUAL,GL_LEQUAL,GL_GREATER,GL_NOTEQUAL,GL_GEQUAL,GL_ALWAYS};
static const GLenum enumToGLBuffers[] = {GL_ELEMENT_ARRAY_BUFFER,GL_ARRAY_BUFFER/*,GL_TEXTURE_BUFFER,GL_TRANSFORM_FEEDBACK_BUFFER,GL_UNIFORM_BUFFER*/};
static const GLenum enumToGLBuffersBindings[] = {GL_ELEMENT_ARRAY_BUFFER_BINDING,GL_ARRAY_BUFFER_BINDING/*,GL_TEXTURE_BINDING_BUFFER,GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,GL_UNIFORM_BUFFER_BINDING*/};
static const GLenum enumToGLBufferUses[] = {GL_STREAM_DRAW, GL_STATIC_DRAW,GL_DYNAMIC_DRAW};
static const GLenum enumToGLPrimitiveType[] = {GL_POINTS,GL_TRIANGLES,GL_TRIANGLE_STRIP,GL_LINES,GL_LINE_STRIP};
static const GLenum enumToGLElementType[] = {GL_UNSIGNED_BYTE,GL_UNSIGNED_SHORT,GL_UNSIGNED_INT};
static const GLenum enumToGLAttribType[] = {GL_SHORT,GL_INT,GL_FLOAT};
static const GLenum enumToGLMask[] = {GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT, GL_COLOR_BUFFER_BIT};
static const GLenum enumToGLFrameBuffer[] = {/*GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER,*/ GL_FRAMEBUFFER};
static const GLenum enumToGLFrameBufferBlitFilter[] = {GL_NEAREST, GL_LINEAR};

//http://www.opengl.org/wiki/Image_Format#Texture_and_Renderbuffer
static const GLenum enumToGLTextureFormats[] = {
	GL_ALPHA, 
	GL_LUMINANCE, 
	GL_LUMINANCE_ALPHA, 
	GL_RGB, 
	GL_RGBA,
	GL_DEPTH_COMPONENT
/*
GL_DEPTH_COMPONENT,
GL_DEPTH_COMPONENT16, 
GL_DEPTH_COMPONENT24, 
GL_DEPTH_COMPONENT32,
GL_DEPTH_COMPONENT32F,
GL_DEPTH24_STENCIL8,
GL_DEPTH32F_STENCIL8,
GL_RGBA8, GL_RG8, GL_R8,
GL_RGBA16, GL_RG16, GL_R16,
GL_RGBA16F, GL_RG16F, GL_R16F,
GL_RGBA32F, GL_RG32F, GL_R32F,
GL_RGBA8I, GL_RG8I, GL_R8I,
GL_RGBA16I, GL_RG16I, GL_R16I,
GL_RGBA32I, GL_RG32I, GL_R32I,
GL_RGBA8UI, GL_RG8UI, GL_R8UI,
GL_RGBA16UI, GL_RG16UI, GL_R16UI,
GL_RGBA32UI, GL_RG32UI, GL_R32UI,
GL_RGB10_A2,GL_R11F_G11F_B10F,GL_SRGB8_ALPHA8,
//TEXTURE ONLY
GL_RGB8, GL_RGB16,
GL_RGBA8_SNORM, GL_RGB8_SNORM, GL_RG8_SNORM, GL_R8_SNORM,
GL_RGBA16_SNORM, GL_RGB16_SNORM, GL_RG16_SNORM, GL_R16_SNORM,
GL_RGB16F,GL_RGB32F,
GL_RGB32F,GL_RGB32F, 
GL_RGB8I, GL_RGB16I, GL_RGB32I,
GL_RGB8UI, GL_RGB16UI, GL_RGB32UI,
//END OF TEX ONLY
*/
};

//framebuffer formats
static const GLenum enumToGLFrameBufferFormats[] = {
	GL_RGBA4, GL_RGB565, GL_RGB5_A1, GL_DEPTH_COMPONENT16, GL_STENCIL_INDEX8
};
static const GLenum enumToGLPixelDataFormats[] = {GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA};
static const GLenum enumToGLPixelDataTypes[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_5_5_5_1};
static const GLenum enumToGLTextureParamName[] = {GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T};
static const GLenum enumToGLTextureParams[] = {GL_LEQUAL,GL_GEQUAL,GL_LESS,GL_GREATER,GL_EQUAL,GL_NOTEQUAL,GL_ALWAYS,GL_NEVER,GL_NONE, GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE};
static const GLenum enumToGLDrawBufferTypes[] = {GL_NONE, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK};
static const GLenum enumToGLDrawBuffersTypes[] = {GL_NONE, GL_COLOR_ATTACHMENT0};
//-------------------------------------------------
// Textures
//-------------------------------------------------

void ActiveTexture(const TextureUnit unit)  
{
	glActiveTexture(GL_TEXTURE0 + unit); 
}

void BindTexture(const STextureObject* const tx)
{
	glBindTexture(enumToGLTextureType[tx->descriptor.type], tx->handle);
}

void BindTextureToUnit(const STextureObject* const tx, const TextureUnit unit)
{
	ActiveTexture(unit);
	glBindTexture(enumToGLTextureType[tx->descriptor.type], tx->handle);
}

void CopyFrameRGBToTexture(unsigned int sx, unsigned int sy)
{
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, sx, sy, 0);
}

void CopyFrameDepthToTexture(unsigned int sx, unsigned int sy)
{
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, sx, sy, 0);
}

int CreateTextureData(STextureObject* const obj, const STextureDescriptor* const desc, const SPixelData* const data)
{
	glBindTexture(enumToGLTextureType[desc->type], obj->handle);
	if(glIsTexture(obj->handle) == GL_FALSE)
		return 0;
	if(desc->nSamples == 0)
	{
		//if(IS_1D_TEX(desc->type))
		//	glTexImage1D(enumToGLTextureType[desc->type], data.level, enumToGLTextureFormats[desc->fmt], desc->w, 0, enumToGLPixelDataFormats[data.fmt], enumToGLPixelDataTypes[data.type], data.pData);
		//else if(IS_2D_TEX(desc->type))
		glTexImage2D(enumToGLTextureType[desc->type], data->level, enumToGLTextureFormats[desc->fmt], desc->w, desc->h, 0, enumToGLPixelDataFormats[data->fmt], enumToGLPixelDataTypes[data->type], data->pData);
		//else if(IS_3D_TEX(desc->type))
		//	glTexImage3D(enumToGLTextureType[desc->type], data.level, enumToGLTextureFormats[desc->fmt], desc->w, desc->h, desc->d, 0, enumToGLPixelDataFormats[data.fmt], enumToGLPixelDataTypes[data.type], data.pData);

		//nor for multisamples textures
		//http://www.opengl.org/sdk/docs/man4/xhtml/glGenerateMipmap.xml
		if(desc->genMipMap == GEN_MIPMAP)
			glGenerateMipmap(enumToGLTextureType[desc->type]);
	}
	else
	{
		//if(IS_2D_TEX(desc->type))
		//	glTexImage2DMultisample(enumToGLTextureType[desc->type], desc->nSamples, enumToGLTextureFormats[desc->fmt], desc->w, desc->h, false);
		//else if(IS_3D_TEX(desc->type))
		//	glTexImage3DMultisample(enumToGLTextureType[desc->type], desc->nSamples, enumToGLTextureFormats[desc->fmt], desc->w, desc->h, desc->d, false);
	}
	return 1;
}

int CreateBaseTextureData(STextureObject* const obj, const STextureDescriptor* const desc, SPixelData* const data)
{
	if(glIsTexture(obj->handle) == GL_FALSE)
	{
		//http://www.opengl.org/wiki/GLAPI/glIsTexture
		//also need to bind it, before it becomes a texture (see link)
		glGenTextures(1, &obj->handle);
		if(obj->handle == 0) 
			return 0;
	}
	data->level = 0;
	obj->descriptor = *desc;
	return CreateTextureData(obj, desc, data);
}

int CreateBaseTexture(STextureObject* const obj, const STextureDescriptor* const desc)
{
	SPixelData data = {PX_RGB, PX_UNSIGNED_BYTE, 0, 0};
	//if(IS_DEPTH_TEXTURE(desc->fmt))
	//	data->fmt = DEPTH_COMPONENT;
	return CreateBaseTextureData(obj, desc, &data);
}

//TODO change data descriptor!
void UpdateTextureData(const STextureObject* const obj, const SPixelData* const data)
{
	glBindTexture(enumToGLTextureType[obj->descriptor.type], obj->handle);			
	if(obj->descriptor.nSamples == 0)
	{
		//if(IS_1D_TEX(obj->descriptor.type))
		//	glTexSubImage1D(enumToGLTextureType[obj->descriptor.type], data.level, 0, obj->descriptor.w, enumToGLPixelDataFormats[data.fmt], enumToGLPixelDataTypes[data.type], data.pData);
		//else if(IS_2D_TEX(obj->descriptor.type))
		glTexSubImage2D(enumToGLTextureType[obj->descriptor.type], data->level, 0,0, obj->descriptor.w, obj->descriptor.h, enumToGLPixelDataFormats[data->fmt], enumToGLPixelDataTypes[data->type], data->pData);
		//else if(IS_3D_TEX(obj->descriptor.type))
		//	glTexSubImage3D(enumToGLTextureType[obj->descriptor.type], data.level, 0,0,0, obj->descriptor.w, obj->descriptor.h, obj.descriptor.d, enumToGLPixelDataFormats[data.fmt], enumToGLPixelDataTypes[data.type], data.pData);
	}
	else
	{
		//impossible (right now)
	}
}

void SetTextureParams(const STextureObject* const obj , const STextureParam* const param, unsigned int count)
{
	unsigned int i=0;
	glBindTexture(enumToGLTextureType[obj->descriptor.type], obj->handle);			
	for(i=0;i<count; i++)
	{
		glTexParameteri(enumToGLTextureType[obj->descriptor.type], enumToGLTextureParamName[param[i].name], enumToGLTextureParams[param[i].param]);
	}
}

/* //not yet
void AttachTextureToBuffer(const Texture::SObject& obj, const unsigned int& bufID,  const Texture::Format& fmt)
{

	GLenum err = glGetError();
	//which one is right
	//http://www.opengl.org/wiki/GLAPI/glTexBuffer
	glBindTexture(GL_TEXTURE_BUFFER, obj.handle); //<- pretty sure this one is correct
	//glBindBuffer(GL_TEXTURE_BUFFER, buf);
	//check http://www.opengl.org/wiki/GLAPI/glTexBuffer#Description
	//for the correct formats
	glTexBuffer(GL_TEXTURE_BUFFER, enumToGLTextureFormats[fmt], bufID);
	//attaches the storage for the buffer object named <buf> to the active
    //buffer texture
	// When a buffer object is attached to a buffer texture, the buffer object's
    //data store is taken as the texture's texel array.  The number of texels in
    //the buffer texture's texel array is given by 
	err = glGetError();
}
*/

void DeleteTexture(STextureObject * const tx)
{
	glDeleteTextures(1,&tx->handle);
	tx->handle = 0;
}

//-------------------------------------------------
// Vertex buffers
//-------------------------------------------------
//VAO
//NOT YET
/*
unsigned int CreateVertexArrayObject(void)
{
	unsigned int vao=0;
	glGenVertexArrays(1,&vao);
	return vao;
}
void DeleteVertexArrayObject(unsigned int id)
{
	glDeleteVertexArrays(1, &id);	
}
void BindVertexArrayObject(unsigned int id)
{
	glBindVertexArray(id);
}
void UnbindVertexArrayObject(void)
{
	glBindVertexArray(0); //there is no default VAO?
}
*/

void SetAttributeFormat( const SVertexAttribute* const pAttr, unsigned int numAttr, unsigned int v_offset)
{
	unsigned int i=0;
	GLenum type = GL_FLOAT;		
	unsigned int currentVBO = GetCurrentBuffer(BUFFER_ARRAY);

	for(i=0; i<numAttr; i++)
	{
		if(pAttr[i].vbo != 0)
			BindBuffer(BUFFER_ARRAY, pAttr[i].vbo);

		glEnableVertexAttribArray(pAttr[i].idx);
		//glVertexAttribDivisor(pAttr[i].idx, pAttr[i].divisor); //doesn't exists yet
		
		switch(pAttr[i].type)
		{
		default:
		case STREAM_FLOAT:			
			glVertexAttribPointer(pAttr[i].idx, pAttr[i].size, enumToGLAttribType[pAttr[i].type], pAttr[i].normalized, pAttr[i].stride, BUFFER_OFFSET(v_offset + pAttr[i].offset));
			break;
		case STREAM_INT:
		case STREAM_SHORT:
			//this doesn't exist yet so just use the same one as the float
			//glVertexAttribIPointer(pAttr[i].idx, pAttr[i].size, enumToGLAttribType[pAttr[i].type], pAttr[i].stride, BUFFER_OFFSET(v_offset + pAttr[i].offset)); 
			glVertexAttribPointer(pAttr[i].idx, pAttr[i].size, enumToGLAttribType[pAttr[i].type], pAttr[i].normalized, pAttr[i].stride, BUFFER_OFFSET(v_offset + pAttr[i].offset));
			break;
		}
		if(pAttr[i].vbo != 0)
			BindBuffer(BUFFER_ARRAY, currentVBO);
	}
}


//create/destroy various buffers
unsigned int CreateBuffer(Buffer idx)
{
	unsigned int buffer=0;
	glGenBuffers(1, &buffer);
	return buffer;
}

unsigned int CreateBufferUsingData(Buffer type, BufferUse use, unsigned int size, const void* pData)
{
	unsigned int buffer=0;
	glGenBuffers(1, &buffer);
	glBindBuffer(enumToGLBuffers[type], buffer);
	glBufferData(enumToGLBuffers[type], size, pData, enumToGLBufferUses[use]);
	return buffer;
}

void DeleteBuffer(unsigned int id)
{
	glDeleteBuffers(1,&id);
}


void BindBuffer(Buffer idx, unsigned int id)
{
	glBindBuffer(enumToGLBuffers[idx], id);
}
unsigned int GetCurrentBuffer(Buffer idx)
{
	GLint id = 0;
	glGetIntegerv(enumToGLBuffersBindings[idx], &id);
	return id;
}
/*

void* MapBuffer(Buffer type, int id, int offset, int size, int access)
{
	BindBuffer(type,id);
	return MapBoundBuffer(type,offset,size,access);
}

void* MapBoundBuffer(Buffer type, int offset, int size, int access)
{
	//http://www.opengl.org/wiki/GLAPI/glMapBufferRange
	//GL_MAP_READ_BIT
	//GL_MAP_WRITE_BIT
	//GL_MAP_INVALIDATE_BUFFER_BIT; // GL_MAP_READ_BIT not allowed
	//GL_MAP_FLUSH_EXPLICIT_BIT;//GL_MAP_WRITE_BIT ONLY 
	//GL_MAP_UNSYNCHRONIZED_BIT;//
	GLbitfield bitAccess = 0;
	if(access & BUF_READ)
		bitAccess |=GL_MAP_READ_BIT;
	if(access & BUF_WRITE)
		bitAccess |=GL_MAP_WRITE_BIT;
	if(access & BUF_INVALIDATE)
		bitAccess |=GL_MAP_INVALIDATE_BUFFER_BIT;
	if(access & BUF_FLUSH)
		bitAccess |=GL_MAP_FLUSH_EXPLICIT_BIT;
	if(access & BUF_UNSYNC)
		bitAccess |=GL_MAP_UNSYNCHRONIZED_BIT;

	return glMapBufferRange(enumToGLBuffers[type], offset, size, bitAccess);	
}

void UnmapBuffer(Buffer type)
{
	glUnmapBuffer(enumToGLBuffers[type]);
}
*/
void UnbindBuffer(Buffer idx)
{
	glBindBuffer(enumToGLBuffers[idx], 0); //is this required?
}
/*
//TO A RANGE VERSION TOO
//void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size );
void BindBufferToIndex(Buffer type, unsigned int id, unsigned int idx)
{
	//OR is right here, stupid Analyze
	if(type != UNIFORM || type != FEEDBACK) //need to add atomic counter buffer too
		return
	glBindBufferBase(enumToGLBuffers[type], idx, id);

}
*/

void InitBuffer(Buffer type, BufferUse uidx, unsigned int id, unsigned int size, const void* pData)
{
	glBindBuffer(enumToGLBuffers[type], id);
	glBufferData(enumToGLBuffers[type], size, pData, enumToGLBufferUses[uidx]);
	glBindBuffer(enumToGLBuffers[type], 0);
}
void UpdateBuffer(Buffer idx, BufferUse uidx, unsigned int id, unsigned int size, const void * pData, unsigned int offset)
{
	glBindBuffer(enumToGLBuffers[idx], id);
	glBufferSubData(enumToGLBuffers[idx], offset, size, pData);
	glBindBuffer(enumToGLBuffers[idx], id);
}

void UpdateBoundBuffer(Buffer idx, BufferUse uidx, unsigned int size, const void * pData, unsigned int offset)
{
	glBufferSubData(enumToGLBuffers[idx], offset, size, pData);
}

/*
void FillArrayBufferFromFrameBuffer(unsigned int id, int x, int y, int w, int h,  Pixel::Format)
{
}

void FillTextureFromArrayBuffer(unsigned int id, unsigned int texID, int x, int y, int w, int h,  Pixel::Format)
{
}
*/

void Draw(DrawMode idx, ElementType elm, unsigned int nIndices, unsigned int i_offset)
{
	glDrawElements( enumToGLPrimitiveType[idx], nIndices, enumToGLElementType[elm], BUFFER_OFFSET(i_offset));	
}

void DrawArray(DrawMode idx, unsigned int count, unsigned int offset)
{
	glDrawArrays(enumToGLPrimitiveType[idx], offset, count);
}

/*
//no instancing yet

void DrawInstanced(DrawMode idx, ElementType elm, unsigned int nIndices, unsigned int i_offset, unsigned int n)
{
	glDrawElementsInstanced( enumToGLPrimitiveType[idx], nIndices, enumToGLElementType[elm], BUFFER_OFFSET(i_offset), n);
}

void DrawArrayInstanced(DrawMode idx, unsigned int count, unsigned int offset, unsigned int n)
{
	glDrawArraysInstanced( enumToGLPrimitiveType[idx], offset, count, n);
}
*/

//-------------------------------------------------
// Frame Buffers
//-------------------------------------------------
void CreateRenderBuffer(SRenderbufferObject* const rbo, const SRenderbufferDescriptor* const desc)
{
	rbo->descriptor = *desc;
	glGenRenderbuffers(1, &rbo->handle);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo->handle);
	//http://www.opengl.org/wiki/Image_Formats
	//http://www.opengl.org/sdk/docs/man4/xhtml/glRenderbufferStorage.xml
	glRenderbufferStorage(GL_RENDERBUFFER, enumToGLFrameBufferFormats[rbo->descriptor.fmt], rbo->descriptor.w, rbo->descriptor.h);
	//if we had this glRenderbufferStorageMultisample we could do the below
	//if nsamples is 0 it should created a nromal render buffer
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, rbo.descriptor.nSamples, enumToGLFrameBufferFormats[rbo.descriptor.fmt], rbo.descriptor.w, rbo.descriptor.h);
}

void DeleteRenderBuffer(SRenderbufferObject* const rbo)
{
	glDeleteRenderbuffers(1, &rbo->handle);
	rbo->handle = 0;	
}

void CreateFrameBuffer(SFramebufferObject* const fbo)
{
	glGenFramebuffers(1, &fbo->handle);
	//clear the attachments
	memset(fbo->attachment,0,sizeof(SFramebufferAttachment)*(ATT_CLR_MAX+1));
	fbo->depth_attachment.active = fbo->stencil_attachment.active = fbo->depth_stencil_attachment.active = 0;
}

void DeleteFrameBuffer(SFramebufferObject* const fbo)
{
	glDeleteFramebuffers(1, &fbo->handle);
	fbo->handle = 0;
	//clear the attachments
	memset(fbo->attachment,0,sizeof(SFramebufferAttachment)*(ATT_CLR_MAX+1));
	fbo->depth_attachment.active = fbo->stencil_attachment.active = fbo->depth_stencil_attachment.active = 0;
}

void AddRenderBuffer(SFramebufferObject* const fbo, const SRenderbufferObject* const rbo, const AttachmentTarget idx)
{
	if(idx >= ATT_MAX)
		return;
	//GL_DRAW_FRAMEBUFFER?
	//GL_READ_FRAMEBUFFER?
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->handle);	
	if(IS_COLOR_RENDERBUFFER(rbo->descriptor.fmt))
	{
		if(idx >= ATT_DEPTH)
		{
			printf("can't bind colour texture to depth component\n");
			return;
		}

		fbo->attachment[idx].type = ATT_RenderBuffer;
		//fbo->attachment[fbo->numColourAttachments].idx = idx;
		fbo->attachment[idx].active = 1;	
		fbo->attachment[idx].r = *rbo;
		//bind renderbuffer	to the framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+idx, GL_RENDERBUFFER, rbo->handle);		
	}
	else if(IS_DEPTH_RENDERBUFFER(rbo->descriptor.fmt))
	{
		//maybe check if it's already active?
		fbo->depth_attachment.type = ATT_RenderBuffer;
		fbo->depth_attachment.active = 1;	
		fbo->depth_attachment.r = *rbo;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo->handle);				
	}
	else if(IS_STENCIL_RENDERBUFFER(rbo->descriptor.fmt))
	{
		//maybe check if it's already active?
		fbo->depth_attachment.type = ATT_RenderBuffer;
		fbo->depth_attachment.active = 1;	
		fbo->depth_attachment.r = *rbo;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->handle);				
	}
	//TODO
	//else if(IS_STENCIL_RENDERBUFFER(rbo.descriptor.fmt))
	//else if(IS_DEPTH_STENCIL_RENDERBUFFER(rbo.descriptor.fmt))
}

//http://www.opengl.org/wiki/Framebuffer_Object#Framebuffer_Object_Structure
//according to the above link we can decide based on texture format :)
void AddTextureRenderBuffer(SFramebufferObject* const fbo, const STextureObject* const to, const AttachmentTarget idx)
{
	if(idx >= ATT_MAX)
		return;
	//GL_DRAW_RENDERBUFFER?
	//GL_READ_RENDERBUFFER?
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->handle);	
	if(IS_COLOR_TEXTURE(to->descriptor.fmt))
	{
		if(idx >= ATT_DEPTH)
		{
			printf("can't bind colour RBO to depth/stencil component\n");
			return;
		}
		fbo->attachment[idx].type = ATT_Texture;
		fbo->attachment[idx].active = 1;
		//fbo->attachment[fbo->numColourAttachments].idx = idx;
		fbo->attachment[idx].t = *to;

		//if(to->descriptor.type == Texture::TEX_1D)
		//	glFramebufferTexture1D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+idx, enumToGLTextureType[to->descriptor.type], to->handle, 0);
		//if(to->descriptor.type >= Texture::TEX_2D || to->descriptor.type <  Texture::TEX_3D)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+idx, enumToGLTextureType[to->descriptor.type], to->handle, 0);
		//if(to->descriptor.type == Texture::TEX_3D)//THIS SHOULD BE glFramebufferTextureLayer
		//	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+idx, enumToGLTextureType[to->descriptor.type], to->handle, 0, 0);
	}
	else if(IS_DEPTH_TEXTURE(to->descriptor.fmt))
	{
		//maybe check if it7s already active?
		fbo->depth_attachment.type = ATT_Texture;
		fbo->depth_attachment.active = 1;	
		fbo->depth_attachment.t = *to;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, enumToGLTextureType[to->descriptor.type], to->handle, 0);		
		//fbo->attachmentActive[fbo->numAttachments] = true; //needed?
		//fbo->attachmentType[fbo->numAttachments] = RBO_DEPTH;
	}
	else if(IS_STENCIL_TEXTURE(to->descriptor.fmt))
	{
		//maybe check if it7s already active?
		fbo->depth_attachment.type = ATT_Texture;
		fbo->depth_attachment.active = 1;	
		fbo->depth_attachment.t = *to;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, enumToGLTextureType[to->descriptor.type], to->handle, 0);		
		//fbo->attachmentActive[fbo->numAttachments] = true; //needed?
		//fbo->attachmentType[fbo->numAttachments] = RBO_DEPTH;
	}
	//todo
	//else if(IS_STENCIL_TEXTURE(to->descriptor.fmt))
	//else if(IS_DEPTH_STENCIL_TEXTURE(to->descriptor.fmt))
}

void InactivateColorAttachment(SFramebufferObject * const fbo, const AttachmentTarget idx)
{
	if(idx > ATT_CLR_MAX)
		return;
	fbo->attachment[idx].active = 0;	
}

void RemoveColorAttachment(SFramebufferObject * const fbo, const AttachmentTarget idx)
{
	if(idx > ATT_CLR_MAX)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo->handle);	
	
	//inactivate it
	if(fbo->attachment[idx].type  == ATT_Texture)
	{		
		//if(fbo->attachment[idx].t.descriptor.type == TEX_1D)
		//	glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+idx, enumToGLTextureType[fbo->attachment[idx].t.descriptor.type], 0, 0);
		//if(fbo->attachment[idx].t.descriptor.type >= TEX_2D || fbo->attachment[idx].t.descriptor.type <  Texture::TEX_3D)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+idx, enumToGLTextureType[fbo->attachment[idx].t.descriptor.type], 0, 0);
		//if(fbo->attachment[idx].t.descriptor.type == TEX_3D)
		//{	//THIS SHOULD BE glFramebufferTextureLayer
		//	glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+idx, enumToGLTextureType[fbo->attachment[idx].t.descriptor.type], 0, 0, 0);
		//}
	}
	else
	{	//Detach
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+idx, GL_RENDERBUFFER, 0);		
	}		
}

void RemoveDepthAttachment(SFramebufferObject * const fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->handle);	
	if(fbo->depth_attachment.active)
		return;

	fbo->depth_attachment.active = 0;
	if(fbo->depth_attachment.type  == ATT_Texture)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, enumToGLTextureType[fbo->depth_attachment.t.descriptor.type], 0, 0);
	}
	else
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);				
}

void RemoveStencilAttachment(SFramebufferObject * const fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->handle);	
	if(fbo->stencil_attachment.active)
		return;

	fbo->stencil_attachment.active = 0;
	if(fbo->stencil_attachment.type  == ATT_Texture)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, enumToGLTextureType[fbo->stencil_attachment.t.descriptor.type], 0, 0);
	}
	else
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);				
}
/*
//NOT YET
void RemoveDepthStencilAttachment(SFramebufferObject * const fbo)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->handle);	
	if(fbo->depth_stencil_attachment.active)
		return;

	fbo->depth_stencil_attachment.active = 0;
	if(fbo->depth_stencil_attachment.type  == ATT_Texture)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, enumToGLTextureType[fbo->depth_stencil_attachment.t.descriptor.type], 0, 0);
	}
	else
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
}*/

static void checkfbo(void)
{
	switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)) 
	{
		case GL_FRAMEBUFFER_COMPLETE: 
			printf("GL_FRAMEBUFFER_COMPLETE\n");
			return;
			break; 			 
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
			break;			
		//case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		//	d_printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
		//	break;
		//case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		//	d_printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
		//	break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED: 
			printf("GL_FRAMEBUFFER_UNSUPPORTED\n");
		    break;                                                 
        default:                                               			  
			printf("FBO Status Error!\n");
			break;
	}
}

void CheckFrameBuffer(SFramebufferObject*const fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->handle);	
	checkfbo();
	fbo->initialised = 1;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

void BindFrameBuffer(const FramebufferType type, const SFramebufferObject*const  fbo)
{
	glBindFramebuffer(enumToGLFrameBuffer[type], fbo->handle);
}

void UnbindFrameBuffer(const FramebufferType type)
{
	glBindFramebuffer(enumToGLFrameBuffer[type], 0);
}
/*
void BlitFrameBuffer(int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH, Mask mask, FrameBuffer::BlitFilter filter)
{
	unsigned int bitMask = 0;
	if(mask & COLOR_BIT)
		bitMask |= GL_COLOR_BUFFER_BIT;
	if(mask & DEPTH_BIT)
		bitMask |= GL_DEPTH_BUFFER_BIT;
	if(mask & STENCIL_BIT)
		bitMask |= GL_STENCIL_BUFFER_BIT;	

	glBlitFramebuffer(srcX, srcY, srcW, srcH, dstX, dstY, dstW, dstH, bitMask, enumToGLFrameBufferBlitFilter[filter]);
}

void SetDrawBuffer(const DrawBuffer buf)
{
	glDrawBuffer(enumToGLDrawBufferTypes[buf]);
}


void SetDrawBuffers(const unsigned int n, const DrawBuffers * const pABuf)
{
	//glDrawBuffersTypes
	GLenum aBuffers[16]; //16 should be max draw arrays or something
	for(unsigned int i=0;i<n;i++)
		aBuffers[i] = enumToGLDrawBuffersTypes[pABuf[i]];
	glDrawBuffers(n, aBuffers);
}
*/

//-------------------------------------------------	
void ClearScreenBuffer(Mask mask)
{
	unsigned int bitMask = 0;
	if(mask & COLOR_BIT)
		bitMask |= GL_COLOR_BUFFER_BIT;
	if(mask & DEPTH_BIT)
		bitMask |= GL_DEPTH_BUFFER_BIT;
	if(mask & STENCIL_BIT)
		bitMask |= GL_STENCIL_BUFFER_BIT;
	glClear(bitMask);
}

void SetupViewport(unsigned int startX, unsigned int startY, unsigned int width, unsigned int height)
{
	glViewport(startX, startY, width, height);
}

void UseScissor(int set)
{
	if(set)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void SetupScissor(unsigned int startX, unsigned int startY, unsigned int width, unsigned int height)
{
	glScissor(startX, startY, width, height);
}

//-------------------------------------------------
//various render modes
void Flush(void)
{
	
}

void Finish(void)
{
}

void EnableCulling(int set)
{
	if(set)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void CullMode(const Cull mode)
{
	if(mode == FRONT_FACE)
		glCullFace(GL_FRONT);
	else
		glCullFace(GL_BACK);
}	

void SetClearColour(const float r, const float g, const float b, const float a)
{
	glClearColor(r,b,g,a);
}

void EnableDepthTest(const int set)
{
	if(set)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void EnableDepthWriting(const int set)
{
	glDepthMask(enumToGLBoolean[set]);
}

void SetDepthFunction(const DepthFunc idx)
{
	glDepthFunc(enumToGLDepthFuncParams[idx]);
}