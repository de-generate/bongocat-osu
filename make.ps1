Write-Output ">>> running make";
wsl --exec make;
Write-Output ">>> moving bin/bongo.exe to .";
wsl --exec mv bin/bongo.exe .;
Write-Output ">>> executing bongo.exe";
./bongo.exe