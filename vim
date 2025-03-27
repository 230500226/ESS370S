curl -LO https://github.com/vim/vim-win32-installer/releases/latest/download/gvim_9.0.0_x64.zip
powershell -Command "Expand-Archive -Path gvim_9.0.0_x64.zip -DestinationPath ."
.\gvim\gvim.exe