SETLOCAL
set HOST_IP=172.20.10.4
set PORT=5000
call env\Scripts\activate.bat
python app.py --host 0.0.0.0 --port %PORT%
ENDLOCAL