#include <win32_game\graphics_device.h>
#include <core/scope.h>
#include <glad/glad.h>
#include <glad/glad_wgl.h>
#include <windows.h>

namespace
{
	void *GetAnyGLFuncAddress(const char *name)
	{
		void *p = (void *)wglGetProcAddress(name);
		if (p == 0 ||
			(p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
			(p == (void*)-1))
		{
			HMODULE module = LoadLibraryA("opengl32.dll");
			p = (void *)GetProcAddress(module, name);
		}

		return p;
	}
}
namespace graphics
{

std::unique_ptr<Device> make_device(ptrdiff_t in_hwnd)
{
	HWND hwnd = reinterpret_cast<HWND>(in_hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	HDC device_context = GetDC(hwnd);
	int nPixelFormat = ChoosePixelFormat(device_context, &pfd);
	if (nPixelFormat == 0) 
		throw std::exception("ChoosePixelFormat failed");

	BOOL bResult = SetPixelFormat(device_context, nPixelFormat, &pfd);
	if (!bResult) 
		throw std::exception("SetPixelFormat failed");

	HGLRC dummy_context = wglCreateContext(device_context);
	core::OnExitScope scope_exit([=]()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(dummy_context);
	});
	if (TRUE != wglMakeCurrent(device_context, dummy_context))
		throw std::exception("Failed to make dummy context current");
	
	// make proper GL context
	if (!gladLoadWGL(device_context))
		throw std::exception("glad failed to load WGL!");
	if (!gladLoadGL())
		throw std::exception("glad failed to load OpenGL!");

	const int contextAttribsList[] = 
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 4,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef _DEBUG
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#else
		WGL_CONTEXT_FLAGS_ARB, 0,
#endif
		0,
	};
	HGLRC rc = wglCreateContextAttribsARB(device_context, nullptr, contextAttribsList);
	if ( rc == nullptr )
	{
		throw std::exception("Failed to create core OpenGL context");
	}
	
	return std::make_unique<Device>(std::make_unique<Device_Impl>(device_context, rc));
}

class Device_Impl 
{
	friend class Device;
public:

	Device_Impl(HDC dc, HGLRC rc)
	:	m_device_context(dc), 
		m_gl_context(rc)
	{}

	~Device_Impl() {};
	HDC m_device_context = nullptr;
	HGLRC m_gl_context = nullptr;
};

Device::Device(std::unique_ptr<Device_Impl> impl)
:	m_impl(std::move(impl))
{
}

Device::~Device()
{
	
}

void Device::clear(core::Real r, core::Real g, core::Real b)
{
	glClearColor(r, g, b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Device::begin_frame()
{
	wglMakeCurrent(m_impl->m_device_context, m_impl->m_gl_context);
}

void Device::end_frame()
{
	wglMakeCurrent(NULL, NULL);
	::SwapBuffers(m_impl->m_device_context);
}

}