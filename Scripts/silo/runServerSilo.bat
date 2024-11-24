SETLOCAL
set HOST_IP=10.10.10.98
set PORT=5000
call env\Scripts\activate.bat
python app.py --host 0.0.0.0 --port %PORT%
ENDLOCAL