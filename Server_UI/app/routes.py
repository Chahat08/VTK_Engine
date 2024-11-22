import json
from flask import Blueprint, render_template, request, redirect, url_for, jsonify
from .extensions import sock
import os

main = Blueprint('main', __name__)

@main.route('/')
def index():
    websocket_ip = os.getenv('HOST_IP', '0.0.0.0')
    websocket_port = os.getenv('PORT', '5000')
    websocket_url = f"ws://{websocket_ip}:{websocket_port}/connect"
    return render_template('index.html', websocket_url=websocket_url)

@main.route('/submit', methods=['POST'])
def submit():
    message = request.form.get("message")
    if message:
        from .sockets import relay_message_to_clients
        relay_message_to_clients(message)
        return jsonify({"status": "Message sent to clients"}), 200
    else:
        return jsonify({"status": "No message provided"}), 400
