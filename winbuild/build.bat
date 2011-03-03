@echo off
nmake -f makefile.msc clean ROOT=%COMPILE_ENV%
if not %ERRORLEVEL%==0 goto exitt
nmake -f makefile.msc ROOT=%COMPILE_ENV%
if not %ERRORLEVEL%==0 goto exitt
	
FOR /F "TOKENS=1" %%D IN ('cd') DO set @dirname=%%D
cd %COMPILE_ENV%\out

rem no runtime package
rem if not %ERRORLEVEL%==0 goto exitt
rem zip -q -m -r ..\libzorpll\libzorpll3.9-0_3.9.0.1_i386.zip bin\*.*

if not %ERRORLEVEL%==0 goto exitt
zip -q -m -r ..\libzorpll\libzorpll-dev_3.9.0.1_i386.zip lib\*.* include\*.* debug\*.*
if not %ERRORLEVEL%==0 goto exitt

cd %@dirname%
if not %ERRORLEVEL%==0 goto exitt

rem no runtime package, no point in building a debug version
rem nmake -f makefile.msc clean ROOT=%COMPILE_ENV%
rem if not %ERRORLEVEL%==0 goto exitt
rem 
rem nmake -f makefile.msc DEBUG=1 ROOT=%COMPILE_ENV%
rem if not %ERRORLEVEL%==0 goto exitt
rem 
cd %COMPILE_ENV%\out
rem if not %ERRORLEVEL%==0 goto exitt
rem 
rem zip -q -m -r ..\libzorpll\libzorpll-dbg-dev_3.9.0.1_i386.zip bin\*.* debug\*.*
rem if not %ERRORLEVEL%==0 goto exitt
dir ..\libzorpll

:exitt
exit %ERRORLEVEL%

