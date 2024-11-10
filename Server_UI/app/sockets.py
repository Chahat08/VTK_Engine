from .extensions import sock

clients = []

@sock.route('/connect')
def websocket_handler(ws):
    clients.append(ws)
    try:
        while True:
            message = ws.receive()
            if message:
                print("Received editor state:", message)  # Optional logging
                relay_message_to_clients(message)  # Send to other clients if needed
            else:
                break
    finally:
        clients.remove(ws)

def relay_message_to_clients(message):
    for client in clients:
        client.send(message)