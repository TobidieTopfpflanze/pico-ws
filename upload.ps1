ninja -C ./build 
picotool reboot -f -u
timeout /t 3
$uf2 = Get-ChildItem .\build\*.uf2
picotool load $uf2.FullName -f -x