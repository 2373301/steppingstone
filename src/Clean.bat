@echo Off
del /s /a  *.suo *.ncb *.user  *.netmodule *.aps *.ilk *.sdf *.iobj *.ipdb 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\x64" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Debug" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Release" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Obj" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\ReleaseD" 2>nul
