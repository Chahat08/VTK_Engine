set HOST_IP=130.245.76.49
set PORT=5000
call env\Scripts\activate.bat
python app.py --host 0.0.0.0 --port %PORT%