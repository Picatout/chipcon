@echo OFF
set FDIR=C:\Users\Jacques\Downloads\CHIP-8\Winter\SGAMES\
set list=liste.txt
del %list%

for   %%a in ("%FDIR%*.SC") do call :test  %%~na
goto :end  

:test
echo "%1\n" >> %list%
cvt-chip.exe %FDIR%%1.sc
 
:end

