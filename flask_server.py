from flask import Flask, request, jsonify
from werkzeug.utils import secure_filename
import os
import time

app = Flask(__name__)

UPLOAD_FOLDER = 'uploads'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

@app.route('/')
def landing_page():
    return "This is landing page"

@app.route('/esp32/post_images', methods=['POST'])
def upload_file():
    # Check if the content type is image/jpeg
    if request.content_type != 'image/jpeg':
        return jsonify({'error': 'Invalid content type. Expected image/jpeg'}), 400

    # Get the image data from the request
    image_data = request.get_data()

    if not image_data:
        return jsonify({'error': 'No image data received'}), 400

    # Generate a filename and save the image
    filename = 'image.jpg'  # You can generate a unique name if needed
    filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
    with open(filepath, 'wb') as f:
        f.write(image_data)

    return jsonify({'message': 'File uploaded successfully'}), 200

@app.route('/esp32/get_images', methods=['GET'])
def get_files():
    files = os.listdir(app.config['UPLOAD_FOLDER'])
    return jsonify({'files': files}), 200

@app.route('/esp32/delete_images', methods=['DELETE'])
def delete_files():
    files = os.listdir(app.config['UPLOAD_FOLDER'])
    for file in files:
        filepath = os.path.join(app.config['UPLOAD_FOLDER'], file)
        file_time = os.path.getmtime(filepath)
        if time.time() - file_time > 600: # 10 minutes in seconds
            os.remove(filepath)
    return jsonify({'message': 'Files deleted successfully'}), 200

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)