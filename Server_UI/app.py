from flask import Flask, request, jsonify
from flask_sock import Sock
import threading
import requests

app = Flask(__name__)
sock = Sock(app)

clients = []

@sock.route('/connect')
def echo(ws):
    try:
        while True:
            message = ws.receive()
            if message:
                if message.startswith("CONNECT"):
                    clients.append(ws)
                    print(f"Client {message.split()[1]} Connected")
                    ws.send("Connection Success")
                else:
                    print(f'Client {message.split()[1]}: {message}')
            else:
                break
    finally:
        if ws in clients:
            clients.remove(ws)

@app.route('/send', methods=['POST'])
def send_message():
    message = request.json.get('message')
    if message:
        for client in clients:
            client.send(f"Broadcast: {message}")
        return jsonify({'status': 'Sent to all clients'}), 200
    else:
        return jsonify({'status': 'No message received'}), 400

@app.route('/shutdown', methods=['POST'])
def shutdown():
    func = request.environ.get('werkzeug.server.shutdown')
    if func:
        func()
    return 'Server shutting down...', 200

def run_app():
    app.run(host="localhost", port=5000)

if __name__ == "__main__":
    server_thread = threading.Thread(target=run_app)
    server_thread.daemon = True
    server_thread.start()

    print("Type a message to send to all WebSocket clients, or 'exit' to quit.")
    while True:
        cmd_message = input("Message: ")
        if cmd_message.lower() == "exit":
            requests.post("http://localhost:5000/shutdown")
            break
        response = requests.post("http://localhost:5000/send", json={"message": cmd_message})
        print("Response:", response.json())
