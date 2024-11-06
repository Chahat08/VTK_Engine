import json
from flask import Blueprint, render_template, request, redirect, url_for, jsonify
from .extensions import sock

main = Blueprint('main', __name__)

@main.route('/')
def index():
    return render_template('index.html')

@main.route('/submit', methods=['POST'])
def submit():
    message = request.form.get("message")
    if message:
        from .sockets import relay_message_to_clients
        relay_message_to_clients(message)
        return jsonify({"status": "Message sent to clients"}), 200
    else:
        return jsonify({"status": "No message provided"}), 400
