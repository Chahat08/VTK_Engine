# app/udp_listener.py

import socket
import threading
import json
from .sockets import relay_message_to_clients

COL_MAP = {0: 1, 1: 2, 2: 4, 3: 5}

def start_udp_listener(
    udp_host="localhost",
    udp_port=9999,
    buffer_size=1024
):
    """
    Starts a background thread to listen for UDP packets containing float data.
    Whenever data is received, it's relayed to all connected WebSocket clients.
    """

    def listen():
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind((udp_host, udp_port))
        
        print(f"UDP listener started on {udp_host}:{udp_port}")

        while True:
            data, addr = sock.recvfrom(buffer_size)
            message = data.decode("utf-8", errors="ignore").strip()
            if not message:
                continue
            
            try:
                float_strings = message.split(",")
                float_values = [float(x) for x in float_strings]
            except ValueError:
                continue

            for i, val in enumerate(float_values):
                payload = {
                    "flexAngle": val,
                    "col": COL_MAP.get(i, 0) 
                }
                relay_message_to_clients(json.dumps(payload))

    thread = threading.Thread(target=listen, daemon=True)
    thread.start()
