
DTranps.dll: dlldata.obj DTran_p.obj DTran_i.obj
	link /dll /out:DTranps.dll /def:DTranps.def /entry:DllMain dlldata.obj DTran_p.obj DTran_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del DTranps.dll
	@del DTranps.lib
	@del DTranps.exp
	@del dlldata.obj
	@del DTran_p.obj
	@del DTran_i.obj
