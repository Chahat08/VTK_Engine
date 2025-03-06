# app/udp_listener.py

import socket
import struct
import threading
import json
import math
from .sockets import relay_message_to_clients

INDEX_COL_MAP = {6: 1, 7: 2, 8: 4, 9: 5}
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

            for i in [7, 7, 8, 8]: # NOTE:Assuming angles 6 is same as 7 and 8 is same as 9
                radians_val = float_values[i]
                degrees_val = radians_val * (180.0 / math.pi)  # rad->deg

                col = INDEX_COL_MAP[i]

                payload = {
                    "flexAngle": degrees_val,
                    "col": col
                }
                relay_message_to_clients(json.dumps(payload))

    thread = threading.Thread(target=listen, daemon=True)
    thread.start()
