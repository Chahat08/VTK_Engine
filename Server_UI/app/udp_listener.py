# app/udp_listener.py

import socket
import struct
import threading
import json
from .sockets import relay_message_to_clients

COL_MAP = {0: 1, 1: 2, 2: 4, 3: 5}
NUM_FLOATS = 10
BUFFER_SIZE = NUM_FLOATS * 4  # 40 bytes, if each float is 4 bytes

def start_udp_listener(
    udp_host="0.0.0.0",  
    udp_port=9999
):
    def listen():
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind((udp_host, udp_port))
        
        print(f"UDP listener started on {udp_host}:{udp_port}")

        while True:
            data, addr = sock.recvfrom(BUFFER_SIZE)
            try:
                float_values = struct.unpack("<10f", data)
            except struct.error:
                continue

            print(float_values)

            # for i, val in enumerate(float_values):
            #     payload = {
            #         "flexAngle": val,
            #         "col": COL_MAP.get(i, 0) 
            #     }
            #     relay_message_to_clients(json.dumps(payload))

    thread = threading.Thread(target=listen, daemon=True)
    thread.start()
