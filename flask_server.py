from flask import Flask, request, jsonify
import os
import time
from gtts import gTTS
import supervision as sv
from ultralytics import YOLO

app = Flask(__name__)

frames_folder = 'Team36FinalProject/frames'
os.makedirs(frames_folder, exist_ok=True)

tts_foldes = 'Team36FinalProject/tts'
os.makedirs(tts_foldes, exist_ok=True)

model = YOLO('yolov8s.pt')

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
    filename = 'frame.jpg'  # You can generate a unique name if needed
    filepath = os.path.join(frames_folder, filename)
    with open(filepath, 'wb') as f:
        f.write(image_data)
        
    # Perform object detection
    result = model(filepath)
    detections = sv.Detections.from_ultralytics(result)
    detections = detections[detections.confidence > 0.5]
    labels = [
        f"{result.names[class_id]}: {confidence:.2f}"
        for class_id, confidence in zip(detections.class_id, detections.confidence)
    ]
    print(labels)
    if len(labels) == 0:
        return jsonify({'message': 'No objects detected'}), 200
    else:
        return jsonify({'message': 'Objects detected', 'labels': labels}), 200 
       
    # return jsonify({'message': 'File uploaded successfully'}), 200


@app.route('/esp32/get_images', methods=['GET'])
def get_files():
    files = os.listdir(frames_folder)
    return jsonify({'files': files}), 200


@app.route('/esp32/delete_images', methods=['DELETE'])
def delete_files():
    files = os.listdir(frames_folder)
    for file in files:
        filepath = os.path.join(up, file)
        file_time = os.path.getmtime(filepath)
        if time.time() - file_time > 600: # 10 minutes in seconds
            os.remove(filepath)
    return jsonify({'message': 'Files deleted successfully'}), 200


@app.route('/esp32/post_and_get_tts_voice', methods=['POST'])
def post_and_get_tts_voice():
    # Check if the content type is text/plain
    if request.content_type != 'text/plain':
        return jsonify({'error': 'Invalid content type. Expected text/plain'}), 400

    # Get the text data from the request
    object_name = request.form.get('object_name')

    if not object_name:
        return jsonify({'error': 'No text data received'}), 400

    voiceCall = "Careful! There is a " + object_name + "in front of you!"
    tts = gTTS(text=voiceCall, lang='en')

    tts.save
    return jsonify({'message': 'Text received and TTS voice generated'}), 200

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)